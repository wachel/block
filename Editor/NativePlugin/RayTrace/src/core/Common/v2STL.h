// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#pragma once

//#include "mmgr.h"

/*
	Reference:
		Allocators (STL)
		Gabriel Fleseriu and Andreas Masur
		http://www.codeguru.com/Cpp/Cpp/cpp_mfc/stl/article.php/c4079/
*/

OO_NAMESPACE_BEGIN

template <typename T> class oo_pool_allocator;

#if OO_USE_OTL
namespace otl
{
	using namespace std;

	typedef basic_stringstream<char, char_traits<char>,	oo_pool_allocator<char> > stringstream;

};
#else
#	define otl std
#endif

#if OO_MEMORY_TRACE

TEMPL_SPEC class oo_pool_allocator<void>
{
public:
	typedef void* pointer;
	typedef const void* const_pointer;
	// reference to void members are impossible.
	typedef void value_type;
	template <class U> 
	struct rebind { typedef oo_pool_allocator<U> other; };
};    

// "The member function 'destroy()' is a bit trickier because it has to 
// decide whether to call a destructor or not. We will use the way C++ 
// resolves function overloading to solve this. We need a couple of helper 
// functions:"

namespace oo_pool_alloc{
	inline void destruct(char *){}
	inline void destruct(wchar_t*){}

	template <typename T> 
	inline void destruct(T *t){t->~T();}
} // namespace

template <typename T>
class oo_pool_allocator
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	template <class U> 
	struct rebind { typedef oo_pool_allocator<U> other; };
	oo_pool_allocator(){}
	pointer address(reference x) const { return &x;}
	const_pointer address(const_reference x) const {return &x;}
	pointer allocate(size_type size, oo_pool_allocator<void>::const_pointer hint = 0)
	{
		return static_cast<pointer>(op_new_vc(size*sizeof(T), "[STL_ALLOC]", 0));
	}
	//for Dinkumware:
	char *_Charalloc(size_type n){return static_cast<char*>(op_new_sc(n, "[STL_ALLOC]", 0));}
	// end Dinkumware

	template <class U> oo_pool_allocator(const oo_pool_allocator<U>&){}
	void deallocate(pointer p, size_type n)
	{
		// op_delete_vc(p, n);
		op_delete_vc(p);
	}
	void deallocate(void *p, size_type n)
	{
		// mem_.deallocate(p, n);
		op_delete_vc(p);
	}
	size_type max_size() const throw() {return size_t(-1) / sizeof(value_type);}
	void construct(pointer p, const T& val)
	{
		new(static_cast<void*>(p)) T(val);
	}
	void construct(pointer p)
	{
		new(static_cast<void*>(p)) T();
	}
	void destroy(pointer p){
		oo_pool_alloc::destruct(p);
	}
	//static void dump(){mem_.dump();};
private:

	//static pool mem_;
};

template <typename T, typename U>
inline bool operator==(const oo_pool_allocator<T>&, const oo_pool_allocator<U>){return true;}

template <typename T, typename U>
inline bool operator!=(const oo_pool_allocator<T>&, const oo_pool_allocator<U>){return false;}

#else // ! OO_MEMORY_TRACE

#define oo_pool_allocator std::allocator
	
#endif


// STL container with memory manager support.

template <class T, class Allocator=oo_pool_allocator<T> >
struct OArray
{
	typedef std::vector<T, Allocator> Type;
	typedef typename Type::iterator iterator;
	typedef typename Type::const_iterator const_iterator;
};

template <class T, class Allocator=oo_pool_allocator<T> >
struct OList
{
	typedef std::list<T, Allocator> Type;
	typedef typename Type::iterator iterator;
	typedef typename Type::const_iterator const_iterator;
};

template <
	class Key, 
	class T, 
	class Traits = std::less<Key>, 
	class Allocator=oo_pool_allocator<std::pair <const Key, T> >
> 
struct OMap
{
	typedef std::map<Key, T, Traits, Allocator> Type;  
	typedef typename Type::iterator iterator;
	typedef typename Type::const_iterator const_iterator;
};

template <
	class Key, 
	class Traits = std::less<Key>, 
	class Allocator=oo_pool_allocator<Key>
> 
struct OSet
{
	typedef std::set<Key, Traits, Allocator> Type;  
	typedef typename Type::iterator iterator;
	typedef typename Type::const_iterator const_iterator;
};

//////////////////////////////////////////////////////////////////////////
// 因为mmgr不支持多线程，这里的类用于多线程里面用stl。
// 使用malloc,free分配内存（因为new/delete可能被mmgr重载），缺陷是不支持构造,析构函数。

template <typename T> class no_mmgr_allocator;

template<>  class no_mmgr_allocator<void>
{
public:
	typedef void* pointer;
	typedef const void* const_pointer;
	// reference to void members are impossible.
	typedef void value_type;
	template <class U> 
	struct rebind { typedef no_mmgr_allocator<U> other; };
};    

// "The member function 'destroy()' is a bit trickier because it has to 
// decide whether to call a destructor or not. We will use the way C++ 
// resolves function overloading to solve this. We need a couple of helper 
// functions:"

//namespace no_mmgr_alloc{
//	inline void destruct(char *){}
//	inline void destruct(wchar_t*){}
//
//	template <typename T> 
//	inline void destruct(T *t){t->~T();}
//} // namespace

template <typename T>
class no_mmgr_allocator
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	template <class U> 
	struct rebind { typedef no_mmgr_allocator<U> other; };
	no_mmgr_allocator(){}
	pointer address(reference x) const { return &x;}
	const_pointer address(const_reference x) const {return &x;}
	pointer allocate(size_type size, no_mmgr_allocator<void>::const_pointer hint = 0)
	{
		return static_cast<pointer>(malloc(size*sizeof(T)));
	}
	//for Dinkumware:
	char *_Charalloc(size_type n){return static_cast<char*>(malloc(n));}
	// end Dinkumware

	template <class U> no_mmgr_allocator(const no_mmgr_allocator<U>&){}
	void deallocate(pointer p, size_type n)
	{
		free(p);
	}
	void deallocate(void *p, size_type n)
	{
		free(p);
	}
	size_type max_size() const throw() {return size_t(-1) / sizeof(value_type);}
	void construct(pointer p, const T& val)
	{
		//new(static_cast<void*>(p)) T(val);
	}
	void construct(pointer p)
	{
		//new(static_cast<void*>(p)) T();
	}
	void destroy(pointer p){
		//no_mmgr_alloc::destruct(p);
	}
	//static void dump(){mem_.dump();};
private:

	//static pool mem_;
};

template <typename T, typename U>
inline bool operator==(const no_mmgr_allocator<T>&, const no_mmgr_allocator<U>){return true;}

template <typename T, typename U>
inline bool operator!=(const no_mmgr_allocator<T>&, const no_mmgr_allocator<U>){return false;}

// STL container with memory manager support.

template <class T, class Allocator=no_mmgr_allocator<T> >
struct NoMmgrArray
{
	typedef std::vector<T, Allocator> Type;
	typedef typename Type::iterator iterator;
	typedef typename Type::const_iterator const_iterator;
};

template <class T, class Allocator=no_mmgr_allocator<T> >
struct NoMmgrList
{
	typedef std::list<T, Allocator> Type;
	typedef typename Type::iterator iterator;
	typedef typename Type::const_iterator const_iterator;
};

template <
	class Key, 
	class T, 
	class Traits = std::less<Key>, 
	class Allocator=no_mmgr_allocator<std::pair <const Key, T> >
> 
struct NoMmgrMap
{
	typedef std::map<Key, T, Traits, Allocator> Type;  
	typedef typename Type::iterator iterator;
	typedef typename Type::const_iterator const_iterator;
};

template <
	class Key, 
	class Traits = std::less<Key>, 
	class Allocator=no_mmgr_allocator<Key>
> 
struct NoMmgrSet
{
	typedef std::set<Key, Traits, Allocator> Type;  
	typedef typename Type::iterator iterator;
	typedef typename Type::const_iterator const_iterator;
};
OO_NAMESPACE_END