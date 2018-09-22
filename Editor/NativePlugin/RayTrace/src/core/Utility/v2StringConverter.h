// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_STRING_CONVERTER_H_
#define OO_STRING_CONVERTER_H_

#include "..\Common\v2ObjStd.h"
#include "v2String.h"
//#include "v2Uuid.h"

OO_NAMESPACE_BEGIN

//  兼容旧的
#define StringConverter cString
//#define OO_API

namespace cString
{
	/*
	Why not use following generic method to implement string converter?
	VC6 encounters compiling error when use template method in a non-templated class.
	but VC7 does it right.

	/// string converting
	template <class T>
	OO_API String toString(const T &val)
	{
	std::stringstream stream(std::ios_base::out);
	stream << val;
	return stream.str();
	}
	*/

	inline String toString(const String &val) { return val; } // some template method needs this.
	/** 实数到字符串转换。

		注意：实数到字符串转换内部使用了_snprintf(.."%f"..)，小数点后面只保存6位，
		对小数位数要求严格的地方，请注意浮点到字符串来回转换的舍入误差。引擎保存XML
		文件使用的也是这个函数。
	*/
	OO_API String toString(const Real &val);
	OO_API String toString(const Dword &val);
	OO_API String toString(const Word &val);
	OO_API String toString(const Byte &val);
	OO_API String toString(const Uint &val);
	OO_API String toString(const int &val);
	inline String toString(const bool &val) { return val ? "1" : "0"; }

	//template <class T>
	//inline String toString(const Size<T> &val)
	//{
	//	return toString(val.cx) + " " + toString(val.cy);
	//};

	//template <class T>
	//inline String toString(const Rect<T> &val)
	//{
	//	return toString(val.minX) + " " + toString(val.minY) + " " + toString(val.maxX) + " " + toString(val.maxY);
	//};

	//template <class T>
	//inline String toString(const Point<T> &val)
	//{
	//	return toString(val.x) + " " + toString(val.y);
	//};


	//OO_API String toString(const PixelFormat &val);

	// generic conversion, useful for variant enum.
	template<class T>
		inline String toString(const T &val) {
			return toString((int)val);
		}

		OO_API inline String toString(const void* data, int size);

		// inlined
		OO_API inline String& parse(const String& str, String &val)
		{
			val = str;
			return val;
		}

		OO_API inline Real& parse(const String& str, Real &val)
		{ 
			val = (Real)atof(str.c_str());
			return val;
		}

		OO_API inline Dword& parse(const String& str, Dword &val)
		{ 
			//old: val = (Dword)atol(str.c_str()); 
			val = strtoul(str.c_str(), 0, 0); // base为0表示支持"0xff123456"这样的转换
			return val;
		}

		OO_API inline Word& parse(const String& str, Word &val)
		{
			val = (Word)atol(str.c_str());
			return val;
		}

		OO_API inline Byte& parse(const String& str, Byte &val)
		{
			val = (Byte)atol(str.c_str());
			return val;
		}

		OO_API inline Uint& parse(const String& str, Uint &val)
		{
			//old: val = (Uint)atol(str.c_str());
			val = (Uint)strtoul(str.c_str(), 0, 10); // base不为0，不支持"0xff123456"这样的转换
			return val;
		}

		OO_API inline int& parse(const String& str, int &val)
		{
			val = (int)atoi(str.c_str());
			return val;
		}

		OO_API inline bool& parse(const String& str, bool &val)
		{
			val = atol(str.c_str()) != 0;
			return val;
		}

//#if OO_PLATFORM == OO_PLATFORM_WIN32
//		OO_API Matrix4& parse(const String& str, Matrix4 &val);
//		OO_API Quaternion& parse(const String& str, Quaternion &val);
//		OO_API Plane& parse(const String& str, Plane &val);
//		inline Uuid& parse(const String& str, Uuid &val) {
//			val.fromString(str);
//			return val;
//		}
//#endif
//
//		// non-inlined
//#if OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//		// ios(mac?) 上不能正常特化Graphics工程中的Color，可能是因为xcode支持这种跨工程的特化有问题。
//		// 这里申明，但在Graphics.Color中实现。
//		OO_API String toString(const Color &val);
//		OO_API Color& parse(const String& str, Color &val);
//#endif
//	
//		OO_API Vector3& parse(const String& str, Vector3 &val);
//
//		template <class T>
//		inline Size<T>& parse(const String& str, Size<T> &val)
//		{
//			StringVector vec = uString::split(str);
//
//			parse(vec[0], val.cx);
//			parse(vec[1], val.cy);
//
//			return val;
//		}
//
//		template <class T>
//		inline Rect<T>& parse(const String& str, Rect<T> &val)
//		{
//			StringVector vec = uString::split(str);
//
//			parse(vec[0], val.minX);
//			parse(vec[1], val.minY);
//			parse(vec[2], val.maxX);
//			parse(vec[3], val.maxY);
//
//			return val;
//		}
//
//		template <class T>
//		inline Point<T>& parse(const String& str, Point<T> &val)
//		{
//			StringVector vec = uString::split(str);
//
//			parse(vec[0], val.x);
//			parse(vec[1], val.y);
//
//			return val;
//		}
//
//		//OO_API PixelFormat& parse(const String& str, PixelFormat &val);
//
//		// generic conversion, useful for variant enum.
//		template<class T>
//			inline T& parse(const String& str, const T &val) {
//				return (T&)parse(str, (int&)val);
//			}
//
//			void* parse(const String& str, void *data, int size);
//
//			/*
//			// Type-String conversion
//			//template <typename T> OO_API const String& typeToName(); // no implemention.
//			template <typename T> OO_API const String& typeToName() { const OO_API String name("enum"); return name; }
//
//			#define DEF_TYPE_NAME(TYPE) TEMPL_SPEC OO_API const String& typeToName< TYPE >() { const OO_API String name(#TYPE); return name; }
//
//			DEF_TYPE_NAME(String)
//			DEF_TYPE_NAME(Real)
//			DEF_TYPE_NAME(Dword)
//			DEF_TYPE_NAME(Word)
//			DEF_TYPE_NAME(Byte)
//			DEF_TYPE_NAME(Uint)
//			DEF_TYPE_NAME(int)
//			DEF_TYPE_NAME(bool)
//
//			DEF_TYPE_NAME(Color)
//			DEF_TYPE_NAME(Vector3)
//			DEF_TYPE_NAME(Matrix4)
//
//			DEF_TYPE_NAME(Size<int>)
//
//			#undef DEF_TYPE_NAME
//			*/
//
		// argb - 只能是小写字母，如"ff808080"
		OO_API Dword toDwordColor(const String& argb);
		// 返回的十六进制字符串时小写的。
		OO_API String fromDwordColor(Dword argb);

};

///////////////////////////////////////////////////////////////////////////////
// inlined functions

OO_NAMESPACE_END

#endif // #ifndef OO_STRING_CONVERTER_H_