// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_VECTOR3_H_
#define OO_VECTOR3_H_

#include "v2Math.h"

OO_NAMESPACE_BEGIN

/** 三维向量。

	Vector3 包含以下公有变量：
	@code
	Real x;
	Real y;
	Real z;
	@endcode
 */
class OO_API Vector3
{
public:
	Real x;
	Real y;
	Real z;

	// predefined specific vectors
	static const Vector3 ZERO;		///< Vector3(0, 0, 0)，预定义常量。
	static const Vector3 ONE;		///< Vector3(1, 1, 1)，预定义常量。
	static const Vector3 UNIT_X;	///< Vector3(1, 0, 0)，预定义常量。
	static const Vector3 UNIT_Y;	///< Vector3(0, 1, 0)，预定义常量。
	static const Vector3 UNIT_Z;	///< Vector3(0, 0, 1)，预定义常量。

public:
	/** */
	Vector3() {};

	/** */
	Vector3( const Real * );

	/** */
	Vector3( const Vector3& );

	/** */
	Vector3( Real x, Real y, Real z );

	// casting
	operator Real* ();
	operator const Real* () const;
	
	// assignment operators
	Vector3& operator += ( const Vector3& );
	Vector3& operator -= ( const Vector3& );
	Vector3& operator *= ( Real );
	Vector3& operator /= ( Real );
	Vector3& operator *= ( const Vector3& );
	Vector3& operator /= ( const Vector3& );

	
	// unary operators
	Vector3 operator + () const;
	Vector3 operator - () const;
	
	// binary operators
	Vector3 operator + ( const Vector3& ) const;
	Vector3 operator - ( const Vector3& ) const;
	Vector3 operator * ( Real ) const;
	Vector3 operator / ( Real ) const;
	Vector3 operator * ( const Vector3& ) const;
	Vector3 operator / ( const Vector3& ) const;
	
	friend Vector3 operator * ( Real, const Vector3& );
		
	bool operator == ( const Vector3& ) const;
	bool operator != ( const Vector3& ) const;

	/** 求向量长度。*/
	Real length() const;

	/** 求向量长度的平方。
		因为少了一个开根号操作，执行速度比length快，能用lengthSq的就不要用length。
	*/
	Real lengthSq() const;
	
	/** 向量点乘。*/
	Real dot(const Vector3 &v) const;
	/** 向量叉乘。*/
	Vector3 cross(const Vector3 &v) const;

	void minimize( const Vector3 &v1, const Vector3 &v2 );
	void maximize( const Vector3 &v1, const Vector3 &v2 );
	void scale(Real s);
	void lerp(const Vector3 &v1, const Vector3 &v2, Real s );

	bool isNAN() const
	{
		return isnan(x) != 0 || isnan(y) != 0 || isnan(z) != 0;
	}

	/** 单位化。*/
	void normalize();

	bool isNormalized() const {
		Real lensq = lengthSq();
		return Math::rIsEqual((Real)1, lensq);
	}

	friend std::ostream& operator<< (std::ostream& stream, const Vector3& v);
	
	/** 取一个于本向量垂直的任意向量。*/
	Vector3 perpendicular() const
	{
		Vector3 perp = cross( Vector3::UNIT_X );

		// Check length
		if( Math::rIsEqual(perp.lengthSq(), 0.f) )
		{
			/* This vector is the Y axis multiplied by a scalar, so we have 
			to use another axis.
			*/
			perp = cross( Vector3::UNIT_Y );
		}

		return perp;
	}

	/** 取一个于本向量垂直并且朝向dir的任意向量。
		@param refDir 任意向量，不需要单位化。
	*/
	Vector3 perpendicularAndOrient(const Vector3 &refDir) const
	{
		Vector3 binor = refDir.cross(*this); // this is called a 'binormal', right?
		// return cross(binor);
		if (Math::rIsEqual(binor.lengthSq(), 0.f))
			return perpendicular();
		else
			return cross(binor);
	}
	/** 判断两个向量是否相等*/
	bool isEqual (Vector3 & v, Real epsilon = Math::EPSILON) const;
};

//-----------------------------------------------------------------------------
// Inlined Functions

inline
Vector3::Vector3( const Real *pf )
{
	x = pf[0];
	y = pf[1];
	z = pf[2];
}

inline
Vector3::Vector3( const Vector3& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}

inline
Vector3::Vector3( Real fx, Real fy, Real fz )
{
	x = fx;
	y = fy;
	z = fz;
}


// casting
inline
Vector3::operator Real* ()
{
	return (Real *) &x;
}

inline
Vector3::operator const Real* () const
{
	return (const Real *) &x;
}


// assignment operators
inline Vector3&
Vector3::operator += ( const Vector3& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline Vector3&
Vector3::operator -= ( const Vector3& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline Vector3&
Vector3::operator *= ( Real f )
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

inline Vector3&
Vector3::operator /= ( Real f )
{
	Real fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	return *this;
}

inline Vector3&
Vector3::operator *= ( const Vector3& v)
{
	x*=v.x;
	y*=v.y;
	z*=v.z;
	return *this;
}

inline Vector3&
Vector3::operator /= ( const Vector3& v)
{
	x/=v.x;
	y/=v.y;
	z/=v.z;
	return *this;
}


// unary operators
inline Vector3
Vector3::operator + () const
{
	return *this;
}

inline Vector3
Vector3::operator - () const
{
	return Vector3(-x, -y, -z);
}


// binary operators
inline Vector3
Vector3::operator + ( const Vector3& v ) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

inline Vector3
Vector3::operator - ( const Vector3& v ) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

inline Vector3
Vector3::operator * ( Real f ) const
{
	return Vector3(x * f, y * f, z * f);
}

inline Vector3
Vector3::operator / ( Real f ) const
{
	Real fInv = 1.0f / f;
	return Vector3(x * fInv, y * fInv, z * fInv);
}

inline Vector3
Vector3::operator * ( const Vector3& v) const
{
	return Vector3(x*v.x, y*v.y, z*v.z);
}

inline Vector3
Vector3::operator / ( const Vector3& v) const
{
	return Vector3(x/v.x, y/v.y, z/v.z);
}


inline Vector3
operator * ( Real f, const Vector3& v )
{
	return Vector3(f * v.x, f * v.y, f * v.z);
}


inline bool
Vector3::operator == ( const Vector3& v ) const
{
	return x == v.x && y == v.y && z == v.z;
}

inline bool
Vector3::operator != ( const Vector3& v ) const
{
	return x != v.x || y != v.y || z != v.z;

}

inline Real
Vector3::length() const
{
	return Math::rSqrt(x * x + y * y + z * z);
}

inline Real
Vector3::lengthSq() const
{
	return x * x + y * y + z * z;
}

inline Real 
Vector3::dot(const Vector3 &v) const
{
	return x * v.x + y * v.y + z * v.z;
}

inline Vector3
Vector3::cross( const Vector3 &v ) const
{
	Vector3 out;
	out.x = y * v.z - z * v.y;
	out.y = z * v.x - x * v.z;
	out.z = x * v.y - y * v.x;

	return  out;
}

inline void
Vector3::minimize( const Vector3 &v1, const Vector3 &v2 )
{
	x = v1.x < v2.x ? v1.x : v2.x;
	y = v1.y < v2.y ? v1.y : v2.y;
	z = v1.z < v2.z ? v1.z : v2.z;
}

inline void
Vector3::maximize( const Vector3 &v1, const Vector3 &v2 )
{
	x = v1.x > v2.x ? v1.x : v2.x;
	y = v1.y > v2.y ? v1.y : v2.y;
	z = v1.z > v2.z ? v1.z : v2.z;
}

inline void
Vector3::scale(Real s)
{
	x *= s;
	y *= s;
	z *= s;
}

inline void
Vector3::lerp(const Vector3 &v1, const Vector3 &v2, Real s )
{
	x = v1.x + s * (v2.x - v1.x);
	y = v1.y + s * (v2.y - v1.y);
	z = v1.z + s * (v2.z - v1.z);
}

inline void 
Vector3::normalize()
{
	Real lenSq = lengthSq();
	if (lenSq > Math::EPSILON) {
		Real invLen = Math::rInvSqrt(lenSq);
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}	
}

inline bool Vector3::isEqual(Vector3 & v, Real epsilon /* = Math::EPSILON */) const
{
	if((fabs(x - v.x) >= epsilon)
		|| (fabs(y - v.y) >= epsilon)
		|| (fabs(z - v.z) >= epsilon)) 
		return false;
	else
		return true;
}



OO_NAMESPACE_END

#endif // #ifndef OO_VECTOR3_H_


