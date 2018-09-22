// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_VECTOR4_H_
#define OO_VECTOR4_H_
#include "..\Common\v2ObjStd.h"
#include "v2Math.h"

OO_NAMESPACE_BEGIN

//class OO_API Vector4
//{
//public:
//	Real x, y, z, w;
//
//public:
//    Vector4() {};
//    Vector4( const Real* );
//    Vector4( Real x, Real y, Real z, Real w );
//
//    // casting
//    operator Real* ();
//    operator const Real* () const;
//
//    // assignment operators
//    Vector4& operator += ( const Vector4& );
//    Vector4& operator -= ( const Vector4& );
//    Vector4& operator *= ( Real );
//    Vector4& operator /= ( Real );
//
//    // unary operators
//    Vector4 operator + () const;
//    Vector4 operator - () const;
//
//    // binary operators
//    Vector4 operator + ( const Vector4& ) const;
//    Vector4 operator - ( const Vector4& ) const;
//    Vector4 operator * ( Real ) const;
//    Vector4 operator / ( Real ) const;
//
//    friend Vector4 operator * ( Real, const Vector4& );
//
//    bool operator == ( const Vector4& ) const;
//    bool operator != ( const Vector4& ) const;
//};
//
////--------------------------
//// 4D Vector
////--------------------------
//
//inline
//Vector4::Vector4( const Real *pf )
//{
//    x = pf[0];
//    y = pf[1];
//    z = pf[2];
//    w = pf[3];
//}
//
//inline
//Vector4::Vector4( Real fx, Real fy, Real fz, Real fw )
//{
//    x = fx;
//    y = fy;
//    z = fz;
//    w = fw;
//}
//
//// casting
//inline
//Vector4::operator Real* ()
//{
//    return (Real *) &x;
//}
//
//inline
//Vector4::operator const Real* () const
//{
//    return (const Real *) &x;
//}
//
//
//// assignment operators
//inline Vector4&
//Vector4::operator += ( const Vector4& v )
//{
//    x += v.x;
//    y += v.y;
//    z += v.z;
//    w += v.w;
//    return *this;
//}
//
//inline Vector4&
//Vector4::operator -= ( const Vector4& v )
//{
//    x -= v.x;
//    y -= v.y;
//    z -= v.z;
//    w -= v.w;
//    return *this;
//}
//
//inline Vector4&
//Vector4::operator *= ( Real f )
//{
//    x *= f;
//    y *= f;
//    z *= f;
//    w *= f;
//    return *this;
//}
//
//inline Vector4&
//Vector4::operator /= ( Real f )
//{
//    Real fInv = 1.0f / f;
//    x *= fInv;
//    y *= fInv;
//    z *= fInv;
//    w *= fInv;
//    return *this;
//}
//
//
//// unary operators
//inline Vector4
//Vector4::operator + () const
//{
//    return *this;
//}
//
//inline Vector4
//Vector4::operator - () const
//{
//    return Vector4(-x, -y, -z, -w);
//}
//
//
//// binary operators
//inline Vector4
//Vector4::operator + ( const Vector4& v ) const
//{
//    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
//}
//
//inline Vector4
//Vector4::operator - ( const Vector4& v ) const
//{
//    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
//}
//
//inline Vector4
//Vector4::operator * ( Real f ) const
//{
//    return Vector4(x * f, y * f, z * f, w * f);
//}
//
//inline Vector4
//Vector4::operator / ( Real f ) const
//{
//    Real fInv = 1.0f / f;
//    return Vector4(x * fInv, y * fInv, z * fInv, w * fInv);
//}
//
//
//inline Vector4
//operator * ( Real f, const Vector4& v )
//{
//    return Vector4(f * v.x, f * v.y, f * v.z, f * v.w);
//}
//
//
//inline bool
//Vector4::operator == ( const Vector4& v ) const
//{
//    return x == v.x && y == v.y && z == v.z && w == v.w;
//}
//
//inline bool
//Vector4::operator != ( const Vector4& v ) const
//{
//    return x != v.x || y != v.y || z != v.z || w != v.w;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//
////--------------------------
//// 4D Vector
////--------------------------
//
//// inline
//
//Real Vector4Length
//    ( const Vector4 *pV );
//
//Real Vector4LengthSq
//    ( const Vector4 *pV );
//
//Real Vector4Dot
//    ( const Vector4 *pV1, const Vector4 *pV2 );
//
//Vector4* Vector4Add
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2);
//
//Vector4* Vector4Subtract
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2);
//
//// Minimize each component.  x = min(x1, x2), y = min(y1, y2), ...
//Vector4* Vector4Minimize
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2);
//
//// Maximize each component.  x = max(x1, x2), y = max(y1, y2), ...
//Vector4* Vector4Maximize
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2);
//
//Vector4* Vector4Scale
//    ( Vector4 *pOut, const Vector4 *pV, Real s);
//
//// Linear interpolation. V1 + s(V2-V1)
//Vector4* Vector4Lerp
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
//      Real s );
//
//// non-inline
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//// Cross-product in 4 dimensions.
//OO_API Vector4* Vector4Cross
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
//      const Vector4 *pV3);
//
//OO_API Vector4* Vector4Normalize
//    ( Vector4 *pOut, const Vector4 *pV );
//
//// Hermite interpolation between position V1, tangent T1 (when s == 0)
//// and position V2, tangent T2 (when s == 1).
//OO_API Vector4* Vector4Hermite
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pT1,
//      const Vector4 *pV2, const Vector4 *pT2, Real s );
//
//// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
//OO_API Vector4* Vector4CatmullRom
//    ( Vector4 *pOut, const Vector4 *pV0, const Vector4 *pV1,
//      const Vector4 *pV2, const Vector4 *pV3, Real s );
//
//// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
//OO_API Vector4* Vector4BaryCentric
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
//      const Vector4 *pV3, Real f, Real g);
//
//// Transform vector by matrix.
//class Matrix4;
//OO_API Vector4* Vector4Transform
//    ( Vector4 *pOut, const Vector4 *pV, const Matrix4 *pM );
//
//#ifdef __cplusplus
//}
//#endif
//
/////////////////////////////////////////////////////////////////////////////////
//
//inline Real Vector4Length
//    ( const Vector4 *pV )
//{
//    return Math::rSqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w);
//}
//
//inline Real Vector4LengthSq
//    ( const Vector4 *pV )
//{
//    return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w;
//}
//
//inline Real Vector4Dot
//    ( const Vector4 *pV1, const Vector4 *pV2 )
//{
//    return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z + pV1->w * pV2->w;
//}
//
//inline Vector4* Vector4Add
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
//{
//    pOut->x = pV1->x + pV2->x;
//    pOut->y = pV1->y + pV2->y;
//    pOut->z = pV1->z + pV2->z;
//    pOut->w = pV1->w + pV2->w;
//    return pOut;
//}
//
//inline Vector4* Vector4Subtract
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
//{
//    pOut->x = pV1->x - pV2->x;
//    pOut->y = pV1->y - pV2->y;
//    pOut->z = pV1->z - pV2->z;
//    pOut->w = pV1->w - pV2->w;
//    return pOut;
//}
//
//inline Vector4* Vector4Minimize
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
//{
//    pOut->x = pV1->x < pV2->x ? pV1->x : pV2->x;
//    pOut->y = pV1->y < pV2->y ? pV1->y : pV2->y;
//    pOut->z = pV1->z < pV2->z ? pV1->z : pV2->z;
//    pOut->w = pV1->w < pV2->w ? pV1->w : pV2->w;
//    return pOut;
//}
//
//inline Vector4* Vector4Maximize
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
//{
//    pOut->x = pV1->x > pV2->x ? pV1->x : pV2->x;
//    pOut->y = pV1->y > pV2->y ? pV1->y : pV2->y;
//    pOut->z = pV1->z > pV2->z ? pV1->z : pV2->z;
//    pOut->w = pV1->w > pV2->w ? pV1->w : pV2->w;
//    return pOut;
//}
//
//inline Vector4* Vector4Scale
//    ( Vector4 *pOut, const Vector4 *pV, Real s)
//{
//    pOut->x = pV->x * s;
//    pOut->y = pV->y * s;
//    pOut->z = pV->z * s;
//    pOut->w = pV->w * s;
//    return pOut;
//}
//
//inline Vector4* Vector4Lerp
//    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
//      Real s )
//{
//    pOut->x = pV1->x + s * (pV2->x - pV1->x);
//    pOut->y = pV1->y + s * (pV2->y - pV1->y);
//    pOut->z = pV1->z + s * (pV2->z - pV1->z);
//    pOut->w = pV1->w + s * (pV2->w - pV1->w);
//    return pOut;
//}

///////////////////////////////////////////////////////////////////////////////
/*

// Cross-product in 4 dimensions.
inline OO_API Vector4* Vector4Cross
    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
      const Vector4 *pV3)
{
	return (Vector4*)D3DXVec4Cross((D3DXVECTOR4*)pOut, (const D3DXVECTOR4*)pV1, (const D3DXVECTOR4*)pV2, (D3DXVECTOR4*)pV3);
}

inline OO_API Vector4* Vector4Normalize
    ( Vector4 *pOut, const Vector4 *pV )
{
	return (Vector4*)D3DXVec4Normalize((D3DXVECTOR4*)pOut, (const D3DXVECTOR4*)pV);
}

// Hermite interpolation between position V1, tangent T1 (when s == 0)
// and position V2, tangent T2 (when s == 1).
inline OO_API Vector4* Vector4Hermite
    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pT1,
      const Vector4 *pV2, const Vector4 *pT2, Real s )
{
	return (Vector4*)D3DXVec4Hermite((D3DXVECTOR4*)pOut, (const D3DXVECTOR4*)pV1,
		(const D3DXVECTOR4*)pT1, (const D3DXVECTOR4*)pV2, (const D3DXVECTOR4*)pT2,
		s);
}


// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
inline OO_API Vector4* Vector4CatmullRom
    ( Vector4 *pOut, const Vector4 *pV0, const Vector4 *pV1,
      const Vector4 *pV2, const Vector4 *pV3, Real s )
{
	return (Vector4*)D3DXVec4CatmullRom((D3DXVECTOR4*)pOut, (const D3DXVECTOR4*)pV0,
		(const D3DXVECTOR4*)pV1, (const D3DXVECTOR4*)pV2, (const D3DXVECTOR4*)pV3,
		s);
}

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
inline OO_API Vector4* Vector4BaryCentric
    ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
      const Vector4 *pV3, Real f, Real g)
{
	return (Vector4*)D3DXVec4BaryCentric((D3DXVECTOR4*)pOut, (const D3DXVECTOR4*)pV1,
		(const D3DXVECTOR4*)pV2, (const D3DXVECTOR4*)pV3, f, g);
}

// Transform vector by matrix.
inline OO_API Vector4* Vector4Transform
    ( Vector4 *pOut, const Vector4 *pV, const Matrix4 *pM )
{
	return (Vector4*)D3DXVec4Transform((D3DXVECTOR4*)pOut, (const D3DXVECTOR4*)pV,
		(const D3DXMATRIX*)pM);
}
*/

OO_NAMESPACE_END

#endif // #ifndef OO_VECTOR4_H_