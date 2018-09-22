// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
//#include "v2Standard.h"
#include "v2StringConverter.h"
//#include "v2Color.h"
//#include "v2Vector3.h"
//#include "v2Matrix4.h"
//#include "v2Quaternion.h"

#include "..\Common\v2STL.h"
#include "..\Common\v2ObjStd.h"

OO_NAMESPACE_BEGIN

static const int STR_BUF_SIZE = 1024;
static char STR_BUF[STR_BUF_SIZE];

#ifndef WIN32
#define SNPRINTF snprintf 
#else
#define SNPRINTF _snprintf_s
#endif



namespace cString
{

String toString(const Real &val)
{
	// 注意：stringstream 会受 locale 影响，可能导致千分符的出现。
	// toString 和 parse 必须配对，不能 toString 用 stringstream 而 parase 用 atof...。
	// 见：http://www.codeguru.com/forum/archive/index.php/t-402370.html
	// 另外 stringstream 性能不高，见：http://www.codeproject.com/KB/string/string_conversions.aspx
	
	// old: otl::stringstream stream(otl::stringstream::out);
	// stream << val;
	// return stream.str();

	SNPRINTF(STR_BUF, STR_BUF_SIZE, "%f", val);
	return STR_BUF;
}

String toString(const Dword &val)
{
	//otl::stringstream stream(otl::stringstream::out);
	//stream << val;
	//return stream.str();

	SNPRINTF(STR_BUF, STR_BUF_SIZE, "%lu", val);
	return STR_BUF;
}

String toString(const Word &val)
{
	//otl::stringstream stream(otl::stringstream::out);
	//stream << val;
	//return stream.str();

	SNPRINTF(STR_BUF, STR_BUF_SIZE, "%u", val);
	return STR_BUF;
}

String toString(const Byte &val)
{
	//otl::stringstream stream(otl::stringstream::out);
	//stream << val;
	//return stream.str();

	SNPRINTF(STR_BUF, STR_BUF_SIZE, "%u", val);
	return STR_BUF;
}

String toString(const Uint &val)
{
	//otl::stringstream stream(otl::stringstream::out);
	//stream << val;
	//return stream.str();

	SNPRINTF(STR_BUF, STR_BUF_SIZE, "%u", val);
	return STR_BUF;
}

String toString(const int &val)
{
	//otl::stringstream stream(otl::stringstream::out);
	//stream << val;
	//return stream.str();

	SNPRINTF(STR_BUF, STR_BUF_SIZE, "%d", val);
	return STR_BUF;
}

/*
String toString(const Color &val)
{
	otl::stringstream stream(otl::stringstream::out);
	//stream << (int)(val.r*255) << " " << (int)(val.g*255) << " " << (int)(val.b*255) << " " << (int)(val.a*255);
	stream << val.r << " " << val.g << " " << val.b << " " << val.a;
	return stream.str();
}
*/

//String toString(const Vector3 &val)
//{
//	//otl::stringstream stream(otl::stringstream::out);
//	//stream << val.x << " " << val.y << " " << val.z;
//	//return stream.str();
//
//	_snprintf(STR_BUF, STR_BUF_SIZE, "%f %f %f", val.x, val.y, val.z);
//	return STR_BUF;
//}
//
//String toString(const Matrix4 &val)
//{
//	//otl::stringstream stream(otl::stringstream::out);
//	//stream << val._11 << " " << val._12 << " " << val._13 << " " << val._14 << " " 
//	//	<< val._21 << " " << val._22 << " " << val._23 << " " << val._24 << " "
//	//	<< val._31 << " " << val._32 << " " << val._33 << " " << val._34 << " " 		
//	//	<< val._41 << " " << val._42 << " " << val._43 << " " << val._44;
//	//return stream.str();
//
//	_snprintf(STR_BUF, STR_BUF_SIZE, 
//		"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
//		val._11, val._12, val._13, val._14,
//		val._21, val._22, val._23, val._24,
//		val._31, val._32, val._33, val._34,
//		val._41, val._42, val._43, val._44);
//	return STR_BUF;
//}
//
//String toString(const Quaternion &val)
//{
//	//otl::stringstream stream(otl::stringstream::out);
//	//stream << val.x << " " << val.y << " " << val.z << " " << val.w;
//	//return stream.str();
//	
//	_snprintf(STR_BUF, STR_BUF_SIZE, "%f %f %f %f", val.x, val.y, val.z, val.w);
//	return STR_BUF;
//}
//
//String toString(const Plane &val)
//{
//	//otl::stringstream stream(otl::stringstream::out);
//	//stream << val.normal.x << " " << val.normal.y << " " << val.normal.z << " " << val.dist;
//	//return stream.str();
//
//
//	_snprintf(STR_BUF, STR_BUF_SIZE, "%f %f %f %f", val.normal.x, val.normal.y, val.normal.z, val.dist);
//	return STR_BUF;
//}

/*
String toString(const PixelFormat &val)
{
	if (val == PF_R8G8B8)
		return "PF_R8G8B8";
	else if (val == PF_A8R8G8B8)
		return "PF_A8R8G8B8";
	else if (val == PF_Unknown)
		return "PF_Unknown";
	else if (val == PF_DXT1)
		return "PF_DXT1";
	else if (val == PF_DXT3)
		return "PF_DXT3";
	else if (val == PF_DXT5)
		return "PF_DXT5";
	else {
		ooBreakAssert(false);
		return "BAD PixelFormat:" + toString((int)val);
	}
}


Color& parse(const String& str, Color &val)
{
	StringVector vec = str.split();
	ooBreakAssert(vec.size() == 4);

	parse(vec[0], val.r);
	parse(vec[1], val.g);
	parse(vec[2], val.b);
	parse(vec[3], val.a);

	// convert to float color.
	//val.r /= 255.f;
	//val.g /= 255.f;
	//val.b /= 255.f;
	//val.a /= 255.f;

	return val;
}
*/

//Vector3& parse(const String& str, Vector3 &val)
//{
//	StringVector vec = uString::split(str);
//	ooBreakAssert(vec.size() == 3);
//
//	parse(vec[0], val.x);
//	parse(vec[1], val.y);
//	parse(vec[2], val.z);
//
//	return val;
//}
//
//Matrix4& parse(const String& str, Matrix4 &val)
//{
//	StringVector vec = uString::split(str);
//	ooBreakAssert(vec.size() == 16);
//
//	for(int i = 0; i < 4; ++i)
//	{
//		for(int j = 0; j < 4; ++j)
//		{
//			parse(vec[i*4 + j], val.m[i][j]);
//		}
//	}
//
//	return val;
//}
//
//Quaternion& parse(const String& str, Quaternion &val)
//{
//	StringVector vec = uString::split(str);
//	ooBreakAssert(vec.size() == 4);
//
//	parse(vec[0], val.x);
//	parse(vec[1], val.y);
//	parse(vec[2], val.z);
//	parse(vec[3], val.w);
//
//	return val;
//}
//
//Plane& parse(const String& str, Plane &val)
//{
//	StringVector vec = uString::split(str);
//	ooBreakAssert(vec.size() == 4);
//
//	parse(vec[0], val.normal.x);
//	parse(vec[1], val.normal.y);
//	parse(vec[2], val.normal.z);
//	parse(vec[3], val.dist);
//
//	return val;
//}


/*
PixelFormat& parse(const String& str, PixelFormat &val)
{
	if (str == "PF_R8G8B8")
		val = PF_R8G8B8;
	else if (str == "PF_A8R8G8B8")
		val =  PF_A8R8G8B8;
	else if (str == "PF_Unknown")
		val = PF_Unknown;
	else
		ooBreakAssert(false);

	return val;
}
*/

static char _hexToChar[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

inline Byte _charToHex(char c)
{
	if (c <= '9')
		return c - '0';
	else
		return 10 + (c - 'a');
};

String toString(const void* data, int size)
{
	ooBreakAssert((data || size == 0) && size >= 0);

	String out;
	out.resize(size*2 + 1);// one byte - two characters

	char *to = (char*)out.c_str();
	Byte *from = (Byte*)data;

	for (int i = 0; i < size; ++i) {
		*(to++) = _hexToChar[(*from) >> 4]; // 4 high bits.
		*(to++) = _hexToChar[(*from) & 0x0f]; // 4 low bits.

        ++from;
	}

	return out;
}

void* parse(const String& str, void *data, int size)
{
	ooBreakAssert(data && str.size() == size*2); // one byte - two characters

	const char* from = str.c_str();
    Byte *to = (Byte*)data;

	for (int i = 0; i < size; ++i) {
        *to = (_charToHex(*from) << 4); // 4 high bits.
		++from;

		*to = (*to) | _charToHex(*from); // 4 low bits.
		++from;

		++to;
	}
	
	return data;
}


Dword toDwordColor(const String &argb)
{
	if (argb.size() != 8)
		return 0; // error

	Dword out = 0;

	const char* from = argb.c_str();
	Byte *to = (Byte*)(&out) + 3;

	for (int i = 0; i < 4; i++) {
		*to = (_charToHex(*from) << 4); // 4 high bits
		++ from;

		*to = (*to) | _charToHex(*from); // 4 low bits.
		++from;

		--to; // 显示的和存储的高低位相反。
	}

	return out;
}

String fromDwordColor(Dword argb)
{
	String out;
	out.resize(4*2+1);// one byte - two characters

	char *to = (char*)out.c_str();
	Byte *from = (Byte*)(&argb) + 3;

	for (int i = 0; i < 4; ++i) {
		*(to++) = _hexToChar[(*from) >> 4]; // 4 high bits.
		*(to++) = _hexToChar[(*from) & 0x0f]; // 4 low bits.

		--from; // 显示的和存储的高低位相反。
	}

	return out;
}

	
////////////////////////////
//	//////////////////
//	// Float
//	String toString(const Float2 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%f %f", val[0], val[1]);
//		return STR_BUF;
//	}
//	Float2& parse(const String& str, Float2 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 2) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
//	String toString(const Float3 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%f %f %f", val[0], val[1], val[2]);
//		return STR_BUF;
//	}
//	Float3& parse(const String& str, Float3 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 3) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
//	String toString(const Float4 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%f %f %f %f", val[0], val[1], val[2], val[3]);
//		return STR_BUF;
//	}
//	Float4& parse(const String& str, Float4 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 4) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
//	String toString(const Float5 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%f %f %f %f %f", val[0], val[1], val[2], val[3], val[4]);
//		return STR_BUF;
//	}
//	Float5& parse(const String& str, Float5 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 5) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//		return val;
//	}
//	String toString(const Float6 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%f %f %f %f %f %f", val[0], val[1], val[2], val[3], val[4], val[5]);
//		return STR_BUF;
//	}
//	Float6& parse(const String& str, Float6 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 6) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
//	
//	//////////////////
//	// Int
//	String toString(const Int2 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%d %d", val[0], val[1]);
//		return STR_BUF;
//	}
//	Int2& parse(const String& str, Int2 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 2) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
//	String toString(const Int3 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%d %d %d", val[0], val[1], val[2]);
//		return STR_BUF;
//	}
//	Int3& parse(const String& str, Int3 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 3) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
//	String toString(const Int4 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%d %d %d %d", val[0], val[1], val[2], val[3]);
//		return STR_BUF;
//	}
//	Int4& parse(const String& str, Int4 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 4) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
//	String toString(const Int5 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%d %d %d %d %d", val[0], val[1], val[2], val[3], val[4]);
//		return STR_BUF;
//	}
//	Int5& parse(const String& str, Int5 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 5) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
//	String toString(const Int6 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%d %d %d %d %d %d", val[0], val[1], val[2], val[3], val[4], val[5]);
//		return STR_BUF;
//	}
//	Int6& parse(const String& str, Int6 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 6) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
//	String toString(const Int7 &val)
//	{
//		SNPRINTF(STR_BUF, STR_BUF_SIZE, "%d %d %d %d %d %d %d", val[0], val[1], val[2], val[3], val[4], val[5], val[6]);
//		return STR_BUF;
//	}
//	Int7& parse(const String& str, Int7 &val)
//	{
//		StringVector vec = uString::split(str);
//		if (vec.size() > 7) {
//			//logError("Parameters error!");
//			return val;
//		}
//		for (int i = 0; i < vec.size(); ++i)
//			parse(vec[i], val[i]);
//		return val;
//	}
}

OO_NAMESPACE_END