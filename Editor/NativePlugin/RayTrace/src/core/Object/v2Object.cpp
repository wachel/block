// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#include "..\Common\v2ObjStd.h"
#include "..\Utility\v2String.h"
#include "..\Utility\v2StringConverter.h"
#include "v2Object.h"
#include "v2ObjectValue.h"

//#include "v2VFile.h"
#include "VFileSystem.h"

#include "v2BinVisitor.h"
#include "v2XmlVisitorImpl.h"
#include "v2BinVisitorImpl.h"

#pragma warning(disable: 4074)
#pragma init_seg(compiler)

OO_NAMESPACE_BEGIN

// special runtime-class structure for OObject (no base class)
#if OO_DYNAMIC_LINKAGE == 1

OClass*  OObject::getBaseClass()
{ return 0; }

#endif
class OClass OObject::_CLASS;

static OClass* g_classList = 0x0;			// 通过OClass::next连接成的链表。
static OClass* g_classListLast = 0x0;		// 保存最后一项，加快建链速度。

void init_gObj()
{
#if OO_DYNAMIC_LINKAGE == 1
	OClass tmp =
	{ "OObject", sizeof(OObject), /*flags*/OClass::flagAbstract, /*version*/0, /*createaObject*/0, &OObject::getBaseClass, /*child*/0, /*sibling*/0, /*next*/0, /*master*/&OObject::_CLASS };
#else
	OClass tmp =
	{ "OObject", sizeof(OObject), 0, 0, 0, 0 };
#endif

	OObject::_CLASS = tmp;

	g_classList = &OObject::_CLASS;		// 通过OClass::next连接成的链表。
	g_classListLast = g_classList;		// 保存最后一项，加快建链速度。
}

static inline bool _compareUrl(const String &s1, const String &s2)
{
	return uString::stricmp(s1.c_str(), s2.c_str()) == 0;
}

static StringVector _inLoadingObjUrls;
static bool _isInLoading(const String &url)
{
	StringVector::iterator itEnd = _inLoadingObjUrls.end();
	for (StringVector::iterator it = _inLoadingObjUrls.begin(); it != itEnd; ++it) {
		if (_compareUrl(*it, url))
			return true;
	}
	return false;
}

static bool _checkInLoading(const String &url)
{
	if (_isInLoading(url)) {
		logError(LM_Core, "Found circle-reference when loading '%s'", url.c_str());
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

struct BinHeader {
	char mark[3];
	Dword version;

	static const char MARK[];
	void init() {
		memcpy(mark, MARK, sizeof(mark));
		version = 0;
	}
};
const char BinHeader::MARK[] = "B04";
const char B01_MARK[] = "BIN";
const char B02_MARK[] = "B02";
const char B03_MARK[] = "B03";

//////////////////////////////////////////////////////////////////////////
// OAttribute

OAttribute::~OAttribute()
{
	// release object value.
	//safeRelease(objectValue);
}

OAttribute& OAttribute::operator= (const OAttribute &rhs)
{
	/*
	object = rhs.object; // or objectPointer = rhs.objectPointer.
	if (rhs.type == typeWrappedObject) {
		ooBreakAssert(object);
		object->addRef();
	}

	objectClass = rhs.objectClass;
	name = rhs.name;
	type = rhs.type;
	*/
	// @todo
	ooBreakAssert(false);

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// OObject

void* OObject::onReadObjectsCallbackArg = 0;
FUN_onReadObjectsNum OObject::onReadObjectsNumCallback = 0;
FUN_onReadObjectsItem OObject::onReadObjectsItemCallback = 0;

bool OObject::bDevMode = false;
bool OObject::bClientMode = false;

OBJ_API ObjectList& getRefObjects() {
	static ObjectList g_objects;
	return g_objects;
}

#define USE_OBJMAP

#ifdef USE_OBJMAP

//#ifdef WIN32
//typedef HashMap<String, OObject*, StringHash> ObjMap;
//#else
//typedef HashMap<String, OObject*> ObjMap;
//#endif
typedef std::map<String,OObject*>ObjMap;

OBJ_API ObjMap& getRefObjMap() {
	static ObjMap g_objects;
	return g_objects;
}
#endif 

OBJ_API void addRefObj(OObject *obj)
{
	getRefObjects().push_back(obj);

#ifdef USE_OBJMAP
	ooBreakAssert(getRefObjMap().find(obj->getUrl()) == getRefObjMap().end());
	getRefObjMap()[obj->getUrl()] = obj;
#endif
}

OBJ_API void removeRefObj(OObject *obj)
{
#ifdef USE_OBJMAP
	{
		ObjMap::iterator it = getRefObjMap().find(obj->getUrl());
		ooBreakAssert(it != getRefObjMap().end());

		getRefObjMap().erase(it);
	}
#endif

	ObjectList::iterator it = std::find(getRefObjects().begin(), getRefObjects().end(), obj);
	ooBreakAssert(it != getRefObjects().end());

	getRefObjects().erase(it);

}

OBJ_API void setObjUrl(OObject *obj, const String& url)
{
	ooBreakAssert(obj);
	//ooBreakAssert(url.find('\\') == String::npos);

	if (!obj->getUrl().empty())
		removeRefObj(obj);

    obj->setUrl(url);
	if (!url.empty())
        addRefObj(obj);
}

int OObject::_numObj = 0;
OObject* OObject::_objListHead = 0;
OObject* OObject::_objListTail = 0;

// 是否调试对象系统，调试时 grap_dump_used_tex, dump_obj_cnt 这些命令才管用
// 为了节省编译时间没有把头文件里 _nextObj, _prevObj 包含进去。
// 12/3/15,打开时编辑里退出时Debug版会有错误，可能动态库析构顺序问题。(找到原因，是因为一个plugin没有重新编译的问题，-wb, 2012/7/31）
#define DEBUG_OBJ_LIST

OObject::OObject()
{
	++_numObj;

#ifdef DEBUG_OBJ_LIST
	_linkObj();
#endif
}

OObject::~OObject()
{
	--_numObj;
	if (!m_url.empty())
		removeRefObj(this);

#ifdef DEBUG_OBJ_LIST
	_unlinkObj();
#endif

}

OObject::OObject(const OObject &)
{
	++_numObj;

#ifdef DEBUG_OBJ_SYS
	_linkObj();
#endif
}

OObject& OObject::operator=(const OObject &)
{
	//_linkObj();
	return *this;
}

void OObject::_linkObj()
{
	if (_objListTail) {
		ooBreakAssert(!_objListTail->_nextObj);
		this->_prevObj = _objListTail;
		this->_nextObj = 0;
		_objListTail->_nextObj = this;
		_objListTail = this;
	}
	else {
		ooBreakAssert(!_objListHead);
		_objListHead = _objListTail = this;
		this->_nextObj = this->_prevObj = 0;
	}
}

void OObject::_unlinkObj()
{
	ooBreakAssert(!_objListTail || !_objListTail->_nextObj);
	ooBreakAssert(!_nextObj || _nextObj->_prevObj == this);

	ooBreakAssert(!_objListTail || !_objListTail->_nextObj);

 	ooBreakAssert(_objListTail && _objListHead);
 	if (this->_prevObj)
 		this->_prevObj->_nextObj = this->_nextObj;
 	if (this->_nextObj)
 		this->_nextObj->_prevObj = this->_prevObj;
 	if (this == _objListHead)
 		_objListHead = this->_nextObj;
 	if (this == _objListTail)
 		_objListTail = this->_prevObj;
 
	ooBreakAssert(!_objListTail || !_objListTail->_nextObj);

 	_nextObj = _prevObj = 0;
}

//////////////////////////////////////////////////////////////////////////
// Memory manage.

#if OO_MEMORY_TRACE	

void* OObject::operator new(size_t size, const char *sourceFile, int sourceLine) 
{
	return op_new_sc(size, sourceFile, sourceLine);
}
void OObject::operator delete(void* ptr, const char *sourceFile, int sourceLine) 
{
	op_delete_sc(ptr);
}

void* OObject::operator new(size_t size) 
{
	//ooAssertDesc(false, "Use 'objNew' instead.");
	return op_new_sc(size, "[N/A]", 0);
}
void OObject::operator delete(void* ptr) 
{
	op_delete_sc(ptr);
}

/*
void* OObject::operator new(size_t, void* p)
{
	return p;
}
void OObject::operator delete(size_t, void* p)
{
}
*/

void* OObject::operator new[](size_t size, const char *sourceFile, int sourceLine) 
{
	return op_new_vc(size, sourceFile, sourceLine);
}
void OObject::operator delete[](void* ptr, const char *sourceFile, int sourceLine) 
{
	op_delete_vc(ptr);
}

void* OObject::operator new[](size_t size) 
{
	ooAssertDesc(false, "Use 'objNewA' instead.");
	return op_new_vc(size, "[N/A]", 0);
}
void OObject::operator delete[](void* ptr) 
{
	op_delete_vc(ptr);
}

#endif // #if OO_MEMORY_TRACE	

//////////////////////////////////////////////////////////////////////////

OObject* OObject::findByUrl(const String &url)
{
#ifndef USE_OBJMAP
	ObjectList *objs = &getRefObjects();
	for (ObjectList::iterator it = objs->begin(); it != objs->end(); ++it) {
		OObject *obj = *it;
		if (_compareUrl(obj->getUrl(), url))
			return *it;
	}
	return 0;

#else

	ObjMap::iterator it = getRefObjMap().find(url);
	if (it != getRefObjMap().end())
		return it->second;
	else
		return 0;
#endif
}

int OObject::findByClass(ObjectList &lst, OClass *cls, bool isEndClass)
{
	//OO_UNIMPLEMENTED_METHOD

	lst.clear();

	ObjectList *objs = &getRefObjects();
	for (ObjectList::iterator it = objs->begin(); it != objs->end(); ++it) {
		OObject *obj = *it;
		if ((isEndClass && obj->getClass() == cls) || obj->isA(cls)) {
			lst.push_back(obj);
		}
	}

	return lst.size();
}

bool OObject::hasObject(OObject *obj, bool recursive)
{
	ooBreakAssert(obj);

	AttriVisitor vis;
	
	_objVisit(vis);

	for (OAttribute::List::iterator it = vis.attributes.begin(); it != vis.attributes.end(); ++it) {
		OAttribute *curAttri = *it;

        OObject *curObj = curAttri->getObject();
		if (curObj == obj) {
			//if (attri)
			//	*attri = *curAttri; 
			return true;
		}

		if (recursive && curObj) {
			if (curObj->hasObject(obj, recursive))
				return true;
		}
	}

	return false;
}

OClass* OObject::getClass() const
{
	return OO_CLASS(OObject);
}

bool OObject::isA(const OClass* pClass) const
{
	ooBreakAssert(this != 0);
	// it better be in valid memory, at least for OObject size
	//ooBreakAssert(ooIsValidAddress(this, sizeof(OObject)));

	// simple SI case
	OClass* pClassThis = getClass();
	return pClassThis->isDerivedFrom(pClass);
}

bool OObject::saveToFile(VFileSystemBase* fileSystem, const String &path, FileType type /* = FT_Xml */, Dword fileVersion /* = 0 */)
{
	if (type == FT_Xml)
		return saveToXml(fileSystem,path, fileVersion);
	else
		return saveToBin(fileSystem,path, fileVersion);
}

// vpath必须是标准化后的，就是checkVPath后的。
static OObject* _doLoadFromXml(VFileSystemBase* fileSystem,const String& url, Dword *fileVersion)
{
	ooGuard("_doLoadFromXml");

	if(!fileSystem || !fileSystem->testFile(url)){
		return 0;
	}

	// check for circle-reference.
	if (!_checkInLoading(url))
		return 0;
	_inLoadingObjUrls.push_back(url);
#ifdef _DEBUG
	int stackSize = _inLoadingObjUrls.size();
#endif

	OObject *obj = 0;
	try {
		TiXmlDocument docLoad;
		VFileBase::Ptr vfile = fileSystem->openFile(url);
		docLoad.Parse((const char*)vfile->readAllData()->buffer);
		vfile->close();

		//VFileSystem *vfs = VFileSystem::instancePtr();
		//if (vfs) {
  //          VFileHandle h = vfs->fileOpen(vpath);
		//	if (h.null()) {
		//		//ooThrow(EFileNotFound(url));
		//	}
		//	docLoad.Parse((const char*)vfs->fileGetBuffer(h));
		//}
		//else { // without vfs, load from disk directly. useful when vfs is not initialized.
		//	docLoad.LoadFile(vpath.c_str());
		//}

		ArgReadXml arg;
		arg.type = AT_ReadXml;
		arg.node = &docLoad;
		arg.filename = url;
		arg.fileSystem = fileSystem;

		// read in xml format.
		//Dword format = XF_X01; // default;
		// 下面一行修复iOS Release版发现的错误，arg.format未指定值。如果arg.format碰巧为0(XF_X01)，visitMeta会错误的读取"XmlFormat"。 -wb, 2013/9/7
		arg.format = XF_X03; // 任意XF_X01后的版本均可。
		//visitMeta("XmlFormat", format, "Dword", arg, 0, 0);
		//arg.format = (XmlFormat)format;
		//{
		//	TiXmlElement *elem = arg.node->FirstChildElement("XmlFormat");
		//	if (elem) {
		//		// remove version elem.
		//		arg.node->RemoveChild(elem);
		//	}
		//}

		// read in file version.
		//visitMeta("Version", arg.fileVersion, "Dword", arg, 0, 0);
		//if (fileVersion) *fileVersion = arg.fileVersion;

		// version update. should be removed furture.
		//TiXmlElement *elem = arg.node->FirstChildElement("Version");
		//if (elem) {
		//	// remove version elem.
		//	arg.node->RemoveChild(elem);
		//}

		//Real startTime = Utility::GetTime();
		// read in class.
		visitObject(0, obj, 0, arg, 0, 0);

		//logDebug("_doLoadFromXml cost time %f seconds", Utility::GetTime() - startTime);
		if (obj) {
			// visitObject will increase the ref count of the object.
			obj->decRef();

			// @fixme "FileSystem::checkUrl(url)" deal with old url, replace "\" with "/".

			setObjUrl(obj, url);
		}

		if (obj){
			//logInfo(LM_Core, "Object loaded from '%s', Ref=%d", vpath.c_str(), obj->getRef());
		}
	}
	catch (const String &str) { // "Could not find class..."
		obj = 0;
		logError(LM_Core, "从文件中装载对象失败！path: %s，错误信息：%s", url.c_str(), str.c_str());
	}
	//catch (OError &e) {
	//	obj = 0;
	//	logError(LM_Core, "从文件中装载对象失败！path: " + vpath + "\n" + e.getFullDesc());
	//}
    catch(...) {
		obj = 0;
		//logError(LM_Core, "从文件中装载对象失败！path: %s", vpath.c_str());
	}

#ifdef _DEBUG
	ooAssert(stackSize = _inLoadingObjUrls.size());
#endif
	_inLoadingObjUrls.pop_back();

	return obj;
}

OObject* OObject::loadFromXml(VFileSystemBase* fileSystem, const String &vpathArg, Dword *fileVersion)
{
	//String vpath = VFileSystem::checkVPath(vpathArg);
	//String url = VFileSystem::vpathToUrl(vpath);
	String url = vpathArg;

	// first try to find loaded object.
	OObject *obj = findByUrl(url);
	if (!obj) {
		obj = _doLoadFromXml(fileSystem,url, fileVersion);
	}

	return obj;
}

//String _print(TiXmlDocument &doc, const String &lineBreak)
//{
//	const char tempfile[] = "_temp.xml";
//
//	FILE *file = fopen(tempfile, "w");
//	doc.Print(file, 0);
//	fclose(file);
//
//	file = fopen(tempfile, "r");
//	String read;
//	char c;
//	while(fread(&c, 1, 1, file) == 1) {
//		read += c;
//	}
//	fclose(file);
//
//	//String r2 = read;
//	read = uString::replace(read, "\n", lineBreak);
//
//	// deletes
//	unlink(tempfile);
//
//	return read;
//}

bool OObject::saveToXml(VFileSystemBase* fileSystem, const String &url, Dword fileVersion)
{
	if (url.empty())
		return false;

	// avoid self-calling. obj->saveToFile(obj->getUrl()). save a local string.
	String tempUrl = url;

	String savedUrl = getUrl();
	setUrl(""); // make XmlArchive do not just save a url.

	bool ret = true;
	try {
		TiXmlDocument docSave;

		ArgWriteXml arg;
		arg.type = AT_WriteXml;
		arg.node = &docSave;
		arg.filename = url;
		arg.fileVersion = fileVersion;

		//// write out xml format.
		Dword format = XF_Last;
		//visitMeta("XmlFormat", format, "Dword", arg, 0, 0);

		//// write out header.
		//visitMeta("Version", fileVersion, "Dword", arg, 0, 0);

		//objWrite->objVisit(arg);
		OObject *obj = this;
		visitObject("Object", obj, getClass(), arg, 0, 0);

		TiXmlOutStream tempStr;
		tempStr<<docSave;
		
		String outString = uString::format("<!--XmlFormat=%d-->",format);
		outString += uString::format("<!--Version=%d-->",fileVersion);
		outString += tempStr.c_str();

		VFileBase::Ptr vfile = fileSystem->openFile(url,"wb");
		vfile->write(outString.c_str(),outString.length());
		vfile->close();
		
		//	String abs = Utility::GetAbsolutePath(tempUrl, Engine::getCaps().modulePath
		//bool ret = ar.save(tempUrl);

		/*
		String xml = _print(docSave, "\r\n");

		// save xml string to file.
		File file;
		file.open(tempUrl, File::modeWrite);

		file.write(xml.c_str(), xml.size());
		file.close();
		*/
//		String writableDir;
//		// @FIXME need getWriteableFile
//#ifdef WIN32
//		writableDir = url + "/..";//Utility::GetModulePath();
//#else
//		writableDir = url + "/../";//PathUtility::removeName(url) + OS_PATH_SPLIT_STR;
//#endif
//		String tmp_file = writableDir + "_obj_temp_XXXXXX";
//
//		//old: String tmp_file = Utility::GetModulePath() + "_obj_temp_XXXXXX";
//#ifdef WIN32
//		tmp_file = _mktemp(const_cast<char*>(tmp_file.c_str()));
//#else
//		tmp_file = mktemp(const_cast<char*>(tmp_file.c_str()));
//#endif
//		if (docSave.SaveFile(tmp_file.c_str())) {
//			String abs = VFileSystem::urlToAbsPath(tempUrl);
//			unlink(abs.c_str());
//			ret = rename(tmp_file.c_str(), abs.c_str()) == 0;
//			if (!ret) {
//				logError(LM_Core, "保存文件失败（只读？）, path:" + abs);
//				unlink(tmp_file.c_str());
//			}
//		}
//		else
//			ret = false;
//
	}
	catch(...) {
		ret = false;
	}

	// restore url.
	setUrl(savedUrl);

	return ret;
}

static OObject* _doLoadFromBin(VFileSystemBase* fileSystem,const String& url, Dword *fileVersion)
{
	ooGuard("_doLoadFromBin");

	if(!fileSystem->testFile(url)){
		return 0;
	}

	// check for circle-reference.
	if (!_checkInLoading(url))
		return 0;
	_inLoadingObjUrls.push_back(url);
#ifdef _DEBUG
	int stackSize = _inLoadingObjUrls.size();
#endif

	OObject *obj = 0;
	try {
		//VFileSystem *vfs = VFileSystem::instancePtr();
		//VFileHandle h = vfs->fileOpen(vpath);
		//if (h.null()) {
			//ooThrow(EFileNotFound(vpath));
		//}

		using namespace std;
		// @FIXME strstream is depreciated.
		// More Effective STL: Item 35
		// * 支持I/O操作。流运行库仍然有部分保留在C++标准中，但标准委员会作了小量的修补。虽然部分类被去除了（特别是iostram和fstram），部分类被替换了（例如，以string为基础的stringstream类替换了以char*为基础的strstream类，现在已不提倡使用 strstream类了），但标准流类的基本功能含概了以前的实现的基本功能。
		// 

		VFileBase::Ptr vfile = fileSystem->openFile(url);
		VFileData::Ptr fileData = vfile->readAllData();
		istrstream file((const char*)fileData->buffer, fileData->size);
		vfile->close();
		ArgReadBin arg;
		arg.type = AT_ReadBin;
		arg.file = &file;
		arg.filename = url;
		arg.classVersion = 0xFFFFFFFF;
		arg.fileSystem = fileSystem;

		// read in header.
		BinHeader bh;
		file.read((char *)&bh, sizeof(bh));

		if (strncmp(bh.mark, BinHeader::MARK, sizeof(bh.mark)) == 0) {
			arg.fileVersion = bh.version;
			arg.format = BF_B04;
		}
		else if (strncmp(bh.mark, B03_MARK, sizeof(bh.mark)) == 0) {
			arg.fileVersion = bh.version;
			arg.format = BF_B03;
		}
		else if (strncmp(bh.mark, B02_MARK, sizeof(bh.mark)) == 0) {
			arg.fileVersion = bh.version;
			arg.format = BF_B02;
		}
		else if(strncmp(bh.mark, B01_MARK, sizeof(bh.mark)) == 0) {
			// old file.
			arg.fileVersion = bh.version;
			arg.format = BF_B01;
		}
		else {
			// old file.
			file.seekg(0); // move read point to begin.
			arg.fileVersion = 0;
			arg.format = BF_B01;
		}

		if (fileVersion) *fileVersion = arg.fileVersion;

		// read in class.
		visitObject(0, obj, 0, arg, 0, 0);

		if (obj) {
			// visitObject will increase the ref count of the object.
			obj->decRef();

			// @fixme "FileSystem::checkUrl(url)" deal with old url, replace "\" with "/".

			setObjUrl(obj, url);
		}

		if (obj)
			logInfo(LM_Core, "Object loaded from '%s', Ref=%d", url.c_str(), obj->getRef());
	}
	//catch (EClassNotFound &e) {
	//	obj = 0;
	//	String desc = e.getFullDesc();
	//	logError(LM_Core, "从文件中装载对象失败！path: %s. 错误信息: %s", vpath.c_str(), desc.c_str());
	//}
	catch(...) {
		obj = 0;
		logError(LM_Core, "从文件中装载对象失败！path: %s", url.c_str());
	}

#ifdef _DEBUG
	ooAssert(stackSize = _inLoadingObjUrls.size());
#endif
	_inLoadingObjUrls.pop_back();

	return obj;
}

OObject* OObject::loadFromBin(VFileSystemBase* fileSystem, const String &vpathArg, Dword *fileVersion)
{
	//String vpath = VFileSystem::checkVPath(vpathArg);
	//String url = VFileSystem::vpathToUrl(vpath);
	String url = vpathArg;

	// first try to find loaded object.
	OObject *obj = findByUrl(url);
	if (!obj) {
		// not find, load from the file.
		obj = _doLoadFromBin(fileSystem,url, fileVersion);
	}

	return obj;
}

bool OObject::saveToBin(VFileSystemBase* fileSystem, const String &url, Dword fileVersion)
{
	if (url.empty())
		return false;

	//String abs = VFileSystem::urlToAbsPath(url);

	// avoid self-calling. obj->saveToFile(obj->getUrl()). save a local string.
	//String tempUrl = abs;

	String savedUrl = getUrl();
	setUrl(""); // make XmlArchive do not just save a url.

//	bool ret = true;
//	String tmp_file = "_obj_temp_XXXXXX";
//#ifdef WIN32
//	tmp_file = _mktemp(const_cast<char*>(tmp_file.c_str()));
//#else
//	tmp_file = mktemp(const_cast<char*>(tmp_file.c_str()));
//#endif
//	try {
		using namespace std;
		stringstream file;
		//  [1/18/2010 Archer]
		{
			//SetupLocal	setuplocal("");
			//file.open(tmp_file.c_str(), ios::binary|ios::out);
		}
		
		//if (!file.is_open())
		//	ooThrow(OError("Could not open file for write. path" + tmp_file));

		ArgWriteBin arg;
		arg.type = AT_WriteBin;
        arg.file = &file;
		arg.filename = url;
		arg.fileVersion = fileVersion;
		arg.fileSystem = fileSystem;

		// write out header.
		BinHeader bh;
		bh.init();
		bh.version = fileVersion;
		file.write((const char*)&bh, sizeof(bh));

		//objWrite->objVisit(arg);
		OObject *obj = this;
		visitObject(getClass()->m_className, obj, getClass(), arg, 0, 0);

		VFileBase::Ptr vfile = fileSystem->openFile(url,"wb");
		vfile->write(file.str().c_str(),file.str().size());
		vfile->close();
	//	//file.close();

	//	// rename the file.
	//	unlink(abs.c_str());
	//	ret = rename(tmp_file.c_str(), abs.c_str()) == 0;
	//	if (!ret) {
	//		logError(LM_Core, "保存文件失败（只读？）, path" + abs);
	//		unlink(tmp_file.c_str());
	//	}
	//}
	//catch(...) {
	//	unlink(tmp_file.c_str());

	//	ret = false;
	//}

	// restore url.
	setUrl(savedUrl);

	return true;
}

bool OObject::saveUrlObjects(VFileSystemBase* fileSystem, bool recursive, FileType type, Dword fileVersion)
{
	AttriVisitor vis;
	_objVisit(vis);
	
	bool hasError = false;

	for (OAttribute::List::iterator it = vis.attributes.begin(); it != vis.attributes.end(); ++it) {
		OAttribute *attri = *it;
		OObject *obj = attri->getObject();
		if (obj) {
			if (!obj->getUrl().empty()) {
				// found a url object, save it.
				if (!obj->saveToFile(fileSystem,obj->getUrl(), type, fileVersion))
					hasError = true;
			}

			// save url objects in this object.
			if (recursive) {
				if (!obj->saveUrlObjects(fileSystem,recursive, type, fileVersion))
					hasError = true;
			}
		}
	}

	return !hasError;
}


static bool _isBinFileValid(const String &url)
{
	return false;
	//VFileSystem *fs = VFileSystem::instancePtr();

	//String binUrl = url + ".bin";
	//time_t tmBin, tmXml;

	//if (!fs->fileCreateTime(binUrl, tmBin))
	//	return false;

	//if (!fs->fileCreateTime(url, tmXml))
	//	return false;

	//return tmBin >= tmXml;
}

OObject* OObject::loadFromFile(VFileSystemBase* fileSystem, const String &vpathArg, Dword *fileVersion)
{
	ooGuard("OObject::loadFromFile");

	if (vpathArg.empty())
		return 0;

	//String vpath = VFileSystem::checkVPath(vpathArg); // vpath to url
	//String url = VFileSystem::vpathToUrl(vpath);
	String url = vpathArg;

	LoadingErrorCode errorCode = LEC_None;
	String errorDesc;

	// first try to find loaded object.
	OObject *obj = findByUrl(url);
	if (!obj) {
		try {
			if (_isBinFileValid(url)) {
				String binUrl = url + ".bin";
				obj = _doLoadFromBin(fileSystem,binUrl, fileVersion);
				if (obj)
					obj->setUrl(url);
			}
			else {
				//VFileSystem *vfs = VFileSystem::instancePtr();
				//char c = 0;
				//if (vfs) {
				//	VFileHandle h = vfs->fileOpen(vpath);
				//	if (h.null()) {
				//		ooThrow(EFileNotFound(vpath));
				//	}

				//	vfs->fileRead(h, &c, sizeof(c));
				//	vfs->fileClose(h);
				//}
				//else { // VFS is not initialized.
				//	using namespace std;
				//	ifstream file;
				//	//locale loc = locale::global(locale(""));//将全局区域设为操作系统默认区域
				//	//SetupLocal	setuplocal("");
				//	file.open(url.c_str(), ios::binary|ios::in);
				//	//std::locale::global(loc);
				//	//if (!file.is_open())
				//	//	ooThrow(EFileNotFound(url));
				//	//
				//	file >> c;
				//	file.close();
				//}
				
				/*
				File file;
				file.open(url);

				char c = 0;
				file.read(&c, sizeof(c));
				file.close();
				*/

				char c = 0;
				VFileBase::Ptr vfile = fileSystem->openFile(url);
				if(!vfile){
					return NULL;
				}
				vfile->readByPos(( char*) &c,0,1);
				vfile->close();

				if (c == '<')
					obj = _doLoadFromXml(fileSystem,url, fileVersion);
				else
					obj = _doLoadFromBin(fileSystem,url, fileVersion);
			}
		}
		//catch (EFileNotFound &e) {
  //          logError(LM_Core, "[OObject::loadFromFile]从文件中装载对象失败！Path:" + url + "\n" + e.getFullDesc());

		//	errorCode = LEC_FillNotFound;
		//	errorDesc = e.path;
		//}
		//catch (OError &e) {
		//	logError(LM_Core, "[OObject::loadFromFile]从文件中装载对象失败！Path:" + url + "\n" + e.getFullDesc());

		//	errorCode = LEC_Error;
		//	errorDesc = vpath + "," + e.getFullDesc();
		//}
		catch (...) {
			logError(LM_Core, ("[OObject::loadFromFile]未知错误！，Path:" + url).c_str());

			errorCode = LEC_Unknown;
			errorDesc = url;
		}

		if (errorCode != LEC_None) {
			addLoadingError(errorCode, errorDesc);
		}
	}

	return obj;
}

OObject* OObject::createObject(const String &clsName)
{
	ooGuard("OObject::createObject");

	OClass *cls = OClass::fromName(clsName.c_str());
	if (cls) {
		OObject *obj = cls->createObject();
		return obj;
	}
	else {
		return 0;
	}
}

OObject* OObject::importObject(VFileSystemBase* fileSystem, const String &url)
{
	ooGuard("OObject::importObject");
	Dword fileVersion = 0;
	OObject::Ptr src = OObject::loadFromFile(fileSystem,url, &fileVersion);
	if (src)
		return OObject::cloneObject(src, fileVersion);
	else
		return 0;

}

// fileVersion 没有保存在 OObject 里，所以必须传入要克隆对象的 fileVersion
OObject* OObject::cloneObject(OObject *obj, Dword fileVersion)
{
	//ooGuard("OObject::cloneObject");
	//if (!obj)
	//	return 0;

	//String tmp_file = "";// PathUtility::getModulePath() + "/_cloned_object.bin";

	//std::stringstream file;
	////std::fstream file;
	////using namespace std;
	//{
	//	//file.open(tmp_file.c_str(), ios::binary|ios::out|ios::in| ios_base::trunc);
	//	//if (!file.is_open())
	//	//	ooThrow(OError("Could not open file for write. path" + tmp_file));

	//	ArgWriteBin arg;
	//	arg.type = AT_WriteBin;
	//	arg.file = &file;
	//	arg.filename = tmp_file;
	//	arg.fileVersion = fileVersion;

	//	String savedUrl = obj->getUrl();
	//	obj->setUrl(""); // make XmlArchive do not just save a url.

	//	visitObject(obj->getClass()->m_className, obj, obj->getClass(), arg, 0, 0);

	//	obj->setUrl(savedUrl);
	//	//file.close();
	//}

	//{
	//	file.seekg(0); // move read point to begin.

	//	ArgReadBin arg;
	//	arg.type = AT_ReadBin;
	//	arg.file = &file;
	//	arg.filename = tmp_file;
	//	arg.fileVersion = fileVersion;
	//	arg.classVersion = 0xFFFFFFFF;
	//	arg.format = BF_B04;

	//	// read in class.
	//	OObject *newObj = 0;
	//	visitObject(0, newObj, 0, arg, 0, 0);

	//	//file.close();

	//	if (!newObj)
	//		return 0;

	//	//// visitObject will increase the ref count of the object.
	//	newObj->decRef();

	//	return newObj;
	//}
	return cloneObject(obj,0,fileVersion);
}

// fileVersion 没有保存在 OObject 里，所以必须传入要克隆对象的 fileVersion
OObject* OObject::cloneObject(OObject *srcObj,OObject *dstObj, Dword fileVersion)
{
	ooGuard("OObject::cloneObject");
	if (!srcObj)
		return 0;

	String tmp_file = "";// PathUtility::getModulePath() + "/_cloned_object.bin";

	std::stringstream file;
	//std::fstream file;
	//using namespace std;
	{
		//file.open(tmp_file.c_str(), ios::binary|ios::out|ios::in| ios_base::trunc);
		//if (!file.is_open())
		//	ooThrow(OError("Could not open file for write. path" + tmp_file));

		ArgWriteBin arg;
		arg.type = AT_WriteBin;
		arg.file = &file;
		arg.filename = tmp_file;
		arg.fileVersion = fileVersion;

		String savedUrl = srcObj->getUrl();
		srcObj->setUrl(""); // make XmlArchive do not just save a url.

		visitObject(srcObj->getClass()->m_className, srcObj, srcObj->getClass(), arg, 0, 0);

		srcObj->setUrl(savedUrl);
		//file.close();
	}

	{
		file.seekg(0); // move read point to begin.

		ArgReadBin arg;
		arg.type = AT_ReadBin;
		arg.file = &file;
		arg.filename = tmp_file;
		arg.fileVersion = fileVersion;
		arg.classVersion = 0xFFFFFFFF;
		arg.format = BF_Last;

		// read in class.
		OObject *newObj = dstObj;
		visitObject(0, newObj, 0, arg, 0, 0);

		//file.close();

		if (!newObj)
			return 0;

		//// visitObject will increase the ref count of the object.
		newObj->decRef();

		return newObj;
	}
}

// 尚未完成，可能需要加一个 class ArgCopy: public OArgBase 
//bool shallowCopy(OObject *objSrc, OObject *objDst) 
//{
//	if (OO_CLASS(objSrc) != OO_CLASS(objDst))
//		return false;
//
//	AttriVisitor visSrc;
//	objSrc->_objVisit(visSrc);
//	OAttribute::List &lstSrc = visSrc.attributes;
//
//	AttriVisitor visDst;
//	objDst->_objVisit(visDst);
//	OAttribute::List &lstDst = visDst.attributes;
//	ooAssert(lstSrc.size() == lstDst.size());
//
//	OAttribute::List::iterator itSrc, itDst, itSrcEnd, itDstEnd;
//	itSrc = lstSrc.begin(); 
//	itSrcEnd = lstSrc.end();
//	itDst = lstDst.begin(); 
//	itDstEnd = lstDst.end();
//	for (; itSrc != itSrcEnd; ++itSrc, ++itDst) {
//		OAttribute *attrSrc = *itSrc;
//		OAttribute *attrDst = *itDst;
//
//
//	}
//
//	return true;
//}
//

static void* onLoadingErrorArg = 0;
static OObject::FUN_onLoadingError onLoadingErrorCallback = 0;
void OObject::registerLoadingErrorCallback(FUN_onLoadingError fun, void *userArg)
{
	onLoadingErrorCallback = fun;
	onLoadingErrorArg = userArg;
}

void OObject::addLoadingError(LoadingErrorCode errorCode, const String& errorDesc) 
{
	if (onLoadingErrorCallback) {
		(*onLoadingErrorCallback)(onLoadingErrorArg, errorCode, errorDesc);
	}
}

//////////////////////////////////////////////////////////////////////////
// OClass

bool OClass::isDerivedFrom(const OClass* pBaseClass) const
{
	ooBreakAssert(this != 0);
	//ooBreakAssert(ooIsValidAddress(this, sizeof(OClass), false));
	
	if (!pBaseClass)
		return false;

	const OClass* masterBase = pBaseClass->master;
	const OClass* masterThis = master;

	// simple SI case
	//const OClass* pClassThis = this;
	while (masterThis != 0)
	{
		if (masterThis == masterBase)
			return true;
		masterThis = (*masterThis->m_funGetBaseClass)();
		ooBreakAssert(!masterThis || masterThis->isMaster()); // master肯定是和master相连的。
	}
	return false;       // walked to the top, no match
}

int OClass::findSubs(List &lst, Dword flags, bool recursive)
{
	int num = 0; // save num for speed.

	OClass *sub = child;
	while (sub) {
		// insert into list.
		if (sub->flags & flags) {
			lst.push_back(sub);
			++num;
		}

		// insert children into list.
		if (recursive)
			num += sub->findSubs(lst, flags, recursive);

		// move to next.
		sub = sub->sibling;
	}

	return num;
}

static std::map<String, String> s_classAliasMap;
OClass* OClass::fromName(const char* className, bool bIncludeAlias)
{
	if (bIncludeAlias) {
		std::map<String, String>::iterator it = s_classAliasMap.find(className);
		if (it != s_classAliasMap.end())
			className = it->second.c_str();
	}


	// search app specific classes
	for (OClass *pClass = g_classList; pClass != 0;
		pClass = pClass->next)
	{
		if (strcmp(className, pClass->m_className) == 0)
		{
			return pClass;
		}
	}

	return 0;
}

bool OClass::registerAlias(const char* className, const char* classAlias)
{
	s_classAliasMap[classAlias] = className;
	return true;
}

OObject* OClass::createObject()
{
	// if it is a plugin, create an implemention of it.
	OObject *obj;
	if (flags & OClass::flagPluginInterface) {
		OClass *cls = child;
		while (cls && !(cls->flags & OClass::flagPluginImp))
			cls = cls->sibling;
		
		if (!cls)
			ooThrow(OError(uString::format("No plugin imp found for %s", m_className)));

		obj = cls->createObject();
		return obj;
	}
	else {
		if (m_funCreateObject == 0)
		{
			String str = uString::format("Trying to create object which is not DECLARE_DYNCREATE or DECLARE_SERIAL: %s.", m_className);
			ooThrow(OError(str));
			return 0;
		}

		OObject* pObject = 0;

		pObject = (*m_funCreateObject)();

		return pObject;
	}
}

bool OClass::isHidden() const
{
	return hasFlags(flagHide) || (!OObject::bDevMode && hasFlags(flagDev));
}

int OClass::calcSlaveNum() const 
{
	int numSlaves = 0;

	OClass *m = master;
	OClass *c = g_classList;
	while (c) {
		if (c->master == m)
			numSlaves++;
		c = c->next;
	}

	return numSlaves;
}

OObject* ooDynamicDownCast(OClass* pClass, OObject* pObject)
{
	if (pObject != 0 && pObject->isA(pClass))
		return pObject;
	else
		return 0;
}

void OBJ_API ooClassInit(OClass* newClass, OClass *baseClass, Dword flags, Dword ver)
{
	ooBreakAssert(newClass);

	//AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	//AfxLockGlobals(CRIT_RUNTIMECLASSLIST);
	//pModuleState->m_classList.AddHead(newClass);
	//AfxUnlockGlobals(CRIT_RUNTIMECLASSLIST);

	// initialize.

	newClass->flags = flags;
	newClass->version = ver;
	
	// @bug 4. *OO_CLASSINIT的构造顺序无法保证。有可能child,sibling有值了。
	/* 
	newClass->child = 0;
	newClass->sibling = 0;
	*/

	//// lazy initialization.
	if (!g_classList) {
		init_gObj();
	}

	// 查找是否有同名类，第一个同名类为master。
	OClass *master = g_classList;
	while (master) {
		if (strcmp(master->m_className, newClass->m_className) == 0)
			break;
		master = master->next;
	}

	if (!master) {
		newClass->master = newClass;
		
		// add to children list.
		if (baseClass) {
			if (!baseClass->child) {
				baseClass->child = newClass;
			}
			else {
				OClass *c = baseClass->child;
				while (c && c->sibling)
					c = c->sibling;
				c->sibling = newClass;
			}
		}
	}
	else {
		logNotice(LM_Core, "注册类到引擎类库时发现该类已经注册过！如果该类在Lib中这种情况则是正常，类名：%s", master->m_className);

		ooBreakAssert(!newClass->child && !newClass->sibling);

		// 把孩子给master管理。
		if (!master->child)
			master->child = newClass->child;
		else {
			OClass *c = master->child;
			while (c && c->sibling)
				c = c->sibling;

			c->sibling = newClass->child;
		}
		newClass->child = 0;

		// 把兄弟交master管理。
		if (!master->sibling)
			master->sibling = newClass->sibling;
		else {
			OClass *c = master->sibling;
			while (c && c->sibling)
				c = c->sibling;

			c->sibling = newClass->child;
		}
		newClass->sibling = 0;

		// 把自己交给master管理。
		newClass->master = master;
	}

	// 不管是不是master都加入列表（目前其实加不加slave都可以，为以后方便管理先全加入）。
	g_classListLast->next = newClass;
	g_classListLast = newClass;
	
}

//////////////////////////////////////////////////////////////////////////
// OAttribute

void OAttribute::doRelease()
{ 
	delete this;
}

//////////////////////////////////////////////////////////////////////////
// AttriVisitor

void AttriVisitor::visitMeta(const char* name, void *data, const char* typeName, Dword flags, ObjectValue *val)
{
	MetaAttri *a = new MetaAttri;
	a->type = OAttribute::typeMeta;
	a->name = name;
	a->flags = flags;
	a->data = data;
	a->typeName = typeName;
	a->objectValue = val;

	attributes.push_back(a);
}

void AttriVisitor::visitMetaArray(const char* name, void *data, const char* typeName, Dword flags, ObjectValue *val)
{
	MetaAttri *a = new MetaAttri;
	a->type = OAttribute::typeMetaArray;
	a->name = name;
	a->flags = flags;
	a->data = data;
	a->typeName = typeName;
	a->objectValue = val;

	attributes.push_back(a);
}

void AttriVisitor::visitObject(const char* name, OObject* &obj, OClass *cls, Dword flags, ObjectValue *val)
{
	ObjAttri *a = new ObjAttri;
	a->type = OAttribute::typeObject;
	a->name = name;
	a->flags = flags;
	a->object = &obj;
	a->objectClass = cls;
	a->objectValue = val;

	attributes.push_back(a);
}

void AttriVisitor::visitObjArray(const char* name, OObject::Array &obj, OClass *cls, Dword flags, ObjectValue *val)
{
	ObjAttri *a = new ObjAttri;
	a->type = OAttribute::typeObjArray;
	a->name = name;
	a->flags = flags;
	a->objArray = &obj;
	a->objectClass = cls;
	a->objectValue = val;

	attributes.push_back(a);
}

void AttriVisitor::visitObjList(const char* name, OObject::List &obj, OClass *cls, Dword flags, ObjectValue *val)
{
	ObjAttri *a = new ObjAttri;
	a->type = OAttribute::typeObjList;
	a->name = name;
	a->flags = flags;
	a->objList = &obj;
	a->objectClass = cls;
	a->objectValue = val;

	attributes.push_back(a);
}
OO_NAMESPACE_END

