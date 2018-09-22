// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#pragma once

#include "v2ObjBase.h"
#include "tinyxml.h"
#include "..\Common\v2STL.h"
#include "..\Utility\v2String.h"
//#include "v2Exception.h"

OO_NAMESPACE_BEGIN

class ObjectValue;

// backward compatible for v1 xml file.
#define TRY_V1 1
static const int V1_BIG_ARRAY_SIZE = 10;

enum XmlFormat
{
	XF_X01,	// no xml format specified.
	XF_X02, // add xml format.
	XF_X03, // 为了方便合并xml文件，去掉数组、链表前面的"Num"属性和子项的"ItemXXX"里面的数字"XXX"。
			// 去掉"ItemXXX"里面的数字后对XML文件格式要求:
			//		1. 容器里面子项的顺序必须从前往后排，以前对XML里容器内项目顺序是无关的。
			//		2. 容器里面子项名字都是"Item"，装载时会忽略掉名字不是"Item"的子项。

	XF_Last = XF_X03 // 存盘用最后的版本保存
};

//////////////////////////////////////////////////////////////////////////
// XML Visitor
class VFileSystemBase;
struct ArgWriteXml : public OArgBase { 
	TiXmlNode *node; 
	String filename; 
	Dword fileVersion; 
	Dword classVersion;
	VFileSystemBase* fileSystem;
};
struct ArgReadXml : public OArgBase { 
	TiXmlNode *node; 
	String filename; 
	Dword fileVersion; 
	Dword classVersion; 
	XmlFormat format; 
	VFileSystemBase* fileSystem;
};

/*
template <class T_OBJ, class T_ARG>
void visitMeta(const char* name, T_OBJ &obj, T_ARG &arg)
{
	COMPILER_ASSERT(false);
}
*/

class OClass;

// write...
template <class T_OBJ>
void visitMeta(const char* name, T_OBJ &obj, const char* type, ArgWriteXml &arg, Dword flags, ObjectValue *val);

template <class T_OBJ>
void visitMetaArray(const char* name, typename OArray<T_OBJ>::Type &obj, const char* type, ArgWriteXml &arg, Dword flags, ObjectValue *val);

template <class T_OBJ>
void visitObject(const char* name, T_OBJ* &obj, OClass *cls, ArgWriteXml &arg, Dword flags, ObjectValue *val);

template <class T_OBJ, class T_CON>
inline void _visitObjArray(const char* name, T_CON &obj, OClass *cls, ArgWriteXml &arg, Dword flags, ObjectValue *val);

template <class T_OBJ>
void visitObjArray(const char* name, typename T_OBJ::Array &obj, OClass *cls, ArgWriteXml &arg, Dword flags, ObjectValue *val);
template <class T_OBJ>
void visitObjList(const char* name, typename T_OBJ::List&obj, OClass *cls, ArgWriteXml &arg, Dword flags, ObjectValue *val);

// read...
template <class T_OBJ>
void visitMeta(const char* name, T_OBJ &obj, const char* type, ArgReadXml &arg, Dword flags, ObjectValue *val);

template <class T_OBJ>
void visitMetaArray(const char* name, typename OArray<T_OBJ>::Type &obj, const char* type, ArgReadXml &arg, Dword flags, ObjectValue *val);

template <class T_OBJ>
void visitObject(const char* name, T_OBJ* &obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val);
template <class T_OBJ, class T_CON>
inline void _visitObjArray(const char* name, T_CON &obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val);
template <class T_OBJ>
void visitObjArray(const char* name, typename T_OBJ::Array &obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val);
template <class T_OBJ>
void visitObjList(const char* name, typename T_OBJ::List&obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val);

OO_NAMESPACE_END