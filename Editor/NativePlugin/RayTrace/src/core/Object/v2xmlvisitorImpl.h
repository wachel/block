// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#pragma once

#include "v2XmlVisitor.h"
#include "..\Utility\v2StringConverter.h"

OO_NAMESPACE_BEGIN


// write...
template <class T_OBJ>
void visitMeta(const char* name, T_OBJ &obj, const char* type, ArgWriteXml &arg, Dword flags, ObjectValue *val)
{
	// write class.
	TiXmlElement *elem = arg.node->InsertEndChild(TiXmlElement(name))->ToElement();
	elem->SetAttribute("Class", type);

	// write attri.

	//// XF_X01
	//TiXmlText text(cString::toString(obj).c_str());
	//elem->InsertEndChild(text);

	// XF_X02
	elem->SetAttribute("Value", cString::toString(obj).c_str());
}

template <class T_OBJ>
void visitMetaArray(const char* name, typename OArray<T_OBJ>::Type &obj, const char* type, ArgWriteXml &arg, Dword flags, ObjectValue *val)
{
	// write class.
	TiXmlElement *elem = arg.node->InsertEndChild(TiXmlElement(name))->ToElement();

	String clsName;
	elem->SetAttribute("Class", "MetaArray");

	// write attri.
	int num = obj.size();
	//if (num < BIG_ARRAY_SIZE) {
	if (true) {
		// removed for XF_X03
		// write num.
		//elem->SetAttribute("Num", num);

		ArgWriteXml arg2 = arg;
		arg2.node = elem;
		arg2.classVersion = 0;

		// write num.
		//visitMeta("Num", num, "int", arg2, flags, val);

		// write all items.
		for (unsigned int i = 0; i < obj.size(); ++i) {
			//String itemName = "Item" + cString::toString(i);
			String itemName = "Item";
            visitMeta(itemName.c_str(), obj[i], type, arg2, flags, val);
		}
	}
	else {
		// removed for XF_X03
		// write num.
		//elem->SetAttribute("Num", num);


		String str;
		for (int i = 0; i < num; ++i) {
			str += cString::toString(obj[i]) + "\n";
		}

		TiXmlText text(str.c_str());
		elem->InsertEndChild(text);
	}
}

template <class T_OBJ>
void visitObject(const char* name, T_OBJ* &obj, OClass *cls, ArgWriteXml &arg, Dword flags, ObjectValue *val)
{
	TiXmlElement *elem = arg.node->InsertEndChild(TiXmlElement(name))->ToElement();

	if (obj) {
		((OObject*)obj)->beforeSaved();

		if (obj->getUrl().empty()) {
			// embedded object.
		
			// write store type.
			elem->SetAttribute("Store", (int)OObject::storeEmbedded);

			// write class.
			OClass* cls = obj->getClass();

			// we should not save the implemention class of a plugin interface.
			// find the interface class.
			while (cls->hasFlags(OClass::flagPluginImp)) {
				cls = cls->getBaseClass();
				ooBreakAssert(cls);
			}
			
			elem->SetAttribute("Class", cls->m_className);
			elem->SetAttribute("Version", cls->version);
			
			// write all attributes.
			ArgWriteXml arg2 = arg;
			arg2.node = elem;
			arg2.classVersion = cls->version;
			obj->_objVisit(arg2);
		}
		else {
			// referenced object.
			elem->SetAttribute("Store", (int)OObject::storeReference);
			elem->SetAttribute("Ref", obj->getUrl().c_str());			
		}

		((OObject*)obj)->postSaved();
	}
	else {
		// NULL object.
		elem->SetAttribute("Store", (int)OObject::storeNull);
	}
}


template <class T_OBJ, class T_CON>
inline void _visitObjArray(const char* name, T_CON &obj, OClass *cls, ArgWriteXml &arg, Dword flags, ObjectValue *val)
{
	// write class.
	TiXmlElement *elem = arg.node->InsertEndChild(TiXmlElement(name))->ToElement();
	elem->SetAttribute("Class", "ObjArray");

	// write num.
	int num = obj.size();
	// removed for XF_X03
    //elem->SetAttribute("Num", num);

	// write all items.
	ArgWriteXml arg2 = arg;
	arg2.node = elem;
	arg2.classVersion = 0xFFFFFFFF;

	for (typename T_CON::iterator it = obj.begin(); it != obj.end(); ++it) {
		//String itemName = "Item" + cString::toString(std::distance(obj.begin(), it));
		String itemName = "Item";
		visitObject(itemName.c_str(), (*it).pointerRef(), cls, arg2, flags, val);
	}
}

template <class T_OBJ>
void visitObjArray(const char* name, typename T_OBJ::Array &obj, OClass *cls, ArgWriteXml &arg, Dword flags, ObjectValue *val)
{
	_visitObjArray<T_OBJ, typename T_OBJ::Array>(name, obj, cls, arg, flags, val);
}

template <class T_OBJ>
void visitObjList(const char* name, typename T_OBJ::List&obj, OClass *cls, ArgWriteXml &arg, Dword flags, ObjectValue *val)
{
	_visitObjArray<T_OBJ, typename T_OBJ::List>(name, obj, cls, arg, flags, val);
}

// read...
template <class T_OBJ>
void visitMeta(const char* name, T_OBJ &obj, const char* type, ArgReadXml &arg, Dword flags, ObjectValue *val)
{
	// read class.
	TiXmlElement *elem = arg.node->FirstChildElement(name);
	if (!elem)
		return; // can not found this element.
	//ooBreakAssert(elem);

	const char* className;
	className = elem->Attribute("Class");
	ooBreakAssert(strcmp(className, type) == 0);

	// read attri.

	if (arg.format == XF_X01) {
		TiXmlText *text = elem->FirstChild()->ToText();
		// note: text == NULL when read a NULL string.
		const char* str = text ? text->Value() : "";
		cString::parse(str, obj);	
	}
	else {
		const char* value = elem->Attribute("Value");
		ooBreakAssert(value);
		cString::parse(value, obj);	
	}
}

template <class T_OBJ>
void visitMetaArray(const char* name, typename OArray<T_OBJ>::Type &obj, const char* type, ArgReadXml &arg, Dword flags, ObjectValue *val)
{
	// read class.
	TiXmlElement *elem = arg.node->FirstChildElement(name);
	if (!elem)
		return; // can not found this element.
    //ooBreakAssert(elem);

	const char* className;
	className = elem->Attribute("Class");
	ooBreakAssert(strcmp(className, "MetaArray") == 0);

	// read num.
	int num = 0;

	if (arg.format < XF_X03) {
	#if TRY_V1
		const char *s = elem->Attribute("Num", &num);
		if (!s) {
			ArgReadXml arg2 = arg;
			arg2.node = elem;
			arg2.classVersion = 0;
	
			visitMeta("Num", num, "int", arg2, 0, 0);
		}
	#else
		elem->Attribute("Num", &num);
	#endif

		obj.resize(num);
		// read all items.
		if (num < V1_BIG_ARRAY_SIZE || !elem->FirstChildElement("Items")) {
			TiXmlElement *itemElem = elem->FirstChildElement("Item0");
			for (int i = 0; i < num; ++i) {
				ooBreakAssert(itemElem);

				if (arg.format == XF_X01) {
					TiXmlText *text = itemElem->FirstChild()->ToText();

					// note: text == NULL when read a NULL string.
					const char* str = text ? text->Value() : "";

					cString::parse(str, obj[i]);
				}
				else {
					const char* value = itemElem->Attribute("Value");
					ooBreakAssert(value);
					cString::parse(value, obj[i]);	
				}

				itemElem = itemElem->NextSiblingElement();
			}
		}
		else {
			// read big array.
			T_OBJ dummy;
			String str;
			str = cString::toString(dummy);

			StringVector sv = uString::split(str);
			int numSeg = sv.size();

			//String str;
			ArgReadXml arg2 = arg;
			arg2.node = elem;
			arg2.classVersion = 0;

			visitMeta("Items", str, "String", arg2, 0, 0);

			//StringVector sv = str.split("\r\n");
			sv = uString::split(str);
			ooBreakAssert(sv.size() >= num*numSeg);

			String line;
			for (int i = 0; i < num; ++i) {
				line = "";
				for (int j = 0; j < numSeg; ++j) {
					line += sv[i*numSeg +j] + " ";
				}
				cString::parse(line, obj[i]);
			}
		}
	}
	else { // XF_X03版
		num = elem->CalcChildrenElementNum("Item"); // for XF_X03

		obj.resize(num);

		// read all items.
		{
			TiXmlElement *itemElem = elem->FirstChildElement("Item");
			for (int i = 0; i < num; ++i) {
				ooBreakAssert(itemElem);

				if (arg.format == XF_X01) {
					TiXmlText *text = itemElem->FirstChild()->ToText();

					// note: text == NULL when read a NULL string.
					const char* str = text ? text->Value() : "";

					cString::parse(str, obj[i]);
				}
				else {
					const char* value = itemElem->Attribute("Value");
					ooBreakAssert(value);
					cString::parse(value, obj[i]);	
				}

				itemElem = itemElem->NextSiblingElement("Item");
			}
		}
	}
}

template <class T_OBJ>
void visitObjectOfElem(TiXmlElement *elem, T_OBJ* &obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val)
{
	ooBreakAssert(elem);
	safeRelease(obj);

	// read store type.
	OObject::StoreType store = OObject::storeNull;
	elem->Attribute("Store", (int*)&store);

	if (store == OObject::storeEmbedded) {
		// read class.
		const char* className;
		className = elem->Attribute("Class");

		int classVersion = 0; // note: Dword -> int.
		elem->Attribute("Version", &classVersion);

		OClass *c = OClass::fromName(className, true);
		if (!c) {
			ooThrow(EClassNotFound(className));		
		}

		obj = (T_OBJ*)c->createObject();
		obj->addRef();

		// read attri.
		ArgReadXml arg2 = arg;
		arg2.node = elem;
		arg2.classVersion = classVersion;

		obj->_objVisit(arg2);

		((OObject*)obj)->postLoaded(classVersion);
	}
	else if (store == OObject::storeReference) {
		const char* url = elem->Attribute("Ref");
		ooBreakAssert(url);

		obj = (T_OBJ*)OObject::loadFromFile(arg.fileSystem, url);
		if (obj)
			obj->addRef();
	}
	else {
		ooBreakAssert(store == OObject::storeNull);
		if (store != OObject::storeNull)
			OObject::addLoadingError(OObject::LEC_Error, "Load object in xml file failed: store != OObject::storeNull");
	}

	//if (obj)
	//	((OObject*)obj)->postLoaded(classVersion);
}

template <class T_OBJ>
void visitObject(const char* name, T_OBJ* &obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val)
{
	TiXmlElement *elem = 0;
	if (name)
		elem = arg.node->FirstChildElement(name);
	else
		// no name provided, get the first element.
		elem = arg.node->FirstChildElement();

	if (!elem)
		return; // can not found the element.

	visitObjectOfElem(elem, obj, cls, arg, flags, val);
}


template <class T_OBJ, class T_CON>
inline void _visitObjArrayInsideElem(TiXmlElement *elem, T_CON &obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val)
{
	ooBreakAssert(elem);

	const char* className;
	className = elem->Attribute("Class");

	// @fixme
	ooBreakAssert(strcmp(className, "ObjArray") == 0 || strcmp(className, "ObjList") == 0);

	// read num.
	int num = 0;

	if (arg.format < XF_X03) {
#if TRY_V1
		const char *s = elem->Attribute("Num", &num);
		if (!s) {
			ArgReadXml arg2 = arg;
			arg2.node = elem;
			arg2.classVersion = 0;

			visitMeta("Num", num, "int", arg2, 0, 0);
		}
#else
		elem->Attribute("Num", &num);
#endif
		obj.resize(num);

		for (typename T_CON::iterator it = obj.begin(); it != obj.end(); ++it) {
			String name = "Item" + cString::toString(std::distance(obj.begin(), it));

			ArgReadXml arg2 = arg;
			arg2.node = elem;
			arg2.classVersion = 0xFFFFFFFF;

			T_OBJ *itemObj = 0;
			visitObject(name.c_str(), itemObj, cls, arg2, flags, val);
			if (itemObj)
				itemObj->decRef();

			// insert in to array.
			*it = itemObj;
		}
	}
	else { // XF_X03代码
		num = elem->CalcChildrenElementNum("Item"); // for XF_X03
		obj.resize(num);

		TiXmlElement *childElem = elem->FirstChildElement("Item");
		typename T_CON::iterator childIt = obj.begin(); 

		for (int i = 0; i < num; i++) {
			ooBreakAssert(childElem && childIt != obj.end());
		//for (typename T_CON::iterator it = obj.begin(); it != obj.end(); ++it) {
			//String name = "Item" + cString::toString(std::distance(obj.begin(), it));

			ArgReadXml arg2 = arg;
			arg2.node = elem;
			arg2.classVersion = 0xFFFFFFFF;

			T_OBJ *itemObj = 0;
			visitObjectOfElem(childElem, itemObj, cls, arg2, flags, val);
			if (itemObj)
				itemObj->decRef();

			// insert in to array.
			*childIt = itemObj;

			childElem = childElem->NextSiblingElement("Item");
			childIt++;
		}
	}
}

template <class T_OBJ, class T_CON>
inline void _visitObjArray(const char* name, T_CON &obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val)
{
	TiXmlElement *elem = 0;
	if (name)
		elem = arg.node->FirstChildElement(name);
	else
		// no name provided, get the first element.
		elem = arg.node->FirstChildElement();
	if (!elem)
		return; // can not found this element.

	_visitObjArrayInsideElem<T_OBJ, T_CON>(elem, obj, cls, arg, flags, val);
}

template <class T_OBJ>
void visitObjArray(const char* name, typename T_OBJ::Array &obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val)
{
	_visitObjArray<T_OBJ, typename T_OBJ::Array>(name, obj, cls, arg, flags, val);
}

template <class T_OBJ>
void visitObjList(const char* name, typename T_OBJ::List&obj, OClass *cls, ArgReadXml &arg, Dword flags, ObjectValue *val)
{
	_visitObjArray<T_OBJ, typename T_OBJ::List>(name, obj, cls, arg, flags, val);
}

OO_NAMESPACE_END