//// This file is part of Origo Engine.
//// Copyright (C), Beijing Tuyasoft Co., Ltd.
//#ifndef OO_MATRIX4_UTIL_H_
//#define OO_MATRIX4_UTIL_H_
//
//#include "../Common/v2ObjStd.h"
//
//OO_NAMESPACE_BEGIN
//class Matrix4;
//class Quaternion;
//class Plane;
//class Vector3;
//
////OO_API Real Matrix4fDeterminant
////    ( const Matrix4 *pM );
////
////OO_API Matrix4* Matrix4Transpose
////    ( Matrix4 *pOut, const Matrix4 *pM );
//
//// Matrix multiplication.  The result represents the transformation M2
//// followed by the transformation M1.  (Out = M1 * M2)
//OO_API Matrix4* Matrix4Multiply
//    ( Matrix4 *pOut, const Matrix4 *pM1, const Matrix4 *pM2 );
//
////// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
////OO_API Matrix4* Matrix4MultiplyTranspose
////    ( Matrix4 *pOut, const Matrix4 *pM1, const Matrix4 *pM2 );
//
//// Calculate inverse of matrix.  Inversion my fail, in which case 0 will
//// be returned.  The determinant of pM is also returned it pfDeterminant
//// is non-0.
//OO_API Matrix4* Matrix4Inverse
//    ( Matrix4 *pOut, Real *pDeterminant, const Matrix4 *pM );
//
////// Build a matrix which scales by (sx, sy, sz)
////OO_API Matrix4* Matrix4Scaling
////    ( Matrix4 *pOut, Real sx, Real sy, Real sz );
//
////// Build a matrix which translates by (x, y, z)
////OO_API Matrix4* Matrix4Translation
////    ( Matrix4 *pOut, Real x, Real y, Real z );
//
//// Build a matrix from a quaternion
//OO_API Matrix4* Matrix4RotationQuaternion
//    ( Matrix4 *pOut, const Quaternion *pQ);
//
//// Yaw around the Y axis, a pitch around the X axis,
//// and a roll around the Z axis.
//OO_API Matrix4* Matrix4RotationYawPitchRoll
//    ( Matrix4 *pOut, Real Yaw, Real Pitch, Real Roll );
//
//
//// Build transformation matrix.  0 arguments are treated as identity.
//// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
//OO_API Matrix4* Matrix4Transformation
//    ( Matrix4 *pOut, const Vector3 *pScalingCenter,
//      const Quaternion *pScalingRotation, const Vector3 *pScaling,
//      const Vector3 *pRotationCenter, const Quaternion *pRotation,
//      const Vector3 *pTranslation);
//
//// Build affine transformation matrix.  0 arguments are treated as identity.
//// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
//OO_API Matrix4* Matrix4AffineTransformation
//    ( Matrix4 *pOut, Real Scaling, const Vector3 *pRotationCenter,
//      const Quaternion *pRotation, const Vector3 *pTranslation);
//
//// Build a lookat matrix. (right-handed)
//OO_API Matrix4* Matrix4LookAtRH
//    ( Matrix4 *pOut, const Vector3 *pEye, const Vector3 *pAt,
//      const Vector3 *pUp );
//
//// Build a lookat matrix. (left-handed)
//OO_API Matrix4* Matrix4LookAtLH
//    ( Matrix4 *pOut, const Vector3 *pEye, const Vector3 *pAt,
//      const Vector3 *pUp );
//
//// Build a perspective projection matrix. (right-handed)
//OO_API Matrix4* Matrix4PerspectiveRH
//    ( Matrix4 *pOut, Real w, Real h, Real zn, Real zf );
//
//// Build a perspective projection matrix. (left-handed)
//OO_API Matrix4* Matrix4PerspectiveLH
//    ( Matrix4 *pOut, Real w, Real h, Real zn, Real zf );
//
//// Build a perspective projection matrix. (right-handed)
//OO_API Matrix4* Matrix4PerspectiveFovRH
//    ( Matrix4 *pOut, Real fovy, Real Aspect, Real zn, Real zf );
//
//// Build a perspective projection matrix. (left-handed)
//OO_API Matrix4* Matrix4PerspectiveFovLH
//    ( Matrix4 *pOut, Real fovy, Real Aspect, Real zn, Real zf );
//
//// Build a perspective projection matrix. (right-handed)
//OO_API Matrix4* Matrix4PerspectiveOffCenterRH
//    ( Matrix4 *pOut, Real l, Real r, Real b, Real t, Real zn,
//      Real zf );
//
//// Build a perspective projection matrix. (left-handed)
//OO_API Matrix4* Matrix4PerspectiveOffCenterLH
//    ( Matrix4 *pOut, Real l, Real r, Real b, Real t, Real zn,
//      Real zf );
//
//// Build an ortho projection matrix. (right-handed)
//OO_API Matrix4* Matrix4OrthoRH
//    ( Matrix4 *pOut, Real w, Real h, Real zn, Real zf );
//
//// Build an ortho projection matrix. (left-handed)
//OO_API Matrix4* Matrix4OrthoLH
//    ( Matrix4 *pOut, Real w, Real h, Real zn, Real zf );
//
//// Build an ortho projection matrix. (right-handed)
//OO_API Matrix4* Matrix4OrthoOffCenterRH
//    ( Matrix4 *pOut, Real l, Real r, Real b, Real t, Real zn,
//      Real zf );
//
//// Build an ortho projection matrix. (left-handed)
//OO_API Matrix4* Matrix4OrthoOffCenterLH
//    ( Matrix4 *pOut, Real l, Real r, Real b, Real t, Real zn,
//      Real zf );
//
///*
//// Build a matrix which flattens geometry into a plane, as if casting
//// a shadow from a light.
//OO_API Matrix4* Matrix4Shadow
//    ( Matrix4 *pOut, const D3DXVECTOR4 *pLight,
//      const D3DXPLANE *pPlane );
//
//// Build a matrix which reflects the coordinate system about a plane
//OO_API Matrix4* Matrix4Reflect
//    ( Matrix4 *pOut, const D3DXPLANE *pPlane );
//*/
//
/////////////////////////////////////////////////////////////////////////////////
//
//OO_NAMESPACE_END
//
//#endif // #ifndef OO_MATRIX4_UTIL_H_
