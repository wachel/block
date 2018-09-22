// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#pragma	once

#include "v2ObjBase.h"
#include "v2XmlVisitor.h"
#include "v2BinVisitor.h"

#include "..\Common\v2STL.h"
//#include "v2Exception.h" // import ooGurad

OO_NAMESPACE_BEGIN
class OObject;
OO_NAMESPACE_END
typedef void (*FUN_onReadObjectsNum)(void *arg, int num);
typedef void (*FUN_onReadObjectsItem)(void *arg, oo::OObject *obj);

//////////////////////////////////////////////////////////////////////////

OO_NAMESPACE_BEGIN

class OBJ_API OClass
{
public:
	// @note reserved, no use current.
	enum Flag {
		flagFinal = 1 << 0,		// end class. no class derived from it.
		flagAbstract = 1 << 1,	// abstract class, should not be concreted.
		flagConcrete = 1 << 2,
		flagPluginInterface = (1 << 3) | (flagAbstract),
		flagPluginImp = (1 << 4) | (flagConcrete), // the implemention of a plugin.
		flagAddinInterface = (1 << 5) | (flagAbstract),
		flagAddinImp = (1 << 6) | (flagConcrete),
		flagHide = (1 << 7), // hide this class in editor.
		flagDev = (1 << 8), // in-developing, not finished or not tested yet.
							// class with this flag will be hidden when OObject::bDevMode == true.
		flagEditorOnly = (1<<9),//skip load this object when OObject::bClient == true,
		flagSkip = (1<<10),//skip load this object,just for test
		flagAll = 0xffffffff,
	};

	// Attributes
	const char* m_className;
	int m_objectSize;
	Dword flags;
	Dword version;
	OObject* (*m_funCreateObject)(); // 0 => abstract class

	OClass* (* m_funGetBaseClass)();
	typedef OList<OClass*>::Type List;
	OClass *child;
	OClass *sibling;

	// OClass objects linked together in simple list
	OClass* next;       // linked list of registered classes

	// 如果Lib中有OObject，同时多个DLL依赖与这个Lib，则会导致同名OClass多次注册，此时记住每个类的master(第一个注册的同名类，其余为slave)。
	// ooIsA等判断操作时都以master为准判断。
	OClass *master;

	// Operations
	OObject* createObject();
	OClass* getBaseClass() {
		ooBreakAssert(m_funGetBaseClass);
		return (*m_funGetBaseClass)();
	}

	bool isDerivedFrom(const OClass* pBaseClass) const;

	/// Find the one of the drived classes, return the first matched one.
	// static OClass* findDerivedFrom(OClass *cls);

	/// Find all sub classes with specified flags.
	int findSubs(List &lst, Dword flags = flagAll, bool recursive = true);

	bool hasFlags(Dword f) const {
		return (flags & f) == f;
	}
	bool isHidden() const;

	/** 从类名创建类实例。
		@param className 类名字。
		@param bIncludeAlias 是否查找别名。

		bIncludeAlias 用于类名字改变的升级，应用程序可以用registerAlias注册类的别名，
		OObject::loadFromFile装载文件时是支持别名的。
	*/
	static OClass* fromName(const char* className, bool bIncludeAlias = false);

	/** 注册一个类的别名。
		@param className 类名字。
		@param classAlias 类的别名。

		className, classAlias字符串可以是临时变量，该函数内部保存了字符串的备份。
	*/
	static bool registerAlias(const char* className, const char* classAlias);

	bool isMaster() const {
		return master == this;
	}
	int calcSlaveNum() const;
};

//////////////////////////////////////////////////////////////////////////

#define WRITE_VERSION(CLASS, arg, bSuper) if (bSuper) { \
	visitMeta<Dword>("__superVersion_"#CLASS, arg.classVersion, "Dword", arg, 0, 0); \
}

// 下面给arg.classVersion赋值初值是为了防止旧版本没有super类号(注释放这是因为宏里面没法用"//"注释，同时用"/* */"注释不好，因为"/* */"没法嵌套)
#define READ_VERSION_XML(CLASS, arg, bSuper) \
	if (bSuper) { \
		arg.classVersion = 0; \
		visitMeta<Dword>("__superVersion_"#CLASS, arg.classVersion, "Dword", arg, 0, 0); \
	}

// 下面给arg.classVersion赋值初值是为了防止旧版本没有super类号(注释放这是因为宏里面没法用"//"注释，同时用"/* */"注释不好，因为"/* */"没法嵌套)
#define READ_VERSION_BIN(CLASS, arg, bSuper) \
	if (bSuper) { \
		arg.classVersion = 0; \
		if (arg.format >= BF_B04) \
			visitMeta<Dword>("__superVersion_"#CLASS, arg.classVersion, "Dword", arg, 0, 0); \
	}

class OAttribute;
class ArgBase;
class OObject;
typedef OList<OObject*>::Type ObjectList;
class VFileSystemBase;
enum FileType {
	FT_Xml,
	FT_Bin,
};

/** 引擎对象系统基类。
*/
class OBJ_API OObject : public OUnknown
{
public:
	OO_DECL_PTR(OObject)

	enum StoreType
	{
		storeEmbedded,  // embeded object.
		storeReference, // reference a global object.
		storeNull,		// Null object pointer.
	};

	OObject();
	virtual ~OObject();

// Memory manage.

#if OO_MEMORY_TRACE	
	static void* operator new(size_t size, const char *sourceFile, int sourceLine);
	static void operator delete(void* ptr, const char *sourceFile, int sourceLine);

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	// placement. @todo ...unfinished.
	//void* operator new(size_t, void* p);
	//void operator delete(size_t, void* p);

	static void* operator new[](size_t size, const char *sourceFile, int sourceLine);
	static void operator delete[](void* ptr, const char *sourceFile, int sourceLine);

	static void* operator new[](size_t size);
	static void operator delete[](void* ptr);

#endif // #if OO_MEMORY_TRACE	

// Serialize.
	enum { verOObject = 0 };

	virtual void _objVisit(ArgWriteXml &arg, bool bSuper = false) { 
		Dword savedVer = arg.classVersion;
		arg.classVersion = verOObject;
		WRITE_VERSION(OObject, arg, bSuper);
		// ...write OObject attributes...
		arg.classVersion = savedVer;
	}
	virtual void _objVisit(ArgReadXml &arg, bool bSuper = false) { 
		Dword savedVer = arg.classVersion;
		READ_VERSION_XML(OObject, arg, bSuper);
		// ...read OObject attributes...
		arg.classVersion = savedVer;
	}

	virtual void _objVisit(ArgWriteBin &arg, bool bSuper = false) { 
		Dword savedVer = arg.classVersion;
		arg.classVersion = verOObject;
		WRITE_VERSION(OObject, arg, bSuper);
		// ...write OObject attributes...
		arg.classVersion = savedVer;
	}
	virtual void _objVisit(ArgReadBin &arg, bool bSuper = false) { 
		Dword savedVer = arg.classVersion;
		READ_VERSION_BIN(OObject, arg, bSuper);
		// ...read OObject attributes...
		arg.classVersion = savedVer;
	}

	virtual void _objVisit(ArgBase &arg) { }

	template <class T_ARG> void objVisit(T_ARG &arg) { }

	bool hasObject(OObject *obj, bool recursive = true);

	bool isA(const OClass* pClass) const;
	virtual OClass* getClass() const;

	static OClass _CLASS;
	static OClass*  getBaseClass();

	void setUrl(const String& url) {
		m_url = url;
	}

	const String& getUrl() const {
		return m_url;
	}

	static OObject* findByUrl(const String &url);
	static int findByClass(ObjectList &lst, OClass *cls, bool isEndClass = true);

	static OObject* createObject(const String &clsName);
	static OObject* loadObject(VFileSystemBase* fileSystem, const String &url) {
		return loadFromFile(fileSystem,url);
	}
	static OObject* importObject(VFileSystemBase* fileSystem, const String &url);
	static OObject* cloneObject(OObject *obj, Dword fileVersion = 0);
	static OObject* cloneObject(OObject *srcObj, OObject* dstObj,Dword fileVersion );

	// 如果之前对象已经被装入,fileVersion将不会被改变。
	static OObject* loadFromFile(VFileSystemBase* fileSystem, const String &path, Dword *fileVersion = 0);
	bool saveToFile(VFileSystemBase* fileSystem, const String &path, FileType type = FT_Xml, Dword fileVersion = 0);

	// 如果之前对象已经被装入,fileVersion将不会被改变。
	static OObject* loadFromXml(VFileSystemBase* fileSystem, const String &path, Dword *fileVersion = 0);
	bool saveToXml(VFileSystemBase* fileSystem, const String &path, Dword fileVersion = 0);

	// 如果之前对象已经被装入,fileVersion将不会被改变。
	static OObject* loadFromBin(VFileSystemBase* fileSystem, const String &path, Dword *fileVersion = 0);
	bool saveToBin(VFileSystemBase* fileSystem, const String &path, Dword fileVersion = 0);

	bool saveUrlObjects(VFileSystemBase* fileSystem, bool recursive = true, FileType type = FT_Xml, Dword fileVersion = 0);

	// 当对象在属性编辑器里编辑后会被调用。
	virtual void postEdited(OAttribute *attri) {}

	// 当对象装载后会被调用。
	virtual void postLoaded(Dword clsVer) {
		postLoaded(); // 兼容旧版
	}
	virtual void postLoaded() {} // 旧版格式

	// 当对象保存后会被调用。
	virtual void postSaved() {}

	// 当对象保存前会被调用。
	virtual void beforeSaved() {}

	// 读取回调函数，显示装载精度用。
	static void* onReadObjectsCallbackArg;
	static FUN_onReadObjectsNum onReadObjectsNumCallback;
	static FUN_onReadObjectsItem onReadObjectsItemCallback;

	// 装载错误提示信息。
	enum LoadingErrorCode
	{
		LEC_None,
		LEC_Warning,
		LEC_Error,			// OError
		LEC_FillNotFound,	// EFileNotFound
		LEC_ClassNotFound,	// EClassNotFound
		LEC_Unknown,
	};
	typedef void (*FUN_onLoadingError)(void *userArg, LoadingErrorCode e, const String &desc);

	// 注册装载错误回调函数。
	static void registerLoadingErrorCallback(FUN_onLoadingError fun, void *userArg);

	// 提示装载错误信息，编辑器时显示在错误提示窗口。
	static void addLoadingError(LoadingErrorCode errorCode, const String& errorDesc);

	static bool bDevMode; // whether the app is running in develop mode.
	static bool bClientMode;//false -> Editor,true -> Client

	static int getObjectNum() {
		return _numObj;
	}
	static OObject* _getHeadObj() {
		return _objListHead;
	}
	static OObject* _getTailObj() {
		return _objListTail;
	}
	OObject* _getNextObj() {
		return _nextObj;
	}
	OObject* _getPrevObj() {
		return _prevObj;
	}
protected:
	String m_url;

// 对象链表
	static int _numObj;
	static OObject* _objListHead;
	static OObject* _objListTail;

	OObject *_nextObj;
	OObject *_prevObj;
	void _linkObj();
	void _unlinkObj();

	// copy constructor
	OObject(const OObject &);
	OObject& operator=(const OObject &);
};

#if OO_MEMORY_TRACE

#define		objNew(CLS) new(__FILE__, __LINE__) CLS
#define		objNewA(CLS, NUM) new(__FILE__, __LINE__) CLS[NUM]

#define		objDel #error 如果不使用智能指针，必须要 ooIncRef,ooDecRef 配对使用，永远不要 delete 。
#define		objDelA #error 如果不使用智能指针，必须要 ooIncRef,ooDecRef 配对使用，永远不要 delete 。

#else // #if OO_MEMORY_TRACE

#define		objNew(CLS) new CLS
#define		objNewA(CLS, NUM) new CLS[NUM]

#define		objDel #error 如果不使用智能指针，必须要 ooIncRef,ooDecRef 配对使用，永远不要 delete 。
#define		objDelA #error 如果不使用智能指针，必须要 ooIncRef,ooDecRef 配对使用，永远不要 delete 。

#endif // #if OO_MEMORY_TRACE

#define ooIncRef(OBJ) do { if (OBJ) (OBJ)->addRef(); } while(false)
#define ooDecRef(OBJ) do { if (OBJ) (OBJ)->release(); } while(false)
#define ooSafeDecRef(OBJ) do { if (OBJ) (OBJ)->release(); (OBJ) = 0; } while(false)

template<class T>
inline void objSafeDel(T* &p)
{
	if(p) { 
		delete p;
		p = 0;
	}
}

template<class T>
inline void objSafeDelA(T* &p)
{
	if(p) { 
		delete[] p;
		p = 0;
	}
}

template<class T>
inline void objSafeRelease(T* &p)
{
	if(p) { 
		p->release();
		p = 0;
	}
}

// Object Manager.
OBJ_API ObjectList& getRefObjects();
OBJ_API void addRefObj(OObject *obj);
OBJ_API void removeRefObj(OObject *obj);
OBJ_API void setObjUrl(OObject *obj, const String& url);

// Helper macros

/// 返回指定类型的OClass。
#define OO_CLASS(CLASS) ((OClass*)(&CLASS::_CLASS)->master)
#define _OO_CLASS_LOCAL(CLASS) ((OClass*)(&CLASS::_CLASS)) // 可能非master的类，内部使用。

/// 断言一个对象是一个类或者是它的子类。
#define ooAssertIsA(CLASS, object) \
	ooAssert((object) && (object)->isA(OO_CLASS(CLASS)))

/// 判断一个对象是否是一个类或者是它的子类。
#define ooIsA(CLASS, object) \
	((object) && (object)->isA(OO_CLASS(CLASS)))

/// 判断一个对象是否是一个类并且不是它的子类。
#define ooIsEndClass(CLASS, object) \
	((object) && (object)->getClass() == OO_CLASS(CLASS))

/// 用于Lib中的OObject类别判断。
#define ooIsSameClass(CLASS_A, CLASS_B) \
	((CLASS_A)->master == (CLASS_B)->master)


// RTTI helper macros/functions
OBJ_API OObject*  ooDynamicDownCast(OClass* pClass, OObject* pObject);
#define ooDynamicCast(CLASS, object) \
	(CLASS*)ooDynamicDownCast(OO_CLASS(CLASS), object)

/** 带类型检测的装载对象。
	
	从 PATH 装入一个类型是 CLASS 的对象，如果文件找不到或者类型不是 CLASS 均返回 NULL，CLASS 包含它的类。
	警告：由于临时对象返回问题，该函数要求接收改返回值的变量 ×必须× 是一个智能指针！
	如：Model::Ptr mdl = ooSafeLoadFromFile(Model, path); // 没问题
	    Model *mdl = ooSafeLoadFromFile(Model, path);     // 错误！可能导致对象刚返回就被析构。
*/
#define ooSafeLoadFromFile(CLASS, PATH) ooDynamicCast(CLASS, OObject::Ptr(OObject::loadFromFile(PATH)))

#ifdef _DEBUG
OBJ_API OObject*  ooStaticDownCast(OClass* pClass, OObject* pObject);
#define ooStaticCast(CLASS, object) \
	((CLASS*)ooStaticDownCast(OO_CLASS(CLASS), object))
#else
#define ooStaticCast(CLASS, object) ((CLASS*)object)
#endif

// static cast a OPointer.
#define ooCast(CLASS, obj) (obj ? (CLASS*)(obj).pointer() : (CLASS*)0)

// generate static object constructor for class registration
void OBJ_API ooClassInit(OClass* newClass, OClass *baseClass, Dword flags, Dword ver);
struct OBJ_API OO_CLASSINIT 
{ 
	OO_CLASSINIT(OClass* newClass, OClass* baseClass, Dword flags, Dword ver) {
		ooClassInit(newClass, baseClass, flags, ver); 
	} 
};

//////////////////////////////////////////////////////////////////////////
// OAttribute

class OObject;
class ObjectValue;

class OBJ_API OAttribute : public OUnknown
{
public:
	OO_DECL_PTR(OAttribute)

	enum Type {
		typeMeta,
		typeMetaArray,
		typeObject,
		typeObjArray,
		typeObjList,
	};

	Type type;
	const char* name;
	Dword flags;

	// possible values for object.
	ObjectValue *objectValue;

	OAttribute() : type((Type)-1), name(0), flags(0), objectValue(0) {}
	virtual ~OAttribute();

	OAttribute& operator= (const OAttribute &rhs);

	virtual OObject* getObject() { return 0; }

	bool hasFlags(Dword f) const {
		return (flags & f) == f;
	}

	// override OUnknown::doRelease.
	virtual void  doRelease();
};

enum AttriFlag
{
	AF_Hide					= 1 << 0,
	AF_ReadOnly				= 1 << 1,	// 对象和对象下面的属性都只读（目前还不支持对象里面的属性只读）
	AF_Object_FromSelected	= 1 << 2,	// 来自于文件的对象，属性编辑器里面可以选择文件，从文件装载对象
	AF_Dev					= 1 << 3,	// 正在开发中属性，当OObject::bDevMode == true时才显示出来，可以通过"-dev"命令行参数启动
	AF_Object_AvoidNull		= 1 << 4,	// 对象不能被付空值
	AF_Path					= 1 << 5,	// 尚未完成
	AF_NameNoExt			= 1 << 6,	// 尚未完成
};

class OBJ_API MetaAttri : public OAttribute
{
public:
	OO_DECL_PTR(MetaAttri)

	const char* typeName;
	void *data;

	MetaAttri() : typeName(0), data(0) { type = typeMeta; }
};

class OBJ_API ObjAttri : public OAttribute
{
public:
	OO_DECL_PTR(ObjAttri)

	union {
		OObject **object;
		OObject::Array *objArray;
		OObject::List *objList;
	};
	OClass *objectClass;

	ObjAttri() : object(0), objectClass(0) { type = typeObject; }

	virtual OObject* getObject() { return (type == typeObject && object) ? *object : 0; }
};

//////////////////////////////////////////////////////////////////////////
// meta

template <class T>
struct MetaType {
	MetaType() { COMPILER_ASSERT(false); }
};

template<>
struct MetaType<int> {
	typedef int Type;
	static const char* name() { return "int"; }
};

template<>
struct MetaType<Real> {
	typedef Real Type;
	static const char* name() { return "Real"; }
};

template<>
struct MetaType<String> {
	typedef String Type;
	static const char* name() { return "String"; }
};

//template<>
//struct MetaType<Vector3> {
//	typedef Vector3 Type;
//	static const char* name() { return "Vector3"; }
//};

//////////////////////////////////////////////////////////////////////////
// ArgBase

class OBJ_API ArgBase
{
public:
	virtual ~ArgBase() {}

	virtual void visitMeta(const char* name, void *data, const char* typeName, Dword flags, ObjectValue *val) = 0;
	virtual void visitMetaArray(const char* name, void *data, const char* typeName, Dword flags, ObjectValue *val) = 0;

	virtual void visitObject(const char* name, OObject* &obj, OClass *cls, Dword flags, ObjectValue *val) = 0;
	virtual void visitObjArray(const char* name, OObject::Array &obj, OClass *cls, Dword flags, ObjectValue *val) = 0;
	virtual void visitObjList(const char* name, OObject::List &obj, OClass *cls, Dword flags, ObjectValue *val) = 0;
};

template <class T_OBJ>
void visitMeta(const char* name, T_OBJ &obj, const char *type, ArgBase &arg, Dword flags, ObjectValue *val)
{
	arg.visitMeta(name, &obj, type, flags, val);
}

template <class T_OBJ>
void visitMetaArray(const char* name, typename OArray<T_OBJ>::Type &obj, const char* type, ArgBase &arg, Dword flags, ObjectValue *val)
{
	arg.visitMetaArray(name, &obj, type, flags, val);
}

template <class T_OBJ>
void visitObject(const char* name, T_OBJ* &obj, OClass *cls, ArgBase &arg, Dword flags, ObjectValue *val)
{
	arg.visitObject(name, reinterpret_cast<OObject*&>(obj), cls, flags, val);
}

/*
template <class T_OBJ, class T_CON>
void visitObjArray(const char* name, T_CON &obj, OClass *cls, ArgBase &arg, Dword flags, ObjectValue *val)
{
	//COMPILER_ASSERT(false);
	arg.visitObjArray(name, reinterpret_cast<T_CON&>(obj), cls, flags, val);
}
*/

template <class T_OBJ>
void visitObjArray(const char* name, typename T_OBJ::Array &obj, OClass *cls, ArgBase &arg, Dword flags, ObjectValue *val)
{
	arg.visitObjArray(name, reinterpret_cast<OObject::Array &>(obj), cls, flags, val);
}

template <class T_OBJ>
void visitObjList(const char* name, typename T_OBJ::List &obj, OClass *cls, ArgBase &arg, Dword flags, ObjectValue *val)
{
	arg.visitObjList(name, reinterpret_cast<OObject::List&>(obj), cls, flags, val);
}

class OBJ_API AttriVisitor : public ArgBase
{
public:
	OAttribute::List attributes;;

	virtual ~AttriVisitor() {}

	virtual void visitMeta(const char* name, void *data, const char* typeName, Dword flags, ObjectValue *val);
	virtual void visitMetaArray(const char* name, void *data, const char* typeName, Dword flags, ObjectValue *val);
	virtual void visitObject(const char* name, OObject* &obj, OClass *cls, Dword flags, ObjectValue *val);
	virtual void visitObjArray(const char* name, OObject::Array &obj, OClass *cls, Dword flags, ObjectValue *val);
	virtual void visitObjList(const char* name, OObject::List &obj, OClass *cls, Dword flags, ObjectValue *val);
};


//////////////////////////////////////////////////////////////////////////
// TEST

#define OO_DECL_ABSTRACT(CLASS, FLAGS, VER) \
	public: \
	OO_DECL_PTR(CLASS) \
	virtual void _objVisit(ArgWriteXml &arg, bool bSuper = false); \
	virtual void _objVisit(ArgReadXml &arg, bool bSuper = false); \
	virtual void _objVisit(ArgWriteBin &arg, bool bSuper = false); \
	virtual void _objVisit(ArgReadBin &arg, bool bSuper = false); \
	virtual void _objVisit(ArgBase &arg); \
	static OClass _CLASS; \
	enum { flags##CLASS = FLAGS, ver##CLASS = VER}; \
	virtual OClass* getClass() const;\
	static OClass* getBaseClass();\
	template <class T_ARG> void CLASS##_objVisit(T_ARG &arg)

#define OO_DECL_CLASS(CLASS, FLAGS, VER) \
	protected: \
	static OObject*  createObject(); \
	virtual void  doRelease();\
	OO_DECL_ABSTRACT(CLASS, FLAGS, VER)


#define _IMPL_COMMON(CLASS, BASE) \
	OClass* CLASS::getClass() const { return OO_CLASS(CLASS); } \
	OClass* CLASS::getBaseClass() { return OO_CLASS(BASE); } \
	OO_CLASSINIT _init_##CLASS(_OO_CLASS_LOCAL(CLASS), _OO_CLASS_LOCAL(BASE), CLASS::flags##CLASS, CLASS::ver##CLASS);\
	void CLASS::_objVisit(ArgWriteXml &arg, bool bSuper) { \
		ooGuard(#CLASS"_objVisit(ArgWriteXml&)"); \
		BASE::_objVisit(arg, true); \
		Dword savedVer = arg.classVersion; \
		arg.classVersion = ver##CLASS; \
		WRITE_VERSION(CLASS, arg, bSuper); \
		CLASS##_objVisit(arg); \
		arg.classVersion = savedVer; \
	} \
	void CLASS::_objVisit(ArgReadXml &arg, bool bSuper) { \
		ooGuard(#CLASS"_objVisit(ArgReadXml&)"); \
		BASE::_objVisit(arg, true); \
		if ((flags##CLASS & OClass::flagPluginImp) != OClass::flagPluginImp) { \
			Dword savedVer = arg.classVersion; \
			READ_VERSION_XML(CLASS, arg, bSuper); \
			if (ver##CLASS < arg.classVersion) \
				logWarning(LM_Core, "从文件中读取的类版本比本程序更新！Class: %s, CurVer: %d, ReadVer: %d, Path: %s", _OO_CLASS_LOCAL(CLASS)->m_className, ver##CLASS, arg.classVersion, arg.filename.c_str()); \
			CLASS##_objVisit(arg); \
			arg.classVersion = savedVer; \
		} \
	} \
	void CLASS::_objVisit(ArgWriteBin &arg, bool bSuper) { \
		ooGuard(#CLASS"_objVisit(ArgWriteBin&)"); \
		BASE::_objVisit(arg, true); \
		Dword savedVer = arg.classVersion; \
		arg.classVersion = ver##CLASS; \
		WRITE_VERSION(CLASS, arg, bSuper); \
		CLASS##_objVisit(arg); \
		arg.classVersion = savedVer; \
	} \
	void CLASS::_objVisit(ArgReadBin &arg, bool bSuper) { \
		ooGuard(#CLASS"_objVisit(ArgReadBin&)"); \
		BASE::_objVisit(arg, true); \
		if ((flags##CLASS & OClass::flagPluginImp) != OClass::flagPluginImp) { \
			Dword savedVer = arg.classVersion; \
			READ_VERSION_BIN(CLASS, arg, bSuper); \
			if (ver##CLASS < arg.classVersion) \
				logWarning(LM_Core, "从文件中读取的类版本比本程序更新！Class: %s, CurVer: %d, ReadVer: %d, Path: %s", _OO_CLASS_LOCAL(CLASS)->m_className, ver##CLASS, arg.classVersion, arg.filename.c_str()); \
			CLASS##_objVisit(arg); \
			arg.classVersion = savedVer; \
		} \
	} \
	void CLASS::_objVisit(ArgBase &arg) { \
		ooGuard(#CLASS"_objVisit(ArgBase&)"); \
		BASE::_objVisit(arg); \
		CLASS##_objVisit(arg); \
	}

//#define OBJ_ATTRI() template <class T_ARG> void CLASS##_objVisit(T_RRG &arg)
//OBJ_GUARD(A) void _objGuard();A

#define OO_IMPL_ABSTRACT(CLASS, BASE) \
	_IMPL_COMMON(CLASS, BASE) \
	OClass CLASS::_CLASS = { const_cast<char*>(#CLASS), sizeof(class CLASS), 0, 0, 0, &CLASS::getBaseClass, 0, 0, 0 };

#define OO_IMPL_CLASS(CLASS, BASE) \
	_IMPL_COMMON(CLASS, BASE) \
	OObject*  CLASS::createObject() { return objNew(CLASS); } \
	void  CLASS::doRelease() { delete this; } \
	OClass CLASS::_CLASS = { /*name*/const_cast<char*>(#CLASS), /*size*/sizeof(class CLASS), /*flag*/0, /*version*/0, CLASS::createObject, &CLASS::getBaseClass, /*child*/0, /*silbing*/0, /*next*/0, /*master*/0 };

/*
// @temp
#define OO_DECLARE_CLASS(CLASS, FLAG)
#define OO_IMPLEMENT_CLASS(CLASS, BASE)
#define OO_ATTRI_BEGIN(CLASS, BASE)
#define OO_ATTRI_END()
*/

template <class T>
inline T*& _convertObjPtr(T* &obj) {
	return obj;
}

template <class T>
inline T*& _convertObjPtr(OPointer<T> &obj) {
	return obj.pointerRef();
}

#define VMETA(DATA, TYPE) visitMeta<TYPE>(#DATA, DATA, #TYPE, arg, 0, 0);
#define VMETA_ARRAY(DATA, TYPE) visitMetaArray<TYPE>(#DATA, DATA, #TYPE, arg, 0, 0);

#define VOBJ(DATA, TYPE) visitObject<TYPE>(#DATA, _convertObjPtr(DATA), OO_CLASS(TYPE), arg, 0, 0);
#define VOBJ_ARRAY(DATA, TYPE) visitObjArray<TYPE>(#DATA, DATA, OO_CLASS(TYPE), arg, 0, 0);
#define VOBJ_LIST(DATA, TYPE) visitObjList<TYPE>(#DATA, DATA, OO_CLASS(TYPE), arg, 0, 0);

/*
#define VOBJ_ARRAY(DATA, TYPE) { \
		objArray *oa((OObject::Array*)(&DATA), OO_CLASS(TYPE), 0, 0); \
		objArray *oa_ptr = &oa; oa.addRef(); \
		visitObject(#DATA, oa_ptr, OO_CLASS(TYPE), arg, 0, 0); \
	}
*/

// with flags specified.
#define VMETA_F(DATA, TYPE, FLAGS) visitMeta<TYPE>(#DATA, DATA, #TYPE, arg, FLAGS, 0);
#define VMETA_ARRAY_F(DATA, TYPE, FLAGS) visitMetaArray<TYPE>(#DATA, DATA, #TYPE, arg, FLAGS, 0);

#define VOBJ_F(DATA, TYPE, FLAGS) visitObject<TYPE>(#DATA, _convertObjPtr(DATA), OO_CLASS(TYPE), arg, FLAGS, 0);
#define VOBJ_ARRAY_F(DATA, TYPE, FLAGS) visitObjArray<TYPE>(#DATA, DATA, OO_CLASS(TYPE), arg, FLAGS, 0);
#define VOBJ_LIST_F(DATA, TYPE, FLAGS) visitObjList<TYPE>(#DATA, DATA, OO_CLASS(TYPE), arg, FLAGS, 0);

// raw
#define VMETA_R(DATA, TYPE, FLAGS, NAME) visitMeta<TYPE>(NAME, DATA, #TYPE, arg, FLAGS, 0);
#define VMETA_ARRAY_R(DATA, TYPE, FLAGS, NAME) visitMetaArray<TYPE>(NAME, DATA, #TYPE, arg, FLAGS, 0);

#define VOBJ_R(DATA, TYPE, FLAGS, NAME) visitObject<TYPE>(NAME, _convertObjPtr(DATA), OO_CLASS(TYPE), arg, FLAGS, 0);
#define VOBJ_ARRAY_R(DATA, TYPE, FLAGS, NAME) visitObjArray<TYPE>(NAME, DATA, OO_CLASS(TYPE), arg, FLAGS, 0);
#define VOBJ_LIST_R(DATA, TYPE, FLAGS, NAME) visitObjList<TYPE>(NAME, DATA, OO_CLASS(TYPE), arg, FLAGS, 0);

// enum
// template
//#define VENUM(DATA, TYPE) visitMeta(#DATA, DATA, #TYPE, arg, 0, getEnumValue<TYPE>());
#define VENUM(DATA, TYPE) visitMeta<int>(#DATA, (int&)DATA, "int", arg, 0, getEnumValue<TYPE>());
#define VENUM_ARRAY(DATA, TYPE) visitMetaArray<TYPE>(#DATA, DATA, "int", arg, 0, getEnumValue<TYPE>());

#define VENUM_F(DATA, TYPE, FLAGS) visitMeta<int>(#DATA, (int&)DATA, "int", arg, FLAGS, getEnumValue<TYPE>());
#define VENUM_ARRAY_F(DATA, TYPE, FLAGS) visitMetaArray<TYPE>(#DATA, DATA, "int", arg, FLAGS, getEnumValue<TYPE>());

#define VENUM_R(DATA, TYPE, FLAGS, NAME) visitMeta<int>(NAME, (int&)DATA, "int", arg, FLAGS, getEnumValue<TYPE>());
#define VENUM_ARRAY_R(DATA, TYPE, FLAGS, NAME) visitMetaArray<TYPE>(NAME, DATA, "int", arg, FLAGS, getEnumValue<TYPE>());

/*
class OBJ_API objArray : public OObject
{
	OO_DECL_CLASS(objArray, OObject, OClass::flagConcrete, 0)

public:
	OObject::Array *objects;
	OClass *objectClass;
	Dword flags;
	ObjectValue *val;

	objArray() : objects(0), objectClass(0), flags(0), val(0) {}
	objArray(OObject::Array *obj, OClass *cls, Dword f, ObjectValue *v) : objects(obj), objectClass(cls), flags(f), val(v) {}

	template <class T_ARG> 
	void objVisit(T_ARG &arg)
	//void objVisit(ArgWriteXml &arg)
	{
		int num = objects->size();
		visitMeta("Num", num, "int", arg, flags, val);

		for (int i = 0; i < num; ++i) {
			visitObject("Item", _convertObjPtr((*objects)[i]), objectClass, arg, flags, val);
		}
	}

	TEMPL_SPEC
	void objVisit(ArgReadXml &arg)
	{
		ooBreakAssert(objects);

		int num = 0;
		visitMeta("Num", num, "int", arg, flags, val);
		objects->resize(num);

		for (int i = 0; i < num; ++i) {
			visitObject("Item", _convertObjPtr((*objects)[i]), objectClass, arg, flags, val);
		}
	}
};
*/

// no tempalte
//#define VENUM(DATA, TYPE) visitMeta(#DATA, DATA, #TYPE, arg, 0, getEnumValue_##TYPE())

//////////////////////////////////////////////////////////////////////////
// 张乐提出的新版的对象系统声明方法，把导出属性的代码从h移动到cpp，减少了应改动头文件带来的重新编译量。

#define OO_DECL_ABST(CLASS, FLAGS, VER) \
	public: \
	OO_DECL_PTR(CLASS) \
	virtual void _objVisit(ArgWriteXml &arg, bool bSuper = false); \
	virtual void _objVisit(ArgReadXml &arg, bool bSuper = false); \
	virtual void _objVisit(ArgWriteBin &arg, bool bSuper = false); \
	virtual void _objVisit(ArgReadBin &arg, bool bSuper = false); \
	virtual void _objVisit(ArgBase &arg); \
	static OClass _CLASS; \
	enum { flags##CLASS = FLAGS, ver##CLASS = VER}; \
	virtual OClass* getClass() const;\
	static OClass* getBaseClass();\
	template <class T_ARG> void CLASS##_objVisit(T_ARG &arg);

#define OO_DECL_CLS(CLASS, FLAGS, VER) \
	static OObject*  createObject(); \
	virtual void  doRelease();\
	OO_DECL_ABST(CLASS, FLAGS, VER)

#define _IMPL_COMM(CLASS, BASE) \
	OClass* CLASS::getClass() const { return OO_CLASS(CLASS); } \
	OClass* CLASS::getBaseClass() { return OO_CLASS(BASE); } \
	OO_CLASSINIT _init_##CLASS(_OO_CLASS_LOCAL(CLASS), _OO_CLASS_LOCAL(BASE), CLASS::flags##CLASS, CLASS::ver##CLASS);\
	void CLASS::_objVisit(ArgWriteXml &arg, bool bSuper) { \
		ooGuard(#CLASS"_objVisit(ArgWriteXml&)"); \
		BASE::_objVisit(arg, true); \
		Dword savedVer = arg.classVersion; \
		arg.classVersion = ver##CLASS; \
		WRITE_VERSION(CLASS, arg, bSuper); \
		CLASS##_objVisit(arg); \
		arg.classVersion = savedVer; \
	} \
	void CLASS::_objVisit(ArgReadXml &arg, bool bSuper) { \
		ooGuard(#CLASS"_objVisit(ArgReadXml&)"); \
		BASE::_objVisit(arg, true); \
		if ((flags##CLASS & OClass::flagPluginImp) != OClass::flagPluginImp) { \
			Dword savedVer = arg.classVersion; \
			READ_VERSION_XML(CLASS, arg, bSuper); \
			if (ver##CLASS < arg.classVersion) \
				logWarning(LM_Core, "从文件中读取的类版本比本程序更新！Class: %s, CurVer: %d, ReadVer: %d, Path: %s", _OO_CLASS_LOCAL(CLASS)->m_className, ver##CLASS, arg.classVersion, arg.filename.c_str()); \
			CLASS##_objVisit(arg); \
			arg.classVersion = savedVer; \
		} \
	} \
	void CLASS::_objVisit(ArgWriteBin &arg, bool bSuper) { \
		ooGuard(#CLASS"_objVisit(ArgWriteBin&)"); \
		BASE::_objVisit(arg, true); \
		Dword savedVer = arg.classVersion; \
		arg.classVersion = ver##CLASS; \
		WRITE_VERSION(CLASS, arg, bSuper); \
		CLASS##_objVisit(arg); \
		arg.classVersion = savedVer; \
	} \
	void CLASS::_objVisit(ArgReadBin &arg, bool bSuper) { \
		ooGuard(#CLASS"_objVisit(ArgReadBin&)"); \
		BASE::_objVisit(arg, true); \
		if ((flags##CLASS & OClass::flagPluginImp) != OClass::flagPluginImp) { \
			Dword savedVer = arg.classVersion; \
			READ_VERSION_BIN(CLASS, arg, bSuper); \
			if (ver##CLASS < arg.classVersion) \
				logWarning(LM_Core, "从文件中读取的类版本比本程序更新！Class: %s, CurVer: %d, ReadVer: %d, Path: %s", _OO_CLASS_LOCAL(CLASS)->m_className, ver##CLASS, arg.classVersion, arg.filename.c_str()); \
			CLASS##_objVisit(arg); \
			arg.classVersion = savedVer; \
		} \
	} \
	void CLASS::_objVisit(ArgBase &arg) { ooGuard(#CLASS"_objVisit(ArgBase&)"); BASE::_objVisit(arg); CLASS##_objVisit(arg); }\
	template <class T_ARG> void CLASS::CLASS##_objVisit(T_ARG &arg)

#define OO_IMPL_CLS(CLASS, BASE) \
	OObject*  CLASS::createObject() { return objNew(CLASS); } \
	void  CLASS::doRelease() { delete this; } \
	OClass CLASS::_CLASS = { /*name*/const_cast<char*>(#CLASS), /*size*/sizeof(class CLASS), /*flag*/0, /*version*/0, CLASS::createObject, &CLASS::getBaseClass, /*child*/0, /*silbing*/0, /*next*/0, /*master*/0};\
	_IMPL_COMM(CLASS, BASE) \

#define OO_IMPL_ABST(CLASS, BASE) \
	OClass CLASS::_CLASS = { const_cast<char*>(#CLASS), sizeof(class CLASS), 0, 0, 0, &CLASS::getBaseClass, 0, 0, 0 }; \
	_IMPL_COMM(CLASS, BASE)

OO_NAMESPACE_END
