// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
//#include "v2Standard.h"
#include "v2String.h"

// @v2 remarked
// #include "v2Exception.h"

#ifdef WIN32
// Windows Platform
#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#	include "windows.h"
//#	include <windowsx.h>
#endif
#include <stdlib.h>

OO_NAMESPACE_BEGIN

// static...不支持多线程，删除。-wb,10/2/25
//static char szBuffer[4097];

// const ooBasicString::size_type ooBasicString::npos = (ooBasicString::size_type)(-1);
//////////////////////////////////////////////////////////////////////////
// uString

String uString::lowercase(const String &str)
{
	if (str.empty())
	{
		return "";
	}
	String out(str.size(), '\0');
	//std::transform(
	//	str.begin(),
	//	str.end(),
	//	out.begin(),
	//	static_cast<int(*)(int)>(::tolower) );
	const char* inBuf = str.c_str();
	char *outBuf = &(*out.begin());
	bool bGBKH = false;	//是否读到了一个gbk编码的高位字节
	for(unsigned int i = 0;i<str.size();i++){
		const unsigned char &c = inBuf[i];
		if(!bGBKH && c >='A' && c <= 'Z'){
			outBuf[i] = c + ('a' - 'A');
		}
		else{
			outBuf[i] = c;
		}
		if(bGBKH){
			bGBKH = false;
		}
		else if(c > 0x7f){
			bGBKH = true;
		}
	}
	return out;
}

String uString::uppercase(const String &str)
{
	if (str.empty())
	{
		return "";
	}
	String out(str.size(), '\0');
	//std::transform(
	//	str.begin(),
	//	str.end(),
	//	out.begin(),
	//	static_cast<int(*)(int)>(::toupper) );
	const char* inBuf = str.c_str();
	char *outBuf = &(*out.begin());
	bool bGBKH = false;	//是否读到了一个gbk编码的高位字节
	for(unsigned int i = 0;i<str.size();i++){
		const unsigned char &c = inBuf[i];
		if(!bGBKH && c >='a' && c <= 'z'){
			outBuf[i] = c - ('a' - 'A');
		}
		else{
			outBuf[i] = c;
		}
		if(bGBKH){
			bGBKH = false;
		}
		else if(c > 0x7f){
			bGBKH = true;
		}
	}
	return out;
}

String uString::replace(const String &str, const String& olds, const String& news)
{
	ooBreakAssert(!olds.empty());

	if (olds == news)
		return str;

	String out = str;
	int pos;
	pos = str.find(olds, 0);
	while (pos != String::npos) {
		out.replace(pos, olds.size(), news);
		pos = out.find(olds, pos+news.size());
	}
	return out;
}

String uString::replace(const String &str, char oldc, char newc)
{
	if (str.empty())
	{
		return str;
	}
	String out = str;
	char *buf = &(*out.begin());
	while (*buf) {
		if (*buf == oldc)
			*buf = newc;
		buf++;
	}
	return out;
}

StringVector uString::split(const String &str, const String& delims)
{
	StringVector out;

	size_t pos, start, end;
	start = 0;

	do {
		pos = str.find_first_not_of(delims, start);
		if (pos != String::npos) {
			end = str.find_first_of(delims, pos);
			if (end == String::npos)
				end = str.size();

			//std::string sss = substr(pos, end-pos);
			out.push_back(str.substr(pos, end-pos));

			start = end;
		}
	} while (pos != String::npos);

	return out;
}

String uString::format(const char* formats, ...)
{
	char szBuffer[4096];

	va_list list;
	va_start( list, formats);

	vsprintf( szBuffer, formats, list );
	va_end( list );

	return szBuffer;
}

WString uString::format(const wchar_t* formats, ...)
{
	wchar_t szBuffer[4096];

	va_list list;
	va_start( list, formats);

#ifdef WIN32
	vswprintf( szBuffer, formats, list );
#else
	vswprintf( szBuffer, 4095, formats, list );
#endif
	
	va_end( list );

	return szBuffer;
}

//typedef int WINAPI FUN_MultiByteToWideChar(
//	UINT CodePage,         // code page
//	DWORD dwFlags,         // character-type options
//	LPCSTR lpMultiByteStr, // string to map
//	int cbMultiByte,       // number of bytes in string
//	LPWSTR lpWideCharStr,  // wide-character buffer
//	int cchWideChar        // size of buffer
//	);
//OO_API FUN_MultiByteToWideChar *funMultiByteToWideChar = 0; // function wrapped by MSUL for taking unicode worked in win98/me platform.

WString uString::toWideString(const String& str)
{
#ifdef WIN32
	int mbcsLen = str.size();
	int wideLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), mbcsLen, NULL, NULL);  // 返回的是不包括'\0'的尺寸
	WString wstr(wideLen, wchar_t(0)); // stl自己会把最后的'\0'加在nLen+1处
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), mbcsLen, const_cast<wchar_t*>(wstr.c_str()), wideLen);
	return wstr;
#else
	int mbcsLen = str.size();
	size_t wideLen = mbstowcs(NULL, str.c_str(), 0); // 返回的是不包括'\0'的尺寸
	WString wstr(wideLen, wchar_t(0));
	mbstowcs(const_cast<wchar_t*>(wstr.c_str()), str.c_str(), wideLen+1);
	return wstr;
#endif

	//WString wstr;
	//wstr.reserve(str.size());
	//for (int i = 0; i < str.size();) {
	//	Byte c0 = (Byte)str.at(i++);
	//	wchar_t w;
	//	if (c0 <= '~')
	//		w = (wchar_t)c0;
	//	else {
	//		if (i < str.size()) {
	//			Byte c1 = str.at(i++);
	//			w = (wchar_t)((c0 << 8) | c1);
	//		}
	//		else
	//			break;
	//	}

	//	wstr.push_back(w);			
	//}
	//return wstr;

//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	if (!funMultiByteToWideChar) { // without MSUL support.
//		int nLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, NULL);
//
//		WString wstr(nLen - 1, wchar_t(0));
//
//		MultiByteToWideChar(CP_ACP, 0, 
//			str.c_str(), -1, const_cast<wchar_t*>(wstr.c_str()), nLen);
//		return wstr;
//	}
//	else { // function wrapped with MSUL.
//		int nLen = (*funMultiByteToWideChar)(CP_ACP, 0, str.c_str(), -1, NULL, NULL);
//
//		WString wstr(nLen - 1, wchar_t(0));
//
//		(*funMultiByteToWideChar)(CP_ACP, 0, 
//			str.c_str(), -1, const_cast<wchar_t*>(wstr.c_str()), nLen);
//
//		return wstr;
//	}
//#else
//#	error "not yet support"
//#endif
}

String uString::toMultiByteString(const WString& wstr)
{
#ifdef WIN32
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, NULL, NULL, NULL);
#else
	size_t nLen = wcstombs(NULL, wstr.c_str(), 0);
#endif

	String str(nLen - 1, char(0));

#ifdef WIN32
	::WideCharToMultiByte(CP_ACP, 0, 
		wstr.c_str(), -1, const_cast<char*>(str.c_str()), nLen, NULL, NULL);
#else
	wcstombs(const_cast<char*>(str.c_str()), wstr.c_str(), nLen+1);
#endif

	return str;
}

String uString::left(const String &str, int n)
{
	if(int(str.size())<n){
		n = str.size();
	}
	return String(str.begin(), str.begin() + n);
}

String uString::right(const String &str, int n)
{
	if(int(str.size())<n){
		n = str.size();
	}
	return String(str.begin() + str.size() - n, str.end());
}

oo::String uString::getFileExtension( const String& str )
{
	StringVector sv = split(str,".");
	if(sv.size() > 1){
		return uppercase(sv.back());
	}
	return "";
}

OO_NAMESPACE_END