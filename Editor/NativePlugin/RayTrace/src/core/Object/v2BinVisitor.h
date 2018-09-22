// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#pragma once

#include "v2ObjBase.h"
//#include "v2Exception.h"

OO_NAMESPACE_BEGIN

enum BinFormat
{
	BF_B01,
	BF_B02,
	BF_B03, // add object size to save.
	BF_B04, // changes arg.classVersion from end class version to local class version.

	BF_Last = BF_B04 // 存盘用最后的版本保存
};

class ObjectValue;

//////////////////////////////////////////////////////////////////////////
// Bin Visitor

class VFileSystemBase;
struct ArgWriteBin  : public OArgBase { 
	std::ostream *file; 
	String filename; 
	Dword fileVersion; 
	Dword classVersion; 
	VFileSystemBase* fileSystem;
};
struct ArgReadBin  : public OArgBase { 
	std::istream *file; 
	String filename; 
	Dword fileVersion; 
	Dword classVersion; 
	BinFormat format; 
	VFileSystemBase* fileSystem;
};

inline void _writeString(std::ostream *file, const String &str)
{
	// 不能用 "*file << str;"，因为可能把str里非结尾那些"\0"存入（比如使用过 String::resize + memcpy）。
	*file << str.c_str();
	file->put(0);
}

inline void _readString(std::istream *file, String &str)
{
	str = "";
	char c = 0;
	while (file->get(c) && !file->eof() && c != 0)
		str += c;
}

// write...
template <class T_OBJ>
inline void visitMeta(const char* name, T_OBJ &obj, const char* type, ArgWriteBin &arg, Dword flags, ObjectValue *val)
{
	arg.file->write((const char*)&obj, sizeof(T_OBJ));
}

// String
template<>
inline void visitMeta<String>(const char* name, String &obj, const char* type, ArgWriteBin &arg, Dword flags, ObjectValue *val)
{
	_writeString(arg.file, obj);
}

// vc2003 的 typename 支持有问题，应该是个 VC 的 Bug 。
// 应该是: #define META_VECTOR_TYPE(TYPE) typename OArray<TYPE>::Type

// 不用 typename 版本：
//COMPILER_ASSERT(!OO_USE_OTL);
#define META_VECTOR_TYPE(TYPE) std::vector<TYPE>

template <class T_OBJ>
inline void visitMetaArray(const char* name, META_VECTOR_TYPE(T_OBJ) &obj, const char* type, ArgWriteBin &arg, Dword flags, ObjectValue *val)
{
	// write num.
	int num = obj.size();
	arg.file->write((const char*)&num, sizeof(num));
	
	// write all items.
	if (!obj.empty())	//  [1/4/2009 Archer]
		arg.file->write((const char*)(&(*obj.begin())), num*sizeof(T_OBJ));
}

// String

template<>
inline void visitMetaArray<String>(const char* name, META_VECTOR_TYPE(String) &obj, const char* type, ArgWriteBin &arg, Dword flags, ObjectValue *val)
{
	// write num.
	int num = obj.size();
	arg.file->write((const char*)&num, sizeof(num));

	// write all items.
	for (int i = 0; i < num; ++i)
		visitMeta<String>(0, obj[i], "String", arg, flags, val);
}

template <class T_OBJ>
void visitObject(const char* name, T_OBJ* &obj, OClass *cls, ArgWriteBin &arg, Dword flags, ObjectValue *val);

template <class T_OBJ, class T_CON>
void _visitObjArray(const char* name, T_CON &obj, OClass *cls, ArgWriteBin &arg, Dword flags, ObjectValue *val);

template <class T_OBJ>
void visitObjArray(const char* name, typename T_OBJ::Array &obj, OClass *cls, ArgWriteBin &arg, Dword flags, ObjectValue *val)
{
	_visitObjArray<T_OBJ, typename T_OBJ::Array>(name, obj, cls, arg, flags, val);
}

template <class T_OBJ>
void visitObjList(const char* name, typename T_OBJ::List&obj, OClass *cls, ArgWriteBin &arg, Dword flags, ObjectValue *val)
{
	_visitObjArray<T_OBJ, typename T_OBJ::List>(name, obj, cls, arg, flags, val);
}


// read...
template <class T_OBJ>
inline void visitMeta(const char* name, T_OBJ &obj, const char* type, ArgReadBin &arg, Dword flags, ObjectValue *val)
{
	arg.file->read((char*)&obj, sizeof(obj));
}

// String
template<>
inline void visitMeta<String>(const char* name, String &obj, const char* type, ArgReadBin &arg, Dword flags, ObjectValue *val)
{
	_readString(arg.file, obj);
}

template <class T_OBJ>
void visitMetaArray(const char* name, META_VECTOR_TYPE(T_OBJ) &obj, const char* type, ArgReadBin &arg, Dword flags, ObjectValue *val)
{
	// read num.
	int num = 0;
	arg.file->read((char*)&num, sizeof(num));

	// read all items.
	obj.resize(num);
	if (num)
		arg.file->read((char*)(&(*obj.begin())), num*sizeof(T_OBJ));
}

// String
template<>
inline void visitMetaArray<String>(const char* name, META_VECTOR_TYPE(String) &obj, const char* type, ArgReadBin &arg, Dword flags, ObjectValue *val)
{
	// read num.
	int num = 0;
	arg.file->read((char*)&num, sizeof(num));

	// read all items.
	obj.resize(num);
	for (int i = 0; i < num; ++i)
		visitMeta<String>(0, obj[i], "String", arg, flags, val);
}

template <class T_OBJ>
void visitObject(const char* name, T_OBJ* &obj, OClass *cls, ArgReadBin &arg, Dword flags, ObjectValue *val);

template <class T_OBJ, class T_CON>
inline void _visitObjArray(const char* name, T_CON &obj, OClass *cls, ArgReadBin &arg, Dword flags, ObjectValue *val);
template <class T_OBJ>
void visitObjArray(const char* name, typename T_OBJ::Array &obj, OClass *cls, ArgReadBin &arg, Dword flags, ObjectValue *val)
{
	_visitObjArray<T_OBJ, typename T_OBJ::Array>(name, obj, cls, arg, flags, val);
}

template <class T_OBJ>
void visitObjList(const char* name, typename T_OBJ::List &obj, OClass *cls, ArgReadBin &arg, Dword flags, ObjectValue *val)
{
	_visitObjArray<T_OBJ, typename T_OBJ::List>(name, obj, cls, arg, flags, val);
}

OO_NAMESPACE_END