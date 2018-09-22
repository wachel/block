// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#include "..\Common\v2ObjStd.h"
#include "v2objectvalue.h"

OO_NAMESPACE_BEGIN

OO_IMPL_ABSTRACT(ObjectValue, OObject)
OO_IMPL_CLASS(EnumValue, ObjectValue)

/*	ԭ���� OO_ENUM_BEGIN ʱʹ�� "static EnumValue _theEnum" �������Ľ��ʹ�������˳�ʱ
	VC �ᱨ�ڴ�й©����Ȼ�Ⲣ����һ��������й©����̬������ std::list, vector ������������������⣬��Ϊ����
	push_back �Ȳ���ʱ��Ҫ�����ڴ档���ڸ�Ϊ�� EnumManager ͳһ����
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