// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_STRING_H_
#define OO_STRING_H_

#if OO_USE_OTL
#endif

#include "..\Common\v2ObjStd.h"
#include "..\Common\v2STL.h"



OO_NAMESPACE_BEGIN

#if OO_USE_OTL
	typedef std::basic_string<char, std::char_traits<char>, pool_allocator<char> > String;
	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>,pool_allocator<wchar_t> > WString;
#else

	typedef std::string String;
	typedef std::wstring WString;
	typedef std::basic_string<unsigned short int, std::char_traits<unsigned short int>, std::allocator<unsigned short> > WString16;
#endif

//typedef std::string String;

#ifdef WIN32
	//typedef HashCompare<String> StringHash;
#else
// 	template<class A>
// 	struct linux_hash_compare
// 	{
// 		size_t operator()(const A & a)const{
// 			return a.c_str();
// 		}
// 		bool operator()(const A & a1, const A & a2)const{
// 			return   a1.a < a2.a;
// 		}
// 	};
// 	typedef linux_hash_compare<String> StringHash;
#endif
typedef OArray<String>::Type StringVector;

//////////////////////////////////////////////////////////////////////////

class OO_API uString
{
public:
	static String lowercase(const String &str);
	static String uppercase(const String &str);

	static String replace(const String &str, const String& olds, const String& news);

	static String getFileExtension(const String& str);
	// 替换单个字符用的专用版本。
	static String replace(const String &str, char oldc, char newc);

	static StringVector split(const String &str, const String& delims = "\t\n\r ");

	static String format(const char* formats, ...);
	static WString format(const wchar_t* formats, ...);

	/// Converts to wide string.
	static WString toWideString(const String& str);

	//add by wanglei
	static String toMultiByteString(const WString& wstr);

	/** Extracts the left part of a string. 
	@remarks 
		if the n exceeds the string length, then the entire is extracted.
	*/
	static String left(const String &str, int n);

	/** Extracts the right part of a string. 
	@remarks
		if the n exceeds the string length, then the entire is extracted.
	*/
	static String right(const String &str, int n);
	
	static inline int stricmp(const char *s1, const char *s2) {
#ifdef WIN32
		return ::_stricmp(s1, s2);
#else	// xcode
		return ::strcasecmp(s1, s2);
#endif
	}
	
	static inline int strnicmp(const char *s1, const char *s2, int n) {
#ifdef WIN32
		return ::_strnicmp(s1, s2, (size_t)n);
#else	// xcode
		return ::strncasecmp(s1, s2, (size_t)n);
#endif
	}

};

OO_NAMESPACE_END

#endif // !defined(AFX_OSSTRING_H__B0FD6847_FFB7_4842_B6F0_ABCFD6C6A5C4__INCLUDED_)