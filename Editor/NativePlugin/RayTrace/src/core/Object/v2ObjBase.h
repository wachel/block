// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#pragma once
#include "..\Common\v2ObjStd.h"
OO_NAMESPACE_BEGIN

class  OUnknown
{
public:
	OUnknown() : numRef(0) {}
	virtual ~OUnknown() { }

	void addRef(void) { 
		++numRef; 
	}
	void decRef(void)
	{
		ooBreakAssert(numRef > 0);

		--numRef;
	}
	int getRef() const {
		return numRef;
	}

	void release() {
		decRef();
		if (numRef == 0)
			doRelease();
	}

private:
	// do "delete this;"
	virtual void doRelease() = 0;

	int numRef;
};

//////////////////////////////////////////////////////////////////////////
// OPointer

template <class T> class OPointer
{
	T* ptr;
public:
	OPointer() : ptr(0) { }
	OPointer(T* p_) : ptr(p_) { 
		//if (ptr)
		//	ptr->addRef(); 

		if (ptr){
			OUnknown* unknowPtr = (OUnknown*)(void*)ptr;
			unknowPtr->addRef();
		}
	}
	OPointer(const OPointer &p_) : ptr(0) { 
		*this = p_;
	}

	~OPointer(void) { 
		//if (ptr)
		//	ptr->release(); 
		if (ptr){
			OUnknown* unknowPtr = (OUnknown*)(void*)ptr;
			unknowPtr->release();
		}
	}

	T* pointer() {
		return ptr;
	}
	T*& pointerRef() {
		return ptr;
	}

	const T* pointer() const {
		return ptr;
	}
	const T*& pointerRef() const {
		return ptr;
	}

	operator T*(void) { 
		return ptr; 
	}
	operator const T*(void) const { 
		return ptr; 
	}

	// v2, add.
	//operator const T*&(void) const { 
	//	return ptr; 
	//}

	T& operator*(void) { 
		return *ptr; 
	}
	T* operator->(void) { 
		return ptr; 
	}
	const T* operator->(void) const { 
		return ptr; 
	}


	/*
	OPointer& operator=(OPointer<T> &p_) {
	return operator=((T *) p_);
	}
	OPointer& operator=(T* p_) {
	p->release(); 
	p = p_; 
	p->addRef(); 
	return *this;
	}
	*/

	const OPointer& operator=(const OPointer<T> &p_) {
		return operator=((const T *) p_);
	}
	const OPointer& operator=(const T* p_) {
		/* old, crash when a = a(a.ref = 1)
		if (ptr)
		ptr->release(); 
		ptr = const_cast<T*>(p_); 
		if (ptr)
		ptr->addRef(); 
		return *this;
		*/
		//if (p_)
		//	const_cast<T*>(p_)->addRef(); 
		//if (ptr)
		//	ptr->release(); 
		if (p_){
			T* ptr_ = const_cast<T*>(p_); 
			OUnknown* unknowPtr = (OUnknown*)(void*)ptr_;
			unknowPtr->addRef();
		}
		if(ptr){
			OUnknown* unknowPtr = (OUnknown*)(void*)ptr;
			unknowPtr->release();
		}
		ptr = const_cast<T*>(p_);
		return *this;
	}

	/*
	bool operator==(const OPointer<T> &p_) {
	return ptr == p_.ptr;
	}
	bool operator==(const T* p_) {
	return ptr == p_;
	}
	*/
};

#define OO_DECL_PTR(CLASS) \
public: \
	typedef OPointer<CLASS> Ptr; \
	typedef OArray<Ptr>::Type Array; \
	typedef OList<Ptr>::Type List;

//////////////////////////////////////////////////////////////////////////

/// OO_DECL_XXX ʱ����Ĳ������ͣ�Ӧ�ó������������֪����ǰ���Ǹ��ػ��汾���������á�
enum OArgType
{
	// ������ʶ������������Ӧ��֪�����ڶ��̻��Ǵ��̣����ֶ����ƻ���Xml�����磺"if (arg.type & AT_Write) ...��
	AT_Write	= 1 << 0,
	AT_Read		= 1 << 1,

	// ��Щ��������Ӧ��֪������Ĳ�����
	AT_WriteBin	= (1 << 2) | AT_Write,
	AT_ReadBin	= (1 << 3) | AT_Read,
	AT_WriteXml	= (1 << 4) | AT_Write,
	AT_ReadXml	= (1 << 5) | AT_Read,
};

struct OArgBase
{
	OArgType type;	// Ӧ���� AT_WriteBin....AT_ReadXml��4��ֵ֮һ������ʹ��AT_Write��AT_Read
};

OO_NAMESPACE_END