// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_Quaternion_H_
#define OO_Quaternion_H_

#include "v2Math.h"
#include "v2Vector3.h"
//#include "v2Vector4.h"
#include "../Math2/Vector4.h"

OO_NAMESPACE_BEGIN

/** 四元组。
*/

class Matrix4;
class OO_API Quaternion
{
public:
	Real x, y, z, w;

	/** 单位四元组。*/
	static const Quaternion IDENTITY;	///< Quaternion(0, 0, 0, 1)，预定义单位Quaternion。

public:
	//@{
	/** 构造函数。*/
	Quaternion() {}
	Quaternion( const Real* pf );
	Quaternion( const Quaternion &q );
	Quaternion( Real x, Real y, Real z, Real w );
	//@}	
	
	// casting
	operator Real* ();
	operator const Real* () const;
	
	//@{
	/** 附值操作。*/
	Quaternion& operator += ( const Quaternion& );
	Quaternion& operator -= ( const Quaternion& );
	Quaternion& operator *= ( const Quaternion& );
	Quaternion& operator *= ( Real );
	Quaternion& operator /= ( Real );
	//@}


	//@{
	/** 一元操作。*/
	Quaternion  operator + () const;
	Quaternion  operator - () const;
	//@}


	//@{
	/** 二元操作。*/
	Quaternion operator + ( const Quaternion& ) const;
	Quaternion operator - ( const Quaternion& ) const;
	Quaternion operator * ( const Quaternion& ) const;
	Quaternion operator * ( Real ) const;
	Quaternion operator / ( Real ) const;
	//@}

	friend Quaternion operator * (Real, const Quaternion& );
	
	bool operator == ( const Quaternion& ) const;
	bool operator != ( const Quaternion& ) const;

	/** 生成一个Quaternion，表示绕axis轴旋转angle角度。
		@param axis 旋转轴。
		@param angle 单位是弧度。
	*/
	inline void fromAxisAngle(const Vector3 &axis, Real angle);

	/** 取得四元组所代表的轴和角度。		
		@param axis 返回的轴。
		@param angle 返回的角度，弧度。
		注意：使用此方法前该四元组必须单位化过。
	*/
	inline void toAxisAngle(Vector3 &axis, Real &angle) const {
		angle = getAngle();
		axis = getAxis();
	}

	/** 返回Quaternion所表示的旋转轴和旋转角度。
	*/
	Vector3 getAxis() const {
		return Vector3(x, y, z);
	}
	/** 返回Quaternion所表示的旋转角度（弧度）。
		注意：使用此方法前该四元组必须单位化过，如果 w 不在[-1,1]区间该函数将返回0。
	*/
	Real getAngle() const {
		if (w < -1.f || w > 1.f) {		
			ooAssertDesc(false, "Quaternion::getAngle 使用前 Quaternion 必须单位化");
			return 0.f; // 返回 0 要比返回 nan 更好，出错是不会 down 掉程序。
		}
		else
			return Math::rAcos(w) * 2.f;
	}

	/** 生成一个Quaternion，表示把向量from旋转到to。
		@param from 起始方向。
		@param to 目标方向。
	*/
	void fromVectorToVector(const Vector3 &from, const Vector3 &to);
	void fromRotationMatrix(const Matrix4 &mat);

	/** 生成一个Quaternion，表示座标系的转换。
		@param xAxis X 轴方向。
		@param yAxis Y 轴方向。
		@param zAxis Z 轴方向。
		三个轴必须正交，并且必须是单位化的。
	*/
	void fromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
	
	/** 生成一个Quaternion，表示两个座标系的转换。
	@param xAxis1 X 轴方向。
	@param yAxis1 Y 轴方向。
	@param zAxis1 Z 轴方向。
	@param xAxis2 X 轴方向。
	@param yAxis2 Y 轴方向。
	@param zAxis2 Z 轴方向。
	三个轴必须正交，并且必须是单位化的。
	*/
	//void fromCoordinateTransform(const Vector3& xAxis1, const Vector3& yAxis1, const Vector3& zAxis1,
	//	const Vector3& xAxis2, const Vector3& yAxis2, const Vector3& zAxis2);

	/** 返回转换后的向量in。
		Transform (x, y, z, 1) by matrix, project result back into w=1.
	*/
	Vector3 transform(const Vector3 &in) const;

	//Vector4 transform(const Vector4 &in) const;

	/** 单位化。
		单位化后length()将返回1。
	*/
	void normalize() {
		Real mag = Math::rSqrt(x*x+y*y+z*z+w*w);
		if(mag>0)
		{
			x/=mag;
			y/=mag;
			z/=mag;
			w/=mag;
		}
	}

	bool isNAN() const
	{
		return isnan(x) != 0 || isnan(y) != 0 || isnan(z) != 0 || isnan(w) != 0;
	}

	Quaternion getConjugate() const {
		return Quaternion(-x, -y, -z, w);
	}

	/** 共轭转换。

		Quaternion的共轭为(-x, -y, -z, w)。Quaternion的逆变换就是它的共
		轭(conjugate)，即：Q*conjugate(Q) = I。
	*/
	void conjugate() {
		*this = getConjugate();
	}


// From Gems I, "Portions Copyright (C) Jason Shankel, 2000"

	/** Linear interpolation between two Quaternions
		@note this method does not check theta > 180. so the returns may not be the shortest arc.
	*/
	void lerp(const Quaternion &q1, const Quaternion &q2, Real t);
	/** Spherical linear interpolation between two Quaternions.*/
	void slerp(const Quaternion &q1,const Quaternion &q2,float t);
	/** Spherical cubic interpolation.*/
	void squad(const Quaternion &q1,const Quaternion &q2,const Quaternion &a,const Quaternion &b,float t);
	/** Given 3 Quaternions, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation. */
	void spline(const Quaternion &q1,const Quaternion &q2,const Quaternion &q3);
	
	static Quaternion exp(const Quaternion &q);
	static Quaternion log(const Quaternion &q);

	Real dot(const Quaternion &q2) const {
		return x*q2.x + y*q2.y + z*q2.z + w*q2.w;
	}

	void fromEuler(const Vector3 &eular);

	void toYawPitchRoll(float & yaw,float & pitch,float & roll);
};


//-----------------------------------------------------------------------------
// Quaternion inlined functions

inline
Quaternion::Quaternion( const Real* pf )
{
	ooBreakAssert(pf);

    x = pf[0];
    y = pf[1];
    z = pf[2];
    w = pf[3];
}

inline
Quaternion::Quaternion( const Quaternion& q )
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
}

inline
Quaternion::Quaternion( Real fx, Real fy, Real fz, Real fw )
{
    x = fx;
    y = fy;
    z = fz;
    w = fw;
}


// casting
inline
Quaternion::operator Real* ()
{
    return (Real *) &x;
}

inline
Quaternion::operator const Real* () const
{
    return (const Real *) &x;
}


// assignment operators
inline Quaternion&
Quaternion::operator += ( const Quaternion& q )
{
    x += q.x;
    y += q.y;
    z += q.z;
    w += q.w;
    return *this;
}

inline Quaternion&
Quaternion::operator -= ( const Quaternion& q )
{
    x -= q.x;
    y -= q.y;
    z -= q.z;
    w -= q.w;
    return *this;
}

inline Quaternion&
Quaternion::operator *= ( Real f )
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    return *this;
}

inline Quaternion&
Quaternion::operator /= ( Real f )
{
    Real fInv = 1.0f / f;
    x *= fInv;
    y *= fInv;
    z *= fInv;
    w *= fInv;
    return *this;
}


// unary operators
inline Quaternion
Quaternion::operator + () const
{
    return *this;
}

inline Quaternion
Quaternion::operator - () const
{
    return Quaternion(-x, -y, -z, -w);
}


// binary operators
inline Quaternion
Quaternion::operator + ( const Quaternion& q ) const
{
    return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

inline Quaternion
Quaternion::operator - ( const Quaternion& q ) const
{
    return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

inline Quaternion
Quaternion::operator * ( Real f ) const
{
    return Quaternion(x * f, y * f, z * f, w * f);
}

inline Quaternion
Quaternion::operator / ( Real f ) const
{
    Real fInv = 1.0f / f;
    return Quaternion(x * fInv, y * fInv, z * fInv, w * fInv);
}


inline Quaternion
operator * (Real f, const Quaternion& q )
{
    return Quaternion(f * q.x, f * q.y, f * q.z, f * q.w);
}


inline bool
Quaternion::operator == ( const Quaternion& q ) const
{
    return x == q.x && y == q.y && z == q.z && w == q.w;
}

inline bool
Quaternion::operator != ( const Quaternion& q ) const
{
    return x != q.x || y != q.y || z != q.z || w != q.w;
}

inline void 
Quaternion::fromAxisAngle(const Vector3 &axis, Real angle)
{
	Vector3 u = axis;
	u.normalize();

	Real s = Math::rSin(angle/2.f); 
	x = s*u.x;
	y = s*u.y;
	z = s*u.z;

	w = Math::rCos(angle/2.f); 
}

OO_NAMESPACE_END

#endif // #ifndef OO_Quaternion_H_
