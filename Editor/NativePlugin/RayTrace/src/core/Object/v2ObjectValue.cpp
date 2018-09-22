// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#include "..\Common\v2ObjStd.h"
#include "v2objectvalue.h"

OO_NAMESPACE_BEGIN

OO_IMPL_ABSTRACT(ObjectValue, OObject)
OO_IMPL_CLASS(EnumValue, ObjectValue)

/*	原来在 OO_ENUM_BEGIN 时使用 "static EnumValue _theEnum" ，这样的结果使得引擎退出时
	VC 会报内存泄漏，虽然这并不是一个真正的泄漏。静态变量的 std::list, vector 等容器都会有这个问题，因为里面
	push_back 等操作时都要分配内存。现在改为由 EnumManager 统一管理。
*/

EnumManager::EnumManager()
{
	//enums = new EnumValueList;
}

EnumManager::~EnumManager() 
{
	for (EnumValueList::iterator it = enums.begin(); it != enums.end(); ++it) {
		//delete *it;
		EnumValue *ev = *it;
		ooAssert(ev->getRef() == 1);
		ooDecRef(ev);
		//ev->items.clear();
	}
	enums.clear();

	//delete enums;
}

void EnumManager::registerEnum(EnumValue *ev) 
{
	enums.push_back(ev);
}

EnumValue* EnumManager::createEnum() 
{
	EnumValue *ev = objNew(EnumValue);
	ooIncRef(ev);
	return ev;
}

EnumManager* getEnumManager()
{
	static EnumManager mgr;
	return &mgr;
}

OO_NAMESPACE_END