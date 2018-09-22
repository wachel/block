//// This file is part of Origo Engine.
//// Copyright (C), Beijing Tuyasoft Co., Ltd.
////#include "v2Standard.h"
#include "../Common/v2Config.h"
#include "../Common/v2ObjStd.h"
#include "v2Matrix4.h"
//#include "v2Vector3.h"
//#include "v2MathCommon.h"
//#include "v2Quaternion.h"
//
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//#include "my_d3dx9math.h"
//#elif OO_PLATFORM == OO_PLATFORM_MAC || OO_PLATFORM == OO_PLATFORM_IOS
//#include <GLKit/GLKMath.h>
//#else // android, else
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#endif
//
OO_NAMESPACE_BEGIN
//
//#if OO_PLATFORM == OO_PLATFORM_MAC || OO_PLATFORM == OO_PLATFORM_IOS
//static inline GLKMatrix4* toGLKMatrix4(Matrix4 *m)
//{
//	return (GLKMatrix4*)m;
//}
//static inline GLKMatrix4& toGLKMatrix4(Matrix4 &m)
//{
//	return (GLKMatrix4&)m;
//}
//static inline Matrix4* toMatrix4(GLKMatrix4 *m)
//{
//	return (Matrix4*)m;
//}
//static inline Matrix4& toMatrix4(GLKMatrix4 &m)
//{
//	return (Matrix4&)m;
//}
//#endif
//
//#if OO_PLATFORM == OO_PLATFORM_ANDROID
//static inline glm::mat4* toGLMMatrix4(Matrix4 *m)
//{
//	return (glm::mat4*)m;
//}
//static inline glm::mat4& toGLMMatrix4(Matrix4 &m)
//{
//	return (glm::mat4&)m;
//}
//static inline Matrix4* toMatrix4(glm::mat4 *m)
//{
//	return (Matrix4*)m;
//}
//static inline Matrix4& toMatrix4(glm::mat4 &m)
//{
//	return (Matrix4&)m;
//}
//#endif
//
const Matrix4 Matrix4::ZERO(0, 0, 0, 0,
							0, 0, 0, 0,
							0, 0, 0, 0,
							0, 0, 0, 0);

const Matrix4 Matrix4::IDENTITY(1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1);

//void Matrix4::makeShadowByDirectionalLight(const Vector3 &lightDir, const Plane& plane)
//{
//	Vector4 L(lightDir.x, lightDir.y, lightDir.z, 0.f);
//	
//	Plane P = plane;
//	P.normalize();
//
//	Real d = P.dot(L);
//
//	set(P.normal.x * L.x - d, P.normal.y * L.x, P.normal.z * L.x, -P.dist * L.x,
//        P.normal.x * L.y, P.normal.y * L.y - d, P.normal.z * L.y, -P.dist * L.y,
//        P.normal.x * L.z, P.normal.y * L.z, P.normal.z * L.z - d, -P.dist * L.z,
//        P.normal.x * L.w, P.normal.y * L.w, P.normal.z * L.w, -P.dist * L.w - d);
//}
//
//void Matrix4::makeShadowByPointLight(const Vector3 &lightPos, const Plane& plane)
//{
//	// !!! NOT TESTED.:(
//
//	Vector4 L(lightPos.x, lightPos.y, lightPos.z, 1.f);
//	
//	Plane P = plane;
//	P.normalize();
//
//	Real d = P.dot(L);
//
//	set(P.normal.x * L.x - d, P.normal.y * L.x, P.normal.z * L.x, -P.dist * L.x,
//        P.normal.x * L.y, P.normal.y * L.y - d, P.normal.z * L.y, -P.dist * L.y,
//        P.normal.x * L.z, P.normal.y * L.z, P.normal.z * L.z - d, -P.dist * L.z,
//        P.normal.x * L.w, P.normal.y * L.w, P.normal.z * L.w, -P.dist * L.w - d);
//}
//
//void Matrix4::makeRotationVectorToVector(const Vector3 &from, const Vector3 &to)
//{
//	Quaternion q;
//	q.fromVectorToVector(from, to);
//	fromQuaternion(q);
//}
//
//void Matrix4::makeRotationAt(const Vector3 &pt, const Vector3 &axis, Real angle)
//{
//	Matrix4 matT, matR, mat_T;
//	matT.makeTranslation(-pt);
//	matR.makeRotationAxis(axis, angle);
//	mat_T.makeTranslation(pt);
//    *this = mat_T*matR*matT;
//}
//
//void Matrix4::fromQuaternion(const Quaternion &q)
//{
//	/*
//	// MUST! invert rotation axis since D3D use right-handle coords.
//	// to D3D quaternion. 
//	// see Quaternion::fromRotationMatrix.
//	Quaternion rot = q;
//	//rot.x = - rot.x;
//	//rot.y = - rot.y;
//	//rot.z = - rot.z;
//
//	D3DXMatrixRotationQuaternion((D3DXMATRIX*)this, (const D3DXQUATERNION*)&rot);
//	*/
//
//	Real fTx  = 2.0*q.x;
//	Real fTy  = 2.0*q.y;
//	Real fTz  = 2.0*q.z;
//	Real fTwx = fTx*q.w;
//	Real fTwy = fTy*q.w;
//	Real fTwz = fTz*q.w;
//	Real fTxx = fTx*q.x;
//	Real fTxy = fTy*q.x;
//	Real fTxz = fTz*q.x;
//	Real fTyy = fTy*q.y;
//	Real fTyz = fTz*q.y;
//	Real fTzz = fTz*q.z;
//
//	*this = IDENTITY;
//
//	m[0][0] = 1.0-(fTyy+fTzz);
//	m[0][1] = fTxy-fTwz;
//	m[0][2] = fTxz+fTwy;
//	m[1][0] = fTxy+fTwz;
//	m[1][1] = 1.0-(fTxx+fTzz);
//	m[1][2] = fTyz-fTwx;
//	m[2][0] = fTxz-fTwy;
//	m[2][1] = fTyz+fTwx;
//	m[2][2] = 1.0-(fTxx+fTyy);
//
//}
//
//void Matrix4::makeReflection(const Plane &p)
//{
//	// see "Real-Time Rendering", p162.
//
//	Matrix4 F, R, T;
//	T.makeTranslation(-p.getPointOnPlane());
//	R.makeRotationVectorToVector(p.normal, Vector3::UNIT_Y);
//	F = R*T;
//
//	Matrix4 S;
//	S.makeScaling(1, -1, 1);
//
//	Matrix4 _F = F;
//	_F.inverse();
//
//	*this = _F*S*F;
//}
//
//void Matrix4::inverse()
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	D3DXMatrixInverse( (D3DXMATRIX*)this, 0, (const D3DXMATRIX*)this);
//#elif OO_PLATFORM == OO_PLATFORM_MAC || OO_PLATFORM == OO_PLATFORM_IOS
//	GLKMatrix4 ret;
//	bool bInvertable;
//	ret = GLKMatrix4Invert(*toGLKMatrix4(this), &bInvertable);
//	//ooAssert(bInvertable); 至少Camera::updateView 里面会faild，先忽略
//	*this = *toMatrix4(&ret);
//#else
//	glm::mat4 ret = glm::inverse(*toGLMMatrix4(this));
//	*this = *toMatrix4(&ret);
//#endif
//}
//
//void Matrix4::makeRotationAxis(const Vector3& axis, Real angle)
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	D3DXMatrixRotationAxis( (D3DXMATRIX*)this, (const D3DXVECTOR3*)(&axis), -angle);
//#elif OO_PLATFORM == OO_PLATFORM_MAC || OO_PLATFORM == OO_PLATFORM_IOS
//	// 测试代码证明要用"-angle",原因不明。
//	GLKMatrix4 m = GLKMatrix4MakeRotation(-angle,axis.x,axis.y,axis.z);
//	*this = toMatrix4(m);
//#else
//
//#ifndef GLM_FORCE_RADIANS
//#	error "glm: GLM_FORCE_RADIANS 必须开启，见"glm/core/setup.hpp""
//#endif
//	glm::mat4 ret = glm::rotate(glm::mat4(1.f), -angle, glm::vec3(axis.x, axis.y, axis.z));
//	*this = toMatrix4(ret);
//#endif
//}
//
//// Build a matrix which rotates around the X axis
//void Matrix4::makeRotationX(Real angle)
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	D3DXMatrixRotationX( (D3DXMATRIX*)this, -angle);
//#elif OO_PLATFORM == OO_PLATFORM_MAC || OO_PLATFORM == OO_PLATFORM_IOS
//	GLKMatrix4 m = GLKMatrix4MakeXRotation(-angle);
//	*this = toMatrix4(m);
//#else
//	glm::mat4 ret = glm::rotate(glm::mat4(1.f), -angle, glm::vec3(1.f, 0.f, 0.f));
//	*this = toMatrix4(ret);
//#endif
//}
//
//// Build a matrix which rotates around the Y axis
//void Matrix4::makeRotationY(Real angle)
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	D3DXMatrixRotationY( (D3DXMATRIX*)this, -angle);
//#elif OO_PLATFORM == OO_PLATFORM_MAC || OO_PLATFORM == OO_PLATFORM_IOS
//	GLKMatrix4 m = GLKMatrix4MakeYRotation(-angle);
//	*this = toMatrix4(m);
//#else
//	glm::mat4 ret = glm::rotate(glm::mat4(1.f), -angle, glm::vec3(0.f, 1.f, 0.f));
//	*this = toMatrix4(ret);
//#endif
//}
//
//// Build a matrix which rotates around the Z axis
//void Matrix4::makeRotationZ(Real angle)
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	D3DXMatrixRotationZ( (D3DXMATRIX*)this, -angle);
//#elif OO_PLATFORM == OO_PLATFORM_MAC || OO_PLATFORM == OO_PLATFORM_IOS
//	GLKMatrix4 m = GLKMatrix4MakeZRotation(-angle);
//	*this = toMatrix4(m);
//#else
//	glm::mat4 ret = glm::rotate(glm::mat4(1.f), -angle, glm::vec3(0.f, 0.f, 1.f));
//	*this = toMatrix4(ret);
//#endif
//}
//
//
OO_NAMESPACE_END
