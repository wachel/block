// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
//#include "v2Standard.h"
#include "v2Quaternion.h"
#include "v2Matrix4.h"

#include "my_d3dx9math.h"

OO_NAMESPACE_BEGIN

const Quaternion Quaternion::IDENTITY(0, 0, 0, 1);

void Quaternion::fromVectorToVector(const Vector3 &from, const Vector3 &to)
{
	Vector3 s = from;
	s.normalize();
	Vector3 t = to;
	t.normalize();

	Vector3 sxt = s.cross(t); // SxT
	Real len_sxt = sxt.length(); // == cos(angle)
	Real dot = s.dot(t);
	if (Math::rIsEqual(len_sxt, 0.f)) {
		// parallel case.

		// 04/7/6 added. fix bug.
		if (dot > 0.0)
            *this = IDENTITY;
		else
			// inverse.
			fromAxisAngle(from.perpendicular(), Math::PI);

		return;
	}

	Vector3 u = sxt / len_sxt;

	//Real angle2 = Math::rAsin(len_sxt)/2.f;
	//if (dot < 0.f)
	//	angle2 = Math::PI/2.0f - angle2;
	Real angle2 = Math::rAcos(dot)/2.f;

	Real sina = Math::rSin(angle2);
	Real cosa = Math::rCos(angle2);

	x = u.x*sina;
	y = u.y*sina;
	z = u.z*sina;
	w = cosa;
}

void Quaternion::fromRotationMatrix(const Matrix4 &mat)
{
	/*
	D3DXQuaternionRotationMatrix((D3DXQuaternion*)this, (CONST D3DXMATRIX *)&mat);

	// MUST! invert rotation axis since D3D use right-handle coords.
	// to D3D Quaternion. 
	// see Matrix4::fromQuaternion
	//x = - x;
	//y = - y;
	//z = - z;
	*/

	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".

	Real fTrace = mat.m[0][0]+mat.m[1][1]+mat.m[2][2];
	Real fRoot;

	if ( fTrace > 0.0 )
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot = Math::rSqrt(fTrace + 1.0);  // 2w
		w = 0.5*fRoot;
		fRoot = 0.5/fRoot;  // 1/(4w)
		x = (mat.m[2][1]-mat.m[1][2])*fRoot;
		y = (mat.m[0][2]-mat.m[2][0])*fRoot;
		z = (mat.m[1][0]-mat.m[0][1])*fRoot;
	}
	else
	{
		// |w| <= 1/2
		static int s_iNext[3] = { 1, 2, 0 };
		int i = 0;
		if ( mat.m[1][1] > mat.m[0][0] )
			i = 1;
		if ( mat.m[2][2] > mat.m[i][i] )
			i = 2;
		int j = s_iNext[i];
		int k = s_iNext[j];

		fRoot = Math::rSqrt(mat.m[i][i]-mat.m[j][j]-mat.m[k][k] + 1.0);
		Real* apkQuat[3] = { &x, &y, &z };
		*apkQuat[i] = 0.5*fRoot;
		fRoot = 0.5/fRoot;
		w = (mat.m[k][j]-mat.m[j][k])*fRoot;
		*apkQuat[j] = (mat.m[j][i]+mat.m[i][j])*fRoot;
		*apkQuat[k] = (mat.m[k][i]+mat.m[i][k])*fRoot;
	}
}

Quaternion& Quaternion::operator *= ( const Quaternion& q )
{
	// D3DXQuaternionMultiply((D3DXQuaternion*)this, (D3DXQuaternion*)this, (D3DXQuaternion*)(&q));
	*this = *this * q;
    return *this;
}

Quaternion Quaternion::operator * ( const Quaternion& q ) const
{
	
    /*Quaternion qT;
    D3DXQuaternionMultiply((D3DXQUATERNION *)&qT, (const D3DXQUATERNION *)&q, (const D3DXQUATERNION *)this);
    return qT;*/
	

	return Quaternion
		(
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y + y * q.w + z * q.x - x * q.z,
		w * q.z + z * q.w + x * q.y - y * q.x,
		w * q.w - x * q.x - y * q.y - z * q.z
		);
}

void Quaternion::fromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
	Matrix4 mat;
	mat.makeRotationFromAxes(xAxis, yAxis, zAxis);
	fromRotationMatrix(mat);
}

//void Quaternion::fromCoordinateTransform( const Vector3& xAxis1, const Vector3& yAxis1, const Vector3& zAxis1,
//						  const Vector3& xAxis2, const Vector3& yAxis2, const Vector3& zAxis2 )
//{
//	Quaternion rotInv;
//	rotInv.fromAxes(xAxis1,yAxis1,zAxis1);
//	rotInv.conjugate();
//	fromAxes(xAxis2,yAxis2,zAxis2);
//	*this = *this * rotInv;
//}
Vector3 Quaternion::transform(const Vector3 &in) const
{
	/*
	Matrix4 mat;
	mat.fromQuaternion(*this);
	return mat.transform(in);
	*/

	// nVidia SDK implementation
	Vector3 uv, uuv; 
	Vector3 qvec(x, y, z);
	uv = qvec.cross(in); 
	uuv = qvec.cross(uv); 
	uv *= (2.0f * w); 
	uuv *= 2.0f; 

	return in + uv + uuv;
}

//Vector4 Quaternion::transform(const Vector4 &in) const
//{
//	Matrix4 mat;
//	mat.fromQuaternion(*this);
//	return mat.transform(in);
//}

/*
Logarithm of a Quaternion, given as:
Qlog(q) = v*a where q = [cos(a),v*sin(a)]
*/
Quaternion Quaternion::log(const Quaternion &q)
{
	Real a = static_cast<Real>(acos(q.w));
	Real sina = static_cast<Real>(sin(a));
	Quaternion ret;
	ret.w = 0;
	if (sina > 0)
	{
		ret.x = a*q.x/sina;
		ret.y = a*q.y/sina;
		ret.z = a*q.z/sina;
	}
	else
	{
		ret.x=ret.y=ret.z=0;
	}

	return ret;
}

/*
e^Quaternion given as:
Qexp(v*a) = [cos(a),vsin(a)]
*/
Quaternion Quaternion::exp(const Quaternion &q)
{
	Real a = static_cast<Real>(sqrt(q.x*q.x + q.y*q.y + q.z*q.z));
	Real sina = static_cast<Real>(sin(a));
	Real cosa = static_cast<Real>(cos(a));
	Quaternion ret;

	ret.w = cosa;
	if(a > 0)
	{
		ret.x = sina * q.x / a;
		ret.y = sina * q.y / a;
		ret.z = sina * q.z / a;
	}
	else
	{
		ret.x = ret.y = ret.z = 0;
	}

	return ret;
}

/*
Linear interpolation between two Quaternions
	note: this method does not check theta > 180. so the returns may not be the shortest arc.
*/
void Quaternion::lerp(const Quaternion &q1,const Quaternion &q2,Real t)
{
	Quaternion ret;
	ret = q1 + t*(q2-q1);
	/*
	return value must be normalized
	*/
	ret.normalize();

	*this = ret;
}

/*
Spherical linear interpolation between two Quaternions
*/
void Quaternion::slerp(const Quaternion &q1,const Quaternion &q2,Real t)
{
	/*Quaternion q3;
	D3DXQuaternionSlerp((D3DXQUATERNION *)this,(const D3DXQUATERNION *)&q1,(const D3DXQUATERNION *)&q2,t);*/


	Quaternion q3;
	Real dot;
	dot = q1.dot(q2);

	/*
	dot = cos(theta)
	if (dot < 0), q1 and q2 are more than 180 degrees apart,
	so we can invert one to reduce spinning
	*/
	if (dot < 0)
	{
		dot = -dot;
		q3 = -1*q2;
	}
	else
	{
		q3 = q2;
	}


	if (dot < 0.95f)
	{
		Real angle = static_cast<Real>(acos(dot));
		Real sina,sinat,sinaomt;
		sina = static_cast<Real>(sin(angle));
		sinat = static_cast<Real>(sin(angle*t));
		sinaomt = static_cast<Real>(sin(angle*(1-t)));
		*this =  (q1*sinaomt+q3*sinat)/sina;
	}
	/*
	if the angle is small, use linear interpolation
	*/
	else
	{
		this->lerp(q1,q3,t);
	}
}

/*
This version of slerp, used by squad, does not check for theta > 180.
*/

Quaternion QslerpNoInvert(const Quaternion &q1,const Quaternion &q2,Real t)
{
	Real dot = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	Quaternion ret;

	if (dot > -0.95f && dot < 0.95f)
	{
		Real angle = static_cast<Real>(acos(dot));
		Real sina,sinat,sinaomt;
		sina = static_cast<Real>(sin(angle));
		sinat = static_cast<Real>(sin(angle*t));
		sinaomt = static_cast<Real>(sin(angle*(1-t)));
		ret = (q1*sinaomt+q2*sinat)/sina;
	}
	/*
	if the angle is small, use linear interpolation
	*/
	else
	{
		ret.lerp(q1,q2,t);
	}

	return ret;
}


/*
Spherical cubic interpolation
*/
void Quaternion::squad(const Quaternion &q1,const Quaternion &q2,const Quaternion &a,const Quaternion &b,Real t)
{
	Quaternion c,d;
	c = QslerpNoInvert(q1,q2,t);
	d = QslerpNoInvert(a,b,t);
	*this = QslerpNoInvert(c,d,2*t*(1-t));
}


/*
Given 3 Quaternions, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
*/

void Quaternion::spline(const Quaternion &qnm1,const Quaternion &qn,const Quaternion &qnp1)
{
	Quaternion qni;

	qni.x = -qn.x;
	qni.y = -qn.y;
	qni.z = -qn.z;
	qni.w = qn.w;

	*this = qn*exp((log(qni*qnm1)+log(qni*qnp1))/-4);
}

void Quaternion::fromEuler(const Vector3 &eular)
{
	Quaternion out, temp;

	out.fromAxisAngle(Vector3::UNIT_X, eular.x);

	temp.fromAxisAngle(Vector3::UNIT_Y, eular.y);
	out = temp*out;

	temp.fromAxisAngle(Vector3::UNIT_Z, eular.z);
	out = temp*out;

	*this = out;
}

void Quaternion::toYawPitchRoll(float & yaw,float & pitch,float & roll)
{
	roll = asin(-2*(x*z - w*y));
	pitch = atan2(2*(y*z + w*x), w*w - x*x - y*y + z*z);
	yaw = atan2(2*(x*y + w*z), w*w + x*x - y*y - z*z);
}

OO_NAMESPACE_END
