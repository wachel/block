// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#pragma once

#include <assert.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
//#include <xutility>
#include <algorithm>
#ifdef WIN32
#	include <strstream>
#else
#	include <strstream>
#endif
#include <sstream>
#include <fstream>
#include <iostream>

// Origo engine release type.
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//#	if OO_DYNAMIC_LINKAGE == 0
//#		define OBJ_API
//#	elif defined( OBJECT_EXPORTS)
//#		define OBJ_API __declspec(dllexport)
//#	else
//#		define OBJ_API __declspec(dllimport)
//#	endif
//#else
#define OBJ_API
#define OO_API
//#endif

#pragma warning ( disable : 4996 )
#pragma warning ( disable : 4244 )

#define OO_NAMESPACE_BEGIN namespace oo {
#define OO_NAMESPACE_END }

namespace oo{}

#define ooAssert(A) assert(A)
#define ooAssertDesc(A, DESC) assert(A&&DESC)
#ifdef _DEBUG
#	define ooVerify(A) ooAssert(A)//do { if (!(A)) ooThrow(OError("ooVerify failed;")); } while(false)
#else
#	define ooVerify(A) ((void)(A))
#endif

//old: #define ooBreak() __asm { int 3 }
#define ooBreak() __debugbreak() // __debugbreak可以在64位下用

// old: #define ooBreakAssert(A) do { if (!(A)) ooBreak(); } while(false)
// @todo ooBreakAssert -> ooAssert
#define ooBreakAssert(A) ooAssert(A)
#define ooGuard(s)
//#define ooAssert assert
#define ooThrow(E) assert(0)
#define COMPILER_ASSERT(x) typedef int COMPILER_ASSERT_[!!(x)]

#define safeRelease(p)      { if(p) { (p)->release(); (p)=NULL; } }
#undef min
#undef max
//typedef std::string String;
#include "v2TypeDef.h"

//typedef unsigned short int Word;
//typedef unsigned char Byte;
//typedef unsigned int Uint;

const static bool LM_Core = false;
typedef int LOG_BOOL_TYPE;
void logWarning(LOG_BOOL_TYPE bOpen, const char *formats, ...);
//void logWarning(LOG_BOOL_TYPE bOpen,const std::string &formats);
void logError(LOG_BOOL_TYPE bOpen, const char *formats, ...);
//void logError(LOG_BOOL_TYPE bOpen,const String &formats);
void logInfo(LOG_BOOL_TYPE bOpen, const char *formats, ...);
void logNotice(LOG_BOOL_TYPE bOpen, const char *formats, ...);
