//// This file is part of Origo Engine.
//// Copyright (C), Beijing Tuyasoft Co., Ltd.
////#include "v2Standard.h"
//#include "v2Matrix4Util.h"
//#include "v2Matrix4.h"
//#include "..\Common\v2Config.h"
//
//#if OO_COMPILER == OO_COMPILER_MSVC
//#include "my_d3dx9math.h"
//#elif OO_PLATFORM == OO_PLATFORM_MAC || OO_PLATFORM == OO_PLATFORM_IOS
//#include <GLKit/GLKMath.h>
//#else // android, else
//#include "glm/glm.hpp"
//#endif
//
//
//OO_NAMESPACE_BEGIN
//
//
//#if OO_PLATFORM == OO_PLATFORM_MAC ||OO_PLATFORM == OO_PLATFORM_IOS
//static inline GLKMatrix4* toGLKMatrix4(Matrix4 *m)
//{
//	return (GLKMatrix4*)m;
//}
//static inline GLKMatrix4& toGLKMatrix4(Matrix4 &m)
//{
//	return (GLKMatrix4&)m;
//}
//static inline const GLKMatrix4* toGLKMatrix4(const Matrix4 *m)
//{
//	return (const GLKMatrix4*)m;
//}
//static inline const GLKMatrix4& toGLKMatrix4(const Matrix4 &m)
//{
//	return (const GLKMatrix4&)m;
//}
//
//static inline Matrix4* toMatrix4(GLKMatrix4 *m)
//{
//	return (Matrix4*)m;
//}
//static inline Matrix4& toMatrix4(GLKMatrix4 &m)
//{
//	return (Matrix4&)m;
//}
//static inline const Matrix4* toMatrix4(const GLKMatrix4 *m)
//{
//	return (const Matrix4*)m;
//}
//static inline const Matrix4& toMatrix4(const GLKMatrix4 &m)
//{
//	return (const Matrix4&)m;
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
//static inline const glm::mat4& toGLMMatrix4(const Matrix4 &m)
//{
//	return (const glm::mat4&)m;
//}
//static inline Matrix4* toMatrix4(glm::mat4 *m)
//{
//	return (Matrix4*)m;
//}
//static inline const Matrix4* toMatrix4(const glm::mat4 *m)
//{
//	return (const Matrix4*)m;
//}
//static inline Matrix4& toMatrix4(glm::mat4 &m)
//{
//	return (Matrix4&)m;
//}
//static inline const Matrix4& toMatrix4(const glm::mat4 &m)
//{
//	return (const Matrix4&)m;
//}
//#endif
//
///*
// // Build a matrix which flattens geometry into a plane, as if casting
// // a shadow from a light.
// Matrix4* Matrix4Shadow
// ( Matrix4 *pOut, const D3DXVECTOR4 *pLight,
// const D3DXPLANE *pPlane )
// {
// return (Matrix4*)D3DXMatrixShadow
// ( (D3DXMATRIX*)pOut, const D3DXVECTOR4 *pLight,
// const D3DXPLANE *pPlane );
// }
// 
// // Build a matrix which reflects the coordinate system about a plane
// Matrix4* Matrix4Reflect
// ( Matrix4 *pOut, const D3DXPLANE *pPlane )
// {
// return (Matrix4*)D3DXMatrixReflect
// ( (D3DXMATRIX*)pOut, const D3DXPLANE *pPlane );
// }
// */
//
////OO_API Real Matrix4fDeterminant
////( const Matrix4 *pM )
////{
////#if OO_PLATFORM == OO_PLATFORM_WIN32
////	return D3DXMatrixDeterminant((D3DXMATRIX*)pM);
////#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
////	OO_UNIMPLEMENTED_METHOD;
////	return 0;
////#else
////#	error unfinish
////#endif
////}
//
////OO_API Matrix4* Matrix4Transpose
////( Matrix4 *pOut, const Matrix4 *pM )
////{
////#if OO_PLATFORM == OO_PLATFORM_WIN32
////	return (Matrix4*)D3DXMatrixTranspose((D3DXMATRIX*)pOut, (const D3DXMATRIX*)pM);
////#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
////	OO_UNIMPLEMENTED_METHOD;
////	return pOut;
////#else
////#	error unfinish
////#endif
////}
//
//// Matrix multiplication.  The result represents the transformation M2
//// followed by the transformation M1.  (Out = M1 * M2)
//OO_API Matrix4* Matrix4Multiply
//( Matrix4 *pOut, const Matrix4 *pM1, const Matrix4 *pM2 )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixMultiply( (D3DXMATRIX*)pOut, (const D3DXMATRIX*)pM1, (const D3DXMATRIX*)pM2 );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	*pOut = toMatrix4(GLKMatrix4Multiply(*toGLKMatrix4(pM2), *toGLKMatrix4(pM1)));
//	return pOut;
//#else
//	*pOut = toMatrix4(toGLMMatrix4(*pM2)*toGLMMatrix4(*pM1));
//	return pOut;
//#endif
//}
//
////// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
////OO_API Matrix4* Matrix4MultiplyTranspose
////( Matrix4 *pOut, const Matrix4 *pM1, const Matrix4 *pM2 )
////{
////#if OO_PLATFORM == OO_PLATFORM_WIN32
////	return (Matrix4*)D3DXMatrixMultiplyTranspose( (D3DXMATRIX*)pOut, (const D3DXMATRIX*)pM1, (const D3DXMATRIX*)pM2 );
////#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
////	OO_UNIMPLEMENTED_METHOD;
////	return pOut;
////#else
////#	error unfinish
////#endif
////}
//
//// Calculate inverse of matrix.  Inversion my fail, in which case 0 will
//// be returned.  The determinant of pM is also returned it pfDeterminant
//// is non-0.
//OO_API Matrix4* Matrix4Inverse
//( Matrix4 *pOut, Real *pDeterminant, const Matrix4 *pM )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixInverse( (D3DXMATRIX*)pOut, pDeterminant, (const D3DXMATRIX*)pM );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	ooAssert(!pDeterminant); // don't support
//	GLKMatrix4 ret;
//	bool bInvertable;
//	ret = GLKMatrix4Invert(*toGLKMatrix4(pM), &bInvertable);
//	if (!bInvertable)
//		return 0;
//	*pOut = *toMatrix4(&ret);
//	return pOut;
//#else
//	ooAssert(!pDeterminant); // don't support
//	*pOut = toMatrix4(glm::inverse(toGLMMatrix4(*pM)));
//	return pOut;
//#endif
//	
//}
//
////// Build a matrix which scales by (sx, sy, sz)
////OO_API Matrix4* Matrix4Scaling
////( Matrix4 *pOut, Real sx, Real sy, Real sz )
////{
////#if OO_PLATFORM == OO_PLATFORM_WIN32
////	return (Matrix4*)D3DXMatrixScaling( (D3DXMATRIX*)pOut, sx, sy, sz );
////#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
////	OO_UNIMPLEMENTED_METHOD;
////	return pOut;
////#else
////#	error unfinish
////#endif
////}
////
////// Build a matrix which translates by (x, y, z)
////OO_API Matrix4* Matrix4Translation
////( Matrix4 *pOut, Real x, Real y, Real z )
////{
////#if OO_PLATFORM == OO_PLATFORM_WIN32
////	return (Matrix4*)D3DXMatrixTranslation( (D3DXMATRIX*)pOut, x, y, z );
////#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
////	OO_UNIMPLEMENTED_METHOD;
////	return pOut;
////#else
////#	error unfinish
////#endif
////}
//
//// Build a matrix from a quaternion
//OO_API Matrix4* Matrix4RotationQuaternion
//( Matrix4 *pOut, const Quaternion *pQ)
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixRotationQuaternion( (D3DXMATRIX*)pOut, (const D3DXQUATERNION *)pQ);
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Yaw around the Y axis, a pitch around the X axis,
//// and a roll around the Z axis.
//OO_API Matrix4* Matrix4RotationYawPitchRoll
//( Matrix4 *pOut, Real Yaw, Real Pitch, Real Roll )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixRotationYawPitchRoll( (D3DXMATRIX*)pOut, Yaw, Pitch, Roll );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//
//// Build transformation matrix.  0 arguments are treated as identity.
//// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
//OO_API Matrix4* Matrix4Transformation
//( Matrix4 *pOut, const Vector3 *pScalingCenter,
// const Quaternion *pScalingRotation, const Vector3 *pScaling,
// const Vector3 *pRotationCenter, const Quaternion *pRotation,
// const Vector3 *pTranslation)
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixTransformation( (D3DXMATRIX*)pOut, (const D3DXVECTOR3*)pScalingCenter,
//											  (const D3DXQUATERNION *)pScalingRotation, (const D3DXVECTOR3*)pScaling,
//											  (const D3DXVECTOR3*)pRotationCenter, (const D3DXQUATERNION *)pRotation,
//											  (const D3DXVECTOR3*)pTranslation);
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build affine transformation matrix.  0 arguments are treated as identity.
//// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
//OO_API Matrix4* Matrix4AffineTransformation
//( Matrix4* pOut, Real Scaling, const Vector3 *pRotationCenter,
// const Quaternion *pRotation, const Vector3 *pTranslation)
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixAffineTransformation( (D3DXMATRIX*)pOut, Scaling, (const D3DXVECTOR3*)pRotationCenter,
//													(const D3DXQUATERNION *)pRotation, (const D3DXVECTOR3*)pTranslation);
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build a lookat matrix. (right-handed)
//OO_API Matrix4* Matrix4LookAtRH
//( Matrix4 *pOut, const Vector3 *pEye, const Vector3 *pAt,
// const Vector3 *pUp )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixLookAtRH( (D3DXMATRIX*)pOut, (const D3DXVECTOR3*)pEye, (const D3DXVECTOR3*)pAt,
//										(const D3DXVECTOR3*)pUp );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build a lookat matrix. (left-handed)
//OO_API Matrix4* Matrix4LookAtLH
//( Matrix4 *pOut, const Vector3 *pEye, const Vector3 *pAt,
// const Vector3 *pUp )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixLookAtLH( (D3DXMATRIX*)pOut, (const D3DXVECTOR3*)pEye, (const D3DXVECTOR3*)pAt,
//										(const D3DXVECTOR3*)pUp );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build a perspective projection matrix. (right-handed)
//OO_API Matrix4* Matrix4PerspectiveRH
//( Matrix4 *pOut, Real w, Real h, Real zn, Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixPerspectiveRH( (D3DXMATRIX*)pOut, w, h, zn, zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build a perspective projection matrix. (left-handed)
//OO_API Matrix4* Matrix4PerspectiveLH
//( Matrix4 *pOut, Real w, Real h, Real zn, Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixPerspectiveLH( (D3DXMATRIX*)pOut, w, h, zn, zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build a perspective projection matrix. (right-handed)
//OO_API Matrix4* Matrix4PerspectiveFovRH
//( Matrix4 *pOut, Real fovy, Real Aspect, Real zn, Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixPerspectiveFovRH( (D3DXMATRIX*)pOut, fovy, Aspect, zn, zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build a perspective projection matrix. (left-handed)
//OO_API Matrix4* Matrix4PerspectiveFovLH
//( Matrix4 *pOut, Real fovy, Real Aspect, Real zn, Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*) D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)pOut, fovy, Aspect, zn, zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build a perspective projection matrix. (right-handed)
//OO_API Matrix4* Matrix4PerspectiveOffCenterRH
//( Matrix4 *pOut, Real l, Real r, Real b, Real t, Real zn,
// Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixPerspectiveOffCenterRH( (D3DXMATRIX*)pOut, l, r, b, t, zn,
//													  zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build a perspective projection matrix. (left-handed)
//OO_API Matrix4* Matrix4PerspectiveOffCenterLH
//( Matrix4 *pOut, Real l, Real r, Real b, Real t, Real zn,
// Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixPerspectiveOffCenterLH( (D3DXMATRIX*)pOut, l, r, b, t, zn,
//													  zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build an ortho projection matrix. (right-handed)
//OO_API Matrix4* Matrix4OrthoRH
//( Matrix4 *pOut, Real w, Real h, Real zn, Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixOrthoRH( (D3DXMATRIX*)pOut, w, h, zn, zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build an ortho projection matrix. (left-handed)
//OO_API Matrix4* Matrix4OrthoLH
//( Matrix4 *pOut, Real w, Real h, Real zn, Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixOrthoLH( (D3DXMATRIX*)pOut, w, h, zn, zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build an ortho projection matrix. (right-handed)
//OO_API Matrix4* Matrix4OrthoOffCenterRH
//( Matrix4 *pOut, Real l, Real r, Real b, Real t, Real zn,
// Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixOrthoOffCenterRH( (D3DXMATRIX*)pOut, l, r, b, t, zn,
//												zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//// Build an ortho projection matrix. (left-handed)
//OO_API Matrix4* Matrix4OrthoOffCenterLH
//( Matrix4 *pOut, Real l, Real r, Real b, Real t, Real zn,
// Real zf )
//{
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	return (Matrix4*)D3DXMatrixOrthoOffCenterLH
//	( (D3DXMATRIX*)pOut, l, r, b, t, zn,
//	 zf );
//#elif OO_PLATFORM == OO_PLATFORM_IOS || OO_PLATFORM == OO_PLATFORM_MAC
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#else
//	OO_UNIMPLEMENTED_METHOD;
//	return pOut;
//#endif
//}
//
//
//OO_NAMESPACE_END
