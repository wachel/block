// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_Quaternion_H_
#define OO_Quaternion_H_

#include "v2Math.h"
#include "v2Vector3.h"
//#include "v2Vector4.h"
#include "../Math2/Vector4.h"

OO_NAMESPACE_BEGIN

/** ��Ԫ�顣
*/

class Matrix4;
class OO_API Quaternion
{
public:
	Real x, y, z, w;

	/** ��λ��Ԫ�顣*/
	static const Quaternion IDENTITY;	///< Quaternion(0, 0, 0, 1)��Ԥ���嵥λQuaternion��

public:
	//@{
	/** ���캯����*/
	Quaternion() {}
	Quaternion( const Real* pf );
	Quaternion( const Quaternion &q );
	Quaternion( Real x, Real y, Real z, Real w );
	//@}	
	
	// casting
	operator Real* ();
	operator const Real* () const;
	
	//@{
	/** ��ֵ������*/
	Quaternion& operator += ( const Quaternion& );
	Quaternion& operator -= ( const Quaternion& );
	Quaternion& operator *= ( const Quaternion& );
	Quaternion& operator *= ( Real );
	Quaternion& operator /= ( Real );
	//@}


	//@{
	/** һԪ������*/
	Quaternion  operator + () const;
	Quaternion  operator - () const;
	//@}


	//@{
	/** ��Ԫ������*/
	Quaternion operator + ( const Quaternion& ) const;
	Quaternion operator - ( const Quaternion& ) const;
	Quaternion operator * ( const Quaternion& ) const;
	Quaternion operator * ( Real ) const;
	Quaternion operator / ( Real ) const;
	//@}

	friend Quaternion operator * (Real, const Quaternion& );
	
	bool operator == ( const Quaternion& ) const;
	bool operator != ( const Quaternion& ) const;

	/** ����һ��Quaternion����ʾ��axis����תangle�Ƕȡ�
		@param axis ��ת�ᡣ
		@param angle ��λ�ǻ��ȡ�
	*/
	inline void fromAxisAngle(const Vector3 &axis, Real angle);

	/** ȡ����Ԫ�����������ͽǶȡ�		
		@param axis ���ص��ᡣ
		@param angle ���صĽǶȣ����ȡ�
		ע�⣺ʹ�ô˷���ǰ����Ԫ����뵥λ������
	*/
	inline void toAxisAngle(Vector3 &axis, Real &angle) const {
		angle = getAngle();
		axis = getAxis();
	}

	/** ����Quaternion����ʾ����ת�����ת�Ƕȡ�
	*/
	Vector3 getAxis() const {
		return Vector3(x, y, z);
	}
	/** ����Quaternion����ʾ����ת�Ƕȣ����ȣ���
		ע�⣺ʹ�ô˷���ǰ����Ԫ����뵥λ��������� w ����[-1,1]����ú���������0��
	*/
	Real getAngle() const {
		if (w < -1.f || w > 1.f) {		
			ooAssertDesc(false, "Quaternion::getAngle ʹ��ǰ Quaternion ���뵥λ��");
			return 0.f; // ���� 0 Ҫ�ȷ��� nan ���ã������ǲ��� down ������
		}
		else
			return Math::rAcos(w) * 2.f;
	}

	/** ����һ��Quaternion����ʾ������from��ת��to��
		@param from ��ʼ����
		@param to Ŀ�귽��
	*/
	void fromVectorToVector(const Vector3 &from, const Vector3 &to);
	void fromRotationMatrix(const Matrix4 &mat);

	/** ����һ��Quaternion����ʾ����ϵ��ת����
		@param xAxis X �᷽��
		@param yAxis Y �᷽��
		@param zAxis Z �᷽��
		������������������ұ����ǵ�λ���ġ�
	*/
	void fromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
	
	/** ����һ��Quaternion����ʾ��������ϵ��ת����
	@param xAxis1 X �᷽��
	@param yAxis1 Y �᷽��
	@param zAxis1 Z �᷽��
	@param xAxis2 X �᷽��
	@param yAxis2 Y �᷽��
	@param zAxis2 Z �᷽��
	������������������ұ����ǵ�λ���ġ�
	*/
	//void fromCoordinateTransform(const Vector3& xAxis1, const Vector3& yAxis1, const Vector3& zAxis1,
	//	const Vector3& xAxis2, const Vector3& yAxis2, const Vector3& zAxis2);

	/** ����ת���������in��
		Transform (x, y, z, 1) by matrix, project result back into w=1.
	*/
	Vector3 transform(const Vector3 &in) const;

	//Vector4 transform(const Vector4 &in) const;

	/** ��λ����
		��λ����length()������1��
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

	/** ����ת����

		Quaternion�Ĺ���Ϊ(-x, -y, -z, w)��Quaternion����任�������Ĺ�
		��(conjugate)������Q*conjugate(Q) = I��
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
