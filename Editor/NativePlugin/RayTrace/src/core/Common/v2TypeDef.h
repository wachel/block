// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_TYPEDEF_H_
#define OO_TYPEDEF_H_

//#include "v2Config.h"

OO_NAMESPACE_BEGIN

typedef unsigned long       Dword;
typedef int                 Bool;
typedef unsigned char       Byte;
typedef unsigned short      Word;
typedef unsigned int        Uint;

#if OO_DOUBLE_PRECISION == 1
	typedef double			Real;
#else
	typedef float			Real;
#endif

/*
enum Bool
{
	True,
	False
};
*/

//#ifndef TRUE
//#	define TRUE 1
//#endif
//
//#ifndef FALSE
//#	define FALSE 0
//#endif
//
//#ifndef NULL
//#	define NULL (void*)0
//#endif

//#if _MSC_VER >= 1400 // for vc8
//#	define HashCompare stdext::hash_compare
//#	define HashMap stdext::hash_map
//#	define oo_fopen fopen_s
//#	define oo_strcpy strcpy_s
//#define NC_HIT_TEST_RETURN LRESULT // mfc: ON_WM_NCHITTEST
//#else
//#	define HashCompare std::hash_compare
//#	define HashMap std::hash_map
//#	define oo_fopen fopen
//#	define oo_strcpy strcpy
//#define NC_HIT_TEST_RETURN UINT // mfc: ON_WM_NCHITTEST
//#endif

OO_NAMESPACE_END

#endif // #ifndef OO_TYPEDEF_H_