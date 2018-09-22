//// This file is part of Origo Engine.
//// Copyright (C), Beijing Tuyasoft Co., Ltd.
///*
//-------------------------------------------------------------------------------
//Notes:
//This file is part of Origo Engine.
//The original version is from flipcode forums(http://www.flipcode.com/).
//ASM math routines posted by davepermen et al on flipcode forums
//-------------------------------------------------------------------------------
//*/
//
//#ifndef OO_ASM_MATH_
//#define OO_ASM_MATH_
//#include "..\Common\v2ObjStd.h"
//OO_NAMESPACE_BEGIN
//
//const float pi = 4.0*atan(1.0);
//const float half_pi = 0.5*pi;
//// ASM math routines posted by davepermen et al on flipcode forums
////----------------------------------------------------------------------------------------------------------
//// NB NO EXPLICIT RETURN REQUIRED FROM THESE METHODS!!
//#pragma warning( push )
//#pragma warning( disable: 4035 )
////----------------------------------------------------------------------------------------------------------
//inline float asm_arccos( float r ) 
//{
//	// return half_pi + arctan( r / -sqr( 1.f - r * r ) );
//	float asm_one = 1.f;
//	float asm_half_pi = half_pi;
//#if defined( _MSC_VER )
//#  ifdef _M_X64
//	return acos(r);
//#  else
//	__asm {
//		fld r // r0 = r
//			fld r // r1 = r0, r0 = r
//			fmul r // r0 = r0 * r
//			fsubr asm_one // r0 = r0 - 1.f
//			fsqrt // r0 = sqrtf( r0 )
//			fchs // r0 = - r0
//			fdiv // r0 = r1 / r0
//			fld1 // {{ r0 = atan( r0 )
//			fpatan // }}
//			fadd asm_half_pi // r0 = r0 + pi / 2
//	} // returns r0
//#  endif
//#elif defined( __GNUC__ )
//	asm("fld %0" : "=f" (r));
//	asm("fld %0" : "=f" (r));
//	asm("fmul %0" : "=f" (r));
//	asm("fsubr %0" : "=f" (asm_one));
//	asm("fsqrt");
//	asm("fchs");
//	asm("fdiv");
//	asm("fld1");
//	asm("fpatan");
//	asm("fadd %0" : "=f" (asm_half_pi));
//#endif
//
//}
//
//inline float asm_arcsin( float r ) {
//	// return arctan( r / sqr( 1.f - r * r ) );
//	const float asm_one = 1.f;
//#if defined( _MSC_VER )
//#	ifdef _M_X64
//	return asin(r);
//#	else
//	__asm {
//		fld r // r0 = r
//			fld r // r1 = r0, r0 = r
//			fmul r // r0 = r0 * r
//			fsubr asm_one // r0 = r0 - 1.f
//			fsqrt // r0 = sqrtf( r0 )
//			fdiv // r0 = r1 / r0
//			fld1 // {{ r0 = atan( r0 )
//			fpatan // }}
//	} // returns r0
//#	endif
//#elif defined( __GNUC__ )
//	asm("fld %0" : "=f" (r));
//	asm("fld %0" : "=f" (r));
//	asm("fmul %0" : "=f" (r));
//	asm("fsubr %0" : "=f" (asm_one));
//	asm("fsqrt");
//	asm("fdiv");
//	asm("fld1");
//	asm("fpatan");
//#endif
//
//}
//
//inline float asm_arctan( float r ) {
//#if defined( _MSC_VER )
//#	ifdef _M_X64
//	return atan(r);
//#	else
//	__asm {
//		fld r // r0 = r
//			fld1 // {{ r0 = atan( r0 )
//			fpatan // }}
//	} // returns r0
//#	endif
//#elif defined( __GNUC__ )
//	asm("fld %0" : "=f" (r));
//	asm("fld1");
//	asm("fpatan");
//#endif
//
//}
//
//inline float asm_sin( float r ) {
//#ifdef _MSC_VER
//#	ifdef _M_X64
//	return asin(r);
//#	else
//	__asm {
//		fld r // r0 = r
//			fsin // r0 = sinf( r0 )
//	} // returns r0
//#	endif
//#elif defined( __GNUC__ )
//	asm("fld %0" : "=f" (r));
//	asm("fsin");
//#endif
//
//}
//
//inline float asm_cos( float r ) {
//#ifdef _MSC_VER
//#	ifdef _M_X64
//	return acos(r);
//#	else
//	__asm {
//		fld r // r0 = r
//			fcos // r0 = cosf( r0 )
//	} // returns r0
//#	endif
//#elif defined( __GNUC__ )
//	asm("fld %0" : "=f" (r));
//	asm("fcos");
//#endif
//
//}
//
//inline float asm_tan( float r ) {
//#ifdef _MSC_VER
//#	ifdef _M_X64
//	return tan(r);
//#	else
//	// return sin( r ) / cos( r );
//	__asm {
//		fld r // r0 = r
//			fsin // r0 = sinf( r0 )
//			fld r // r1 = r0, r0 = r
//			fcos // r0 = cosf( r0 )
//			fdiv // r0 = r1 / r0
//	} // returns r0
//#	endif
//#elif defined( __GNUC__ )
//	asm("fld %0" : "=f" (r));
//	asm("fsin");
//	asm("fld %0" : "=f" (r));
//	asm("fcos");
//	asm("fdiv");
//#endif
//
//}
//
//inline float asm_sqrt( float r ) {
//	// returns a for a * a = r
//#ifdef _MSC_VER
//#	ifdef _M_X64
//	return sqrt(r);
//#	else
//	__asm {
//		fld r // r0 = r
//			fsqrt // r0 = sqrtf( r0 )
//	} // returns r0
//#	endif
//#elif defined( __GNUC__ )
//	asm("fld %0" : "=f" (r));
//	asm("fsqrt");
//#endif
//
//}
//
//// -- Use this for Vector normalisation!!!
//inline float asm_rsq( float r ) {
//	// returns 1 / a for a * a = r
//#ifdef _MSC_VER
//#	ifdef _M_X64
//	return 1.f/sqrt(r);
//#	else
//	__asm {
//		fld1 // r0 = 1.f
//			fld r // r1 = r0, r0 = r
//			fsqrt // r0 = sqrtf( r0 )
//			fdiv // r0 = r1 / r0
//	} // returns r0
//#	endif
//#elif defined( __GNUC__ )
//	asm("fld1");
//	asm("fld %0" : "=f" (r));
//	asm("fsqrt");
//	asm("fdiv");
//#endif
//
//}
//
////// Another version
////inline float apx_rsq( float r ) {
////	// returns 1 / a for a * a = r
////	const float asm_dot5 = 0.5f;
////	const float asm_1dot5 = 1.5f;
////
////#ifdef _MSC_VER
////	__asm {
////		fld r // r0 = r
////			fmul asm_dot5 // r0 = r0 * .5f
////			mov eax, r // eax = r
////			shr eax, 0x1 // eax = eax >> 1
////			neg eax // eax = -eax
////			add eax, 0x5F400000 // eax = eax & MAGICAL NUMBER
////			mov r, eax // r = eax
////			fmul r // r0 = r0 * r
////			fmul r // r0 = r0 * r
////			fsubr asm_1dot5 // r0 = 1.5f - r0
////			fmul r // r0 = r0 * r
////	} // returns r0
////#elif defined( __GNUC__ )
////	asm("fld %0" : "=f" (r));
////	asm("fmul %0" : "=f" (asm_dot5));
////	asm("mov eax, %0" : "f" (r));
////	asm("shr eax, 0x1");
////	asm("neg eax");
////	asm("add eax, 0x5F400000");
////	asm("mov %0, eax" : "=f" (r));
////	asm("fmul %0" : "=f" (r));
////	asm("fmul %0" : "=f" (r));
////	asm("fsubr %0" : "f" (asm_1dot5));
////	asm("fmul %0" : "=f" (r));
////#endif
////
////}
//
/////* very MS-specific, commented out for now
////// Finally the best InvSqrt implementation?
////// Use for vector normalisation instead of 1/length() * x,y,z
////*/
////#ifdef _MSC_VER
////
////inline __declspec(naked) float __fastcall InvSqrt(float fValue)
////{
////	__asm
////	{
////		mov        eax, 0be6eb508h
////			mov        dword ptr[esp-12],03fc00000h
////			sub        eax, dword ptr[esp + 4]
////		sub        dword ptr[esp+4], 800000h
////			shr        eax, 1
////			mov        dword ptr[esp -  8], eax
////
////			fld        dword ptr[esp -  8]
////		fmul    st, st
////			fld        dword ptr[esp -  8]
////		fxch    st(1)
////			fmul    dword ptr[esp +  4]
////		fld        dword ptr[esp - 12]
////		fld        st(0)
////			fsub    st,st(2)
////
////			fld        st(1)
////			fxch    st(1)
////			fmul    st(3),st
////			fmul    st(3),st
////			fmulp    st(4),st
////			fsub    st,st(2)
////
////			fmul    st(2),st
////			fmul    st(3),st
////			fmulp    st(2),st
////			fxch    st(1)
////			fsubp    st(1),st
////
////			fmulp    st(1), st
////			ret 4
////	}
////}
////
////#endif
//
////inline float asm_ln( float r ) {
////	// returns log2( r ) / log2( e )
////	const float asm_e = 2.71828182846f;
////	const float asm_1_div_log2_e = .693147180559f;
////	const float asm_neg1_div_3 = -.33333333333333333333333333333f;
////	const float asm_neg2_div_3 = -.66666666666666666666666666667f;
////	const float asm_2 = 2.f;
////
////	int log_2 = 0;
////#ifdef _MSC_VER
////	__asm {
////		// log_2 = ( ( r >> 0x17 ) & 0xFF ) - 0x80;
////		mov eax, r
////			sar eax, 0x17
////			and eax, 0xFF
////			sub eax, 0x80
////			mov log_2, eax
////
////			// r = ( r & 0x807fffff ) + 0x3f800000;
////			mov ebx, r
////			and ebx, 0x807FFFFF
////			add ebx, 0x3F800000
////			mov r, ebx
////
////			// r = ( asm_neg1_div_3 * r + asm_2 ) * r + asm_neg2_div_3;   // (1)
////			fld r
////			fmul asm_neg1_div_3
////			fadd asm_2
////			fmul r
////			fadd asm_neg2_div_3
////			fild log_2
////			fadd
////			fmul asm_1_div_log2_e
////	}
////#elif defined( __GNUC__ )
////	asm("mov eax, r");
////	asm("sar eax, 0x17");
////	asm("and eax, 0xFF");
////	asm("sub eax, 0x80");
////	asm("mov log_2, eax");
////	asm("mov ebx, r");
////	asm("mov ebx, 0x807FFFFF");
////	asm("add ebx, 0x3F800000");
////	asm("mov r, ebx");
////	asm("fld r");
////	asm("fmul asm_neg1_div_3");
////	asm("fadd asm_2");
////	asm("fmul r");
////	asm("fadd asm_neg2_div_3");
////	asm("fild log_2");
////	asm("fadd");
////	asm("fmul asm_1_div_log2_e");
////#endif
////
////}
//
//#pragma warning( pop )
//
//OO_NAMESPACE_END
//
//#endif // #ifndef OO_ASM_MATH_
