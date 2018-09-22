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
	XF_X03, // Ϊ�˷���ϲ�xml�ļ���ȥ�����顢����ǰ���"Num"���Ժ������"ItemXXX"���������"XXX"��
			// ȥ��"ItemXXX"��������ֺ��XML�ļ���ʽҪ��:
			//		1. �������������˳������ǰ�����ţ���ǰ��XML����������Ŀ˳�����޹صġ�
			//		2. ���������������ֶ���"Item"��װ��ʱ����Ե����ֲ���"Item"�����

	XF_Last = XF_X03 // ���������İ汾����
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