// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_MATH_H_
#define OO_MATH_H_


#include "..\Common\v2ObjStd.h"
#include "v2AsmMath.h"
#include <cmath>
#undef isnan

#pragma warning ( push )
#pragma warning ( disable : 4035 )	

// from "fastmath.cpp"
extern OO_API unsigned int fast_sqrt_table[0x10000];  // declare table of square roots 

OO_NAMESPACE_BEGIN

namespace Math
{
	extern OO_API Real PI;
	extern OO_API Real TWO_PI;
	extern OO_API Real EPSILON;
	extern OO_API Real LOWEPSILON;

	// numeric limit definition.
	// it is very useful to deal with float overflow such as REAL_MAX*some_value.
	extern OO_API Real REAL_MIN; ///< negative, equal to -REAL_MAX
	extern OO_API Real REAL_MAX; ///< positive, equal to -REAL_MIN

	/// Initialization, muse be called before other methods are used.
	OO_API void initialize();
	OO_API void destroy();

	static inline Real rAbs(Real val);
	static inline Real rSqrt(Real val);
	static inline Real rFastSqrt(Real val); // look-up sqrt.
	static inline Real rInvSqrt(Real val);
	static inline Real rSin(Real val);
	static inline Real rAsin(Real val);
	static inline Real rCos(Real val);
	static inline Real rAcos(Real val);
	static inline Real rTan(Real val);
	static inline Real rAtan(Real val);
	static inline Real rModf(Real val, Real* intPart = 0);
	static inline Real rFMod(Real val, Real base);
	static inline Real rPow (Real fBase, Real fExponent);

	
	// Fuzzy compare operations.
	// Notes that IsEqual(a, b) && IsEqual(b, c) does not imply IsEqual(a, c), 
	// although that will rarely be the case.
	static inline bool rIsEqual(const Real &a, const Real &b) { return rAbs(a - b) < EPSILON * (rAbs(a) + 1); }
	static inline bool rIsGreat(const Real &a, const Real &b) { return a > b + EPSILON * (rAbs(a) + 1); }
	static inline bool rIsGreatEqual(const Real &a, const Real &b) { return a > b - EPSILON * (rAbs(a) + 1); }
	static inline bool rIsLess(const Real &a, const Real &b) { return a < b - EPSILON * (rAbs(a) + 1); }
	static inline bool rIsLessEqual(const Real &a, const Real &b) { return a < b + EPSILON * (rAbs(a) + 1); }
	
	template<class T>
	static inline T toRadian(T degree) { return (T)(degree * PI / 180.); }

	template<class T>
	static inline T toDegree(T radian) { return (T)(radian * 180. / PI); }

	
	/** Checks a value if is power of 2.
		@note 0 is not a power of 2.
	*/
	template <class T>
	static inline bool isPower2( T n ) { return ( n && ((n & (n-1)) == 0) ); }

	/// Return the shift value for the passed power-of-two dimension.
	static inline int getPow2Shift( int n )
	{
//#if OO_COMPILER == OO_COMPILER_MSVC
//#	ifndef _M_X64
//		__asm
//		{
//			mov edx, [n]
//				bsr eax, edx
//		}
//#	else
//		OO_UNIMPLEMENTED_METHOD
//			return 0;
//#	endif
//#else
//#	error "not yet finish"
//#endif
		return 0;
	}
	
	/// Return the next power of two that contains the passed in number.
	template <typename T>
	static inline T makePower2Up( T n )
	{
		if ( Math::isPower2( n ) )
			return ( n );
		return ( T( (n == 0) ? 1 : (2 << Math::getPow2Shift( n )) ) );
	}
	
	/** Return the previous power of two that contains the passed in number.
		@note this is useful for finding the most significant bit.
	*/
	template <typename T>
	static inline T makePower2Down( T n )
	{
		if ( isPower2( n ) )
			return ( n );
		return ( T( (n == 0) ? 0 : (1 << getPow2Shift(n)) ) );
	}

	// variable-alignment
	// note that "alignment" should be a power of 2 for these to work
	template <typename T> static inline bool isAligned   ( T  value, size_t alignment)  {  return ( !(value & (alignment - 1)) );  }
	template <typename T> static inline T    getAlignUp  ( T  value, size_t alignment)  {  return ( (value + alignment - 1) & ~(alignment - 1) );  }
	template <typename T> static inline T    getAlignDown( T  value, size_t alignment)  {  return ( value & ~(alignment - 1) );  }
	template <typename T> static inline void alignUp     ( T& value, size_t alignment)  {  value = (value + alignment - 1) & ~(alignment - 1);  }
	template <typename T> static inline void alignDown   ( T& value, size_t alignment)  {  value = value & ~(alignment - 1);  }	
	
	template <typename T, typename U> inline
	static bool clampMin( T& val, const U& min_val )
	{
		if ( val < min_val )
		{
			val = min_val;
			return ( true );
		}
		return ( false );
	}	
	
	template <typename T, typename U> inline
	static bool clampMax( T& val, const U& max_val )
	{
		if ( val > max_val )
		{
			val = max_val;
			return ( true );
		}
		return ( false );
	}	
	
	template <typename T, typename U, typename V> inline
	static bool clampMinMax( T& val, const U& min_val, const V& max_val )
	{
		if ( val < min_val )
		{
			val = min_val;
			return ( true );
		}
		else if ( val > max_val )
		{
			val = max_val;
			return ( true );
		}
		return ( false );
	}

	OO_API int rand(int min, int max);
	//{
	//	//return min + (::rand() % (max - min + 1));
	//	return min;
	//}

	template <typename T> inline
	static T lerp(Real s, const T& v1, const T& v2)
	{
		return v1 + s*(v2-v1);
	}
};

//-----------------------------------------------------------------------------
// Inlined function definition.

inline Real Math::rAbs(Real val) { return fabsf(val); }
inline Real Math::rSqrt(Real val) { return sqrt(val); }
inline Real Math::rInvSqrt(Real val) { return 1.0/sqrt(val); }
inline Real Math::rSin(Real val) { return sin(val); }
inline Real Math::rAsin(Real val) { return asin(val); }
inline Real Math::rCos(Real val) { return cos(val); }
inline Real Math::rAcos(Real val) { return acos(val); }
inline Real Math::rTan(Real val) { return tan(val); }
inline Real Math::rAtan(Real val) { return atan(val); }
inline Real Math::rModf(Real val, Real *intPart) { 
	Real dummy; 
	if (!intPart) intPart = &dummy; 
	return modff(val, intPart); 
}
inline Real Math::rFMod(Real val, Real base) { return fmod(val, base); }

inline Real Math::rPow(Real fBase, Real fExponent)
{
	return (Real)pow((double)fBase,(double)fExponent);
}

inline Real Math::rFastSqrt(Real val)
{
#define FP_BITS(fp) (*(Dword*)&(fp))
	if (FP_BITS(val) == 0)
		return 0.0;                 // check for square root of 0
  
	FP_BITS(val) = fast_sqrt_table[(FP_BITS(val) >> 8) & 0xFFFF] | ((((FP_BITS(val) - 0x3F800000) >> 1) + 0x3F800000) & 0x7F800000);
  
	return val;
#undef FP_BITS
}

OO_NAMESPACE_END

#pragma warning ( pop )
#endif // #ifndef OO_MATH_H_