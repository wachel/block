// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_CONFIG_H_
#define OO_CONFIG_H_

// Origo engine release type
// 1 as a dll
// 0 as a static library
#define OO_DYNAMIC_LINKAGE 0

// Release Assert switch
// 1 enable release assert, oo will throw exceptions on release builds.
// 0 disable release assert
#define OO_RELEASE_ASSERT 0

// !!! @BUG mmgr conflict with MFC in RELEASE.
#ifdef _DEBUG
	// Memory fragment management switch
	// 1 enable memory management
	// 0 disable memory management
	#define OO_MEMORY_FRAGMENT_MANAGEMENT 0

	// Memory leaking detection
	#define OO_MEMORY_TRACE 0

	// Override global new/delete.	
	#define OO_MEMORY_TRACE_GLOBAL 0

#else
	#define OO_MEMORY_FRAGMENT_MANAGEMENT 0

	// Memory leaking detection
	#define OO_MEMORY_TRACE 0

	// @bug app crashed when exit.
	// 05/3/19晚: 重载new/delete有退出错。
	// 05/12/27晚: 错误依然。。。，见NOTE.txt。
	// 09/11晚：去除Python后错误变成：“dllmodul.cpp: AfxTermExtensionModule(controlDLL, TRUE);”
	#define OO_MEMORY_TRACE_GLOBAL 0
#endif

// Function calling stack guarding switch
// 1 enable stack guarding, calling stack is shown when an exception occurs.
// 0 disable stack guarding
#define OO_CALLING_STACK_GUARDING 0

#define OO_MAX_TEXTURE_COORD_SETS 6

#define OO_DISABLE_MUTISTREAM 0

#define OO_USE_STLPORT	0

// Enable/Disable profile for performance tuning.
#define OO_ENABLE_PROFILE 1

#define OO_USE_OTL 0


#define OO_PLATFORM_WIN32 1 // 从1开始这样好点，xcode会认为未定义的宏默认为0。
#define OO_PLATFORM_LINUX 2
#define OO_PLATFORM_IOS 3
#define OO_PLATFORM_MAC 4
#define OO_PLATFORM_ANDROID 5

// Platform recognition.
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( WIN32 )
#	define OO_PLATFORM OO_PLATFORM_WIN32
#elif defined(__APPLE__) // @fixme maybe OSX or iOS
#   include <TargetConditionals.h>
#   if TARGET_IPHONE_SIMULATOR == 1
/* Apple iOS in Xcode simulator */
#       define OO_PLATFORM OO_PLATFORM_IOS
#   elif TARGET_OS_IPHONE == 1
/* Apple iOS on iPhone, iPad, etc. */
#       define OO_PLATFORM OO_PLATFORM_IOS
#   elif TARGET_OS_MAC == 1
/* Apple Mac OSX */
#       define OO_PLATFORM OO_PLATFORM_MAC
#   else
/* Unknown Apple OS */
#       error "Unknown Apple OS!"
#   endif
#elif defined(__ANDROID__)
#	define OO_PLATFORM OO_PLATFORM_ANDROID
#else
#	error "Couldn't recognise platform!"
#endif

#define _GENERAL_CPU_

#endif