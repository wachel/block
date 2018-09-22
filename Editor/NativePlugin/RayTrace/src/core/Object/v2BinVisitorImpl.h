// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#pragma once

#include "v2BinVisitor.h"

OO_NAMESPACE_BEGIN

template <class T_OBJ, class T_CON>
void _visitObjArray(const char* name, T_CON &obj, OClass *cls, ArgWriteBin &arg, Dword flags, ObjectValue *val)
{
	// write class.
	// nothing to do.

	// write num.
	int num = obj.size();
	arg.file->write((const char*)&num, sizeof(num));

	// write all items.
	ArgWriteBin arg2;
	arg2 = arg;

	for (typename T_CON::iterator it = obj.begin(); it != obj.end(); ++it) {
		visitObject(0, (*it).pointerRef(), cls, arg2, flags, val);
	}
}

template <class T_OBJ>
void visitObject(const char* name, T_OBJ* &obj, OClass *cls, ArgWriteBin &arg, Dword flags, ObjectValue *val)
{
	if (obj) {
		((OObject*)obj)->beforeSaved();

		if (obj->getUrl().empty()) {
			// embedded object.

			// write store type.
			OObject::StoreType store = OObject::storeEmbedded;
			arg.file->write((const char*)&store, sizeof(OObject::StoreType));

			// write class.
			{
				OClass* cls = obj->getClass();

				// we should not save the implemention class of a plugin interface.
				// find the interface class.
				while (cls->hasFlags(OClass::flagPluginImp)) {
					cls = cls->getBaseClass();
					ooBreakAssert(cls);
				}

				_writeString(arg.file, cls->m_className);
				arg.file->write((const char *)&cls->version, sizeof(cls->version));
			}

			// write all attributes.
			int objSize = -1, posBegin;
			arg.file->write((const char*)&objSize, sizeof(int)); // placeholder.
			posBegin = (int)arg.file->tellp();

			{
				// write all attributes to attr_data
				ArgWriteBin arg2;
				arg2 = arg;
				arg2.classVersion = cls->version;
				obj->_objVisit(arg2);
			}

			// write real object size.
			objSize = (int)arg.file->tellp() - posBegin;
			arg.file->seekp(posBegin - sizeof(int), std::ios::beg);
			arg.file->write((const char*)&objSize, sizeof(int));
			arg.file->seekp(0, std::ios::end);
		}
		else {
			// referenced object.

			// write store type.
			OObject::StoreType store = OObject::storeReference;
			arg.file->write((const char*)&store, sizeof(OObject::StoreType));

			// write url.
			_writeString(arg.file, obj->getUrl().c_str());
		}

		((OObject*)obj)->postSaved();
	}
	else {
		// NULL object.
		
		// write store type.
		OObject::StoreType store = OObject::storeNull;
		arg.file->write((const char*)&store, sizeof(OObject::StoreType));
	}
}


template <class T_OBJ>
void visitObject(const char* name, T_OBJ* &obj, OClass *cls, ArgReadBin &arg, Dword flags, ObjectValue *val)
{
	safeRelease(obj);

	// read store type.
	OObject::StoreType store = OObject::storeNull;
	arg.file->read((char*)&store, sizeof(store));

	if (store == OObject::storeEmbedded) {
		// read class.
		String className;
		_readString(arg.file, className);

		Dword classVersion;
		int objSize;
		if (arg.format == BF_B01) {
			classVersion = 0;
			objSize = -1;
		}
		else {
			arg.file->read((char *)&classVersion, sizeof(classVersion));

			if (arg.format > BF_B02)
				arg.file->read((char *)&objSize, sizeof(objSize));
		}

		OClass *c = OClass::fromName(className.c_str(), true);
		if (!c) {
			if ((int)arg.format < BF_B03) {
				ooThrow(EClassNotFound(className));
			}
			else {
				// skip this object.
				ooBreakAssert(objSize >= 0);
				arg.file->seekg(objSize, std::ios::cur);

				OObject::addLoadingError(OObject::LEC_ClassNotFound, className);
			}
		}
		else if((c->hasFlags(OClass::flagEditorOnly) && OObject::bClientMode) || c->hasFlags(OClass::flagSkip)){
			arg.file->seekg(objSize, std::ios::cur);
		}
		else {
			obj = (T_OBJ*)c->createObject();
			obj->addRef();

			// read attri.
			ArgReadBin arg2;
			arg2 = arg;
			arg2.classVersion = classVersion;

			obj->_objVisit(arg2);

			((OObject*)obj)->postLoaded(classVersion);
		}
	}
	else if (store == OObject::storeReference) {
		// read url.
		String url;
		_readString(arg.file, url);
		ooBreakAssert(!url.empty());

		obj = (T_OBJ*)OObject::loadFromFile(arg.fileSystem,url);
		if (obj)
			obj->addRef();
	}
	else {
		ooBreakAssert(store == OObject::storeNull);
		if (store != OObject::storeNull)
			OObject::addLoadingError(OObject::LEC_Error, "Load object in bin file failed: store != OObject::storeNull");
	}

	//if (obj)
	//	((OObject*)obj)->postLoaded(classVersion);
}

template <class T_OBJ, class T_CON>
inline void _visitObjArray(const char* name, T_CON &obj, OClass *cls, ArgReadBin &arg, Dword flags, ObjectValue *val)
{
	// read num.
	int num = 0;
	arg.file->read((char *)&num, sizeof(num));

	FUN_onReadObjectsNum fun_onReadObjectsNum = OObject::onReadObjectsNumCallback;
	FUN_onReadObjectsItem fun_onReadObjectsItem = OObject::onReadObjectsItemCallback;
	void* the_onReadObjectsCallbackArg = OObject::onReadObjectsCallbackArg;

	// 必须，防止类成员又调用此函数。
	OObject::onReadObjectsNumCallback = 0; 
	OObject::onReadObjectsItemCallback = 0;
	OObject::onReadObjectsCallbackArg = 0;

	if (fun_onReadObjectsNum)
		(*fun_onReadObjectsNum)(the_onReadObjectsCallbackArg, num);

	// read all items.
	obj.resize(num);

	for (typename T_CON::iterator it = obj.begin(); it != obj.end(); ++it) {
		ArgReadBin arg2;
		arg2 = arg;
		arg2.classVersion = 0xFFFFFFFF;

		T_OBJ *itemObj = 0;
		visitObject(0, itemObj, cls, arg2, flags, val);
		if (itemObj)
			itemObj->decRef();
	
		if (fun_onReadObjectsItem)
			(*fun_onReadObjectsItem)(the_onReadObjectsCallbackArg, itemObj);

		// insert in to array.
		*it = itemObj;
	}
}

OO_NAMESPACE_END