// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#pragma once

#include "v2Object.h"

OO_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// ObjectValue

class OBJ_API ObjectValue : public OObject
{
	OO_DECL_ABSTRACT(ObjectValue, OClass::flagAbstract, 0){}
public:
	//typedef OPointer<ObjectValue> Ptr;
	//typedef OArray<Ptr>::Type Array;
	//typedef OList<Ptr>::Type List;

public:
	ObjectValue() {}
	virtual ~ObjectValue() {}
};

//////////////////////////////////////////////////////////////////////////
// EnumValue

class OBJ_API EnumValue : public ObjectValue
{
	OO_DECL_CLASS(EnumValue, OClass::flagAbstract, 0){}
public:
	//typedef OPointer<EnumValue> Ptr;
	//typedef OArray<Ptr>::Type Array;
	//typedef OList<Ptr>::Type List;

public:
	EnumValue() : name(0) {}
	virtual ~EnumValue() {}

	typedef std::pair<const char*, int> Item;

	// enum items.
	typedef OList<Item>::Type ItemList;
	ItemList items;

	const char* name; // added for localization. see ObjEdit.cpp
};

//////////////////////////////////////////////////////////////////////////

/*
// use template
template <class T>
inline EnumValue* getEnumValue() { return 0; } // no enum.

// Macro for enum export.
#define OO_ENUM_BEGIN(TYPE) \
    template<> \
	inline EnumValue* getEnumValue<TYPE>() \
    { \
		EnumValue *theEnum = new EnumValue;

#define OO_ENUM_VALUE(VALUE) theEnum->items[#VALUE] = VALUE;

#define OO_ENUM_END() \
		return theEnum; \
	}
*/

class OBJ_API EnumManager
{
public:
	EnumManager();
	~EnumManager();

	void registerEnum(EnumValue *ev);
	EnumValue* createEnum();

private:
	typedef OList<EnumValue *>::Type EnumValueList;
	EnumValueList enums;
};
OBJ_API EnumManager* getEnumManager();


// use template
template <class T>
inline EnumValue* getEnumValue() { return 0; } // no enum.

// Macro for enum export.
#define OO_ENUM_BEGIN(TYPE) \
	template<> \
	inline EnumValue* getEnumValue<TYPE>() \
{ \
	static EnumValue *theEnum = 0;\
	if (!theEnum) { \
		static EnumValue *_theEnum;\
		_theEnum = getEnumManager()->createEnum(); \
		_theEnum->name = #TYPE; \
		theEnum = _theEnum;

#define OO_ENUM_VALUE(VALUE) theEnum->items.push_back(EnumValue::Item(#VALUE, VALUE));

#define OO_ENUM_END() \
		getEnumManager()->registerEnum(theEnum); \
	} \
	return theEnum; \
}

// no template
/*
// Macro for enum export.
#define OO_ENUM_BEGIN(TYPE) \
	inline EnumValue* getEnumValue_##TYPE() \
 { \
	static EnumValue *theEnum = 0;\
	if (!theEnum) { \
		static EnumValue _theEnum;\
		theEnum = &_theEnum; \
		theEnum->addRef(); \

#define OO_ENUM_VALUE(VALUE) theEnum->items[#VALUE] = VALUE;

#define OO_ENUM_END() \
	} \
	return theEnum; \
 }
 */

//////////////////////////////////////////////////////////////////////////
// BOOL EnumValue

OO_ENUM_BEGIN(Bool)
	OO_ENUM_VALUE(true)
	OO_ENUM_VALUE(false)
OO_ENUM_END()

OO_NAMESPACE_END