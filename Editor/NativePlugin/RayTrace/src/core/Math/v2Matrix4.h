//// This file is part of Origo Engine.
//// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_MATRIX4_H_
#define OO_MATRIX4_H_
//
//#include "v2Matrix4Util.h"
#include "v2Vector3.h"
////#include "v2Vector4.h"
//#include "../Math2/Vector4.h"
//
OO_NAMESPACE_BEGIN
//
///** 4x4����
//
//	Matrix4�������¹��б�����
//	@code
//    union {
//        struct {
//            Real        _11, _12, _13, _14;
//            Real        _21, _22, _23, _24;
//            Real        _31, _32, _33, _34;
//            Real        _41, _42, _43, _44;
//        };
//        Real m[4][4];
//    };
//	@endcode
//
//	�������ʹ�����������󣬳˷�˳�������Һ��󡣱��� M2*M1 ��ʾ����M1��
//	ת������M2��ת����
//
//*/
//class Vector3;
//class Plane;
class OO_API Matrix4
{
public:
	// ����Ԫ���б�
    union {
        struct {
            Real        _11, _12, _13, _14;
            Real        _21, _22, _23, _24;
            Real        _31, _32, _33, _34;
            Real        _41, _42, _43, _44;
        };
        Real m[4][4];
    };

	//@{ 
	/// ������
	static const Matrix4 ZERO;
	static const Matrix4 IDENTITY;
	//@}

public:
	
	//@{ 
	/// ���캯����
    Matrix4() {};
    Matrix4( const Real * );
    Matrix4( const Matrix4& );
    Matrix4( Real _11, Real _12, Real _13, Real _14,
		Real _21, Real _22, Real _23, Real _24,
		Real _31, Real _32, Real _33, Real _34,
		Real _41, Real _42, Real _43, Real _44 );
	//@}

	void set( Real _11, Real _12, Real _13, Real _14,
		Real _21, Real _22, Real _23, Real _24,
		Real _31, Real _32, Real _33, Real _34,
		Real _41, Real _42, Real _43, Real _44 );
//
//	//@{ 
//	/// ���ʵ���Ԫ�ء�
//    Real& operator () ( Uint Row, Uint Col );
//    Real  operator () ( Uint Row, Uint Col ) const;
//	//@}
//
//    //@{ 
//	/// ����ת��������
//    operator Real* ();
//    operator const Real* () const;
//	//@}
//
//    //@{ 
//	/// ��ֵ������
//    Matrix4& operator *= ( const Matrix4& );
//    Matrix4& operator += ( const Matrix4& );
//    Matrix4& operator -= ( const Matrix4& );
//    Matrix4& operator *= ( Real );
//    Matrix4& operator /= ( Real );
//	//@}
//
//	//@{
//	/// һԪ������
//    Matrix4 operator + () const;
//    Matrix4 operator - () const;
//	//@}
//
//	//@{
//	/// ��Ԫ������
//    Matrix4 operator * ( const Matrix4& ) const;
//    Matrix4 operator + ( const Matrix4& ) const;
//    Matrix4 operator - ( const Matrix4& ) const;
//    Matrix4 operator * ( Real ) const;
//    Matrix4 operator / ( Real ) const;
//	friend Matrix4 operator * ( Real, const Matrix4& );
//	//@}
//
//	//@{
//	/// �Ƚϡ�
//	bool operator == ( const Matrix4& ) const;
//    bool operator != ( const Matrix4& ) const;
//	//@}
//
//	/// ����Ϊȫ0��
//	void zero() { memset(this, 0, sizeof(Matrix4)); }
//
//	/// ����Ϊ��׼����
//	void identity() { *this = Matrix4::IDENTITY; }
//
//	/// Transform (x, y, z, 1) by matrix, project result back into w=1.
//	inline Vector3 transform(const Vector3 &in) const;
//
//	/// Transform (x, y, z, 0) by matrix, project result back into w=1.
//	inline Vector3 transformNormal(const Vector3 &in) const;
//
//	inline Vector4 transform(const Vector4 &in) const;
//
//	/// ת�á�
//	inline void transpose();
//
//	/// ��Ϊ�����
//	void inverse();
//
//	//@{
//	/// ����ƽ�ƾ���
//	inline void makeTranslation(Real x, Real y, Real z);
//	inline void makeTranslation(const Vector3& v) {
//		makeTranslation(v.x, v.y, v.z);
//	}
//	//@}
//
//	//@{
//	/// ��ȡƽ�ơ�
//	inline void getTranslation(Real& x, Real& y, Real& z) const;
//	inline void getTranslation(Vector3& v) const {
//		getTranslation(v.x, v.y, v.z);
//	}
//	//@}
//
//	//@{
//	/// �������ž���
//	inline void makeScaling(Real x, Real y, Real z) {
//		*this = Matrix4::IDENTITY;
//		_11 = x;
//		_22 = y;
//		_33 = z;
//	}
//	inline void makeScaling(const Vector3& v) {
//		makeScaling(v.x, v.y, v.z);
//	}
//	//@}
//
//	/// ���������������ת����
//	void makeRotationAxis(const Vector3& axis, Real angle);
//
//	/// ������X�����ת����
//	void makeRotationX(Real angle);
//
//	/// ������Y�����ת����
//	void makeRotationY(Real angle);
//
//	/// ������Z�����ת����
//	void makeRotationZ(Real angle);
//
//	/** ����ֱ�߹��Ӱ�Ӿ���
//	
//		@param lightDir ����Ͷ�䷽��
//		@param plane ����Ӱ�ӵ�ƽ�档
//	*/
//	void makeShadowByDirectionalLight(const Vector3 &lightDir, const Plane& plane);
//	/** �������Դ��Ӱ�Ӿ���
//	
//		@param lightPos ��Դλ�á�
//		@param plane ����Ӱ�ӵ�ƽ�档
//	*/
//	void makeShadowByPointLight(const Vector3 &lightPos, const Plane& plane);
//
//	/// ȡ��Vector3(_11, _12, _13)��
//	Vector3& xVector() {
//		return *((Vector3*)(&_11));
//	}
//	/// ȡ��Vector3(_21, _22, _23)��
//	Vector3& yVector() {
//		return *((Vector3*)(&_21));
//	}
//	/// ȡ��Vector3(_31, _32, _33)��
//	Vector3& zVector() {
//		return *((Vector3*)(&_31));
//	}
//
//
//	/// ȡ��Vector3(_11, _12, _13)��
//	const Vector3& xVector() const {
//		return *((Vector3*)(&_11));
//	}
//	/// ȡ��Vector3(_21, _22, _23)��
//	const Vector3& yVector() const {
//		return *((Vector3*)(&_21));
//	}
//	/// ȡ��Vector3(_31, _32, _33)��
//	const Vector3& zVector() const {
//		return *((Vector3*)(&_31));
//	}
//
	/** ��������ϵͳ�任����
	
		@param xAxis x�ᣬ��λ����
		@param yAxis y�ᣬ��λ����
		@param zAxis z�ᣬ��λ����

		�����߱��뱣֤�������������һ������ϵ��������λ�ᣬ���������
		����������������ϵ��
	*/
	inline void makeRotationFromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
//
//	/** ��Quaternion��Matrix4��
//	
//		@param q 
//	*/
//	void fromQuaternion(const Quaternion &q);
//
//	/** ������ת����ʹһ�������ܱ任����һ��������
//	
//		@param from 
//		@param to 
//	*/
//	void makeRotationVectorToVector(const Vector3 &from, const Vector3 &to);
//
//	/** �������ϱ任����T����ʾ����ͨ��pt���axis����תangle�Ƕȡ�
//
//		@param pt ��ͨ��������һ�㡣
//		@param axis ��ת�ᡣ
//		@param angle ת�ǣ����ȡ�
//
//		����һ�����ϱ任��������һ����ת������û��ʵ������Ч���ģ����� Quaternion 
//		����û�����ַ����ġ�
//	*/
//	void makeRotationAt(const Vector3 &pt, const Vector3 &axis, Real angle);
//
//	/** �����������
//	
//		@param p ����ƽ�档
//	*/
//	void makeReflection(const Plane &p);// {
//		//D3DXMatrixReflect((D3DXMATRIX*)this, (CONST D3DXPLANE*)&p);
//	//}
};
//
////-----------------------------------------------------------------------------
//// Inline Functions
//
inline
Matrix4::Matrix4( const Real* pf )
{
	ooBreakAssert(pf);

    memcpy(&_11, pf, sizeof(Matrix4));
}

inline
	Matrix4::Matrix4( const Matrix4& mat )
{
	memcpy(&_11, &mat, sizeof(Matrix4));
}

inline
Matrix4::Matrix4( Real f11, Real f12, Real f13, Real f14,
                        Real f21, Real f22, Real f23, Real f24,
                        Real f31, Real f32, Real f33, Real f34,
                        Real f41, Real f42, Real f43, Real f44 )
{
    _11 = f11; _12 = f12; _13 = f13; _14 = f14;
    _21 = f21; _22 = f22; _23 = f23; _24 = f24;
    _31 = f31; _32 = f32; _33 = f33; _34 = f34;
    _41 = f41; _42 = f42; _43 = f43; _44 = f44;
}

inline
void Matrix4::set( Real f11, Real f12, Real f13, Real f14,
                        Real f21, Real f22, Real f23, Real f24,
                        Real f31, Real f32, Real f33, Real f34,
                        Real f41, Real f42, Real f43, Real f44 )
{
    _11 = f11; _12 = f12; _13 = f13; _14 = f14;
    _21 = f21; _22 = f22; _23 = f23; _24 = f24;
    _31 = f31; _32 = f32; _33 = f33; _34 = f34;
    _41 = f41; _42 = f42; _43 = f43; _44 = f44;
}
//
//
//// access grants
//inline Real&
//Matrix4::operator () ( Uint iRow, Uint iCol )
//{
//    return m[iRow][iCol];
//}
//
//inline Real
//Matrix4::operator () ( Uint iRow, Uint iCol ) const
//{
//    return m[iRow][iCol];
//}
//
//
//// casting operators
//inline
//Matrix4::operator Real* ()
//{
//    return (Real *) &_11;
//}
//
//inline
//Matrix4::operator const Real* () const
//{
//    return (const Real *) &_11;
//}
//
//
//// assignment operators
//inline Matrix4&
//Matrix4::operator *= ( const Matrix4& mat )
//{
//    Matrix4Multiply(this, this, &mat);
//    return *this;
//}
//
//inline Matrix4&
//Matrix4::operator += ( const Matrix4& mat )
//{
//    _11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
//    _21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
//    _31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
//    _41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
//    return *this;
//}
//
//inline Matrix4&
//Matrix4::operator -= ( const Matrix4& mat )
//{
//    _11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
//    _21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
//    _31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
//    _41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
//    return *this;
//}
//
//inline Matrix4&
//Matrix4::operator *= ( Real f )
//{
//    _11 *= f; _12 *= f; _13 *= f; _14 *= f;
//    _21 *= f; _22 *= f; _23 *= f; _24 *= f;
//    _31 *= f; _32 *= f; _33 *= f; _34 *= f;
//    _41 *= f; _42 *= f; _43 *= f; _44 *= f;
//    return *this;
//}
//
//inline Matrix4&
//Matrix4::operator /= ( Real f )
//{
//    Real fInv = 1.0f / f;
//    _11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
//    _21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
//    _31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
//    _41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
//    return *this;
//}
//
//
//// unary operators
//inline Matrix4
//Matrix4::operator + () const
//{
//    return *this;
//}
//
//inline Matrix4
//Matrix4::operator - () const
//{
//    return Matrix4(-_11, -_12, -_13, -_14,
//                      -_21, -_22, -_23, -_24,
//                      -_31, -_32, -_33, -_34,
//                      -_41, -_42, -_43, -_44);
//}
//
//
//// binary operators
//inline Matrix4
//Matrix4::operator * ( const Matrix4& mat ) const
//{
//    Matrix4 matT;
//    Matrix4Multiply(&matT, this, &mat);
//    return matT;
//}
//
//inline Matrix4
//Matrix4::operator + ( const Matrix4& mat ) const
//{
//    return Matrix4(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
//                      _21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
//                      _31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
//                      _41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
//}
//
//inline Matrix4
//Matrix4::operator - ( const Matrix4& mat ) const
//{
//    return Matrix4(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
//                      _21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
//                      _31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
//                      _41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
//}
//
//inline Matrix4
//Matrix4::operator * ( Real f ) const
//{
//    return Matrix4(_11 * f, _12 * f, _13 * f, _14 * f,
//                      _21 * f, _22 * f, _23 * f, _24 * f,
//                      _31 * f, _32 * f, _33 * f, _34 * f,
//                      _41 * f, _42 * f, _43 * f, _44 * f);
//}
//
//inline Matrix4
//Matrix4::operator / ( Real f ) const
//{
//    Real fInv = 1.0f / f;
//    return Matrix4(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
//                      _21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
//                      _31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
//                      _41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
//}
//
//
//inline Matrix4
//operator * ( Real f, const Matrix4& mat )
//{
//    return Matrix4(f * mat._11, f * mat._12, f * mat._13, f * mat._14,
//                      f * mat._21, f * mat._22, f * mat._23, f * mat._24,
//                      f * mat._31, f * mat._32, f * mat._33, f * mat._34,
//                      f * mat._41, f * mat._42, f * mat._43, f * mat._44);
//}
//
//
//inline bool
//Matrix4::operator == ( const Matrix4& mat ) const
//{
//    return 0 == memcmp(this, &mat, sizeof(Matrix4));
//}
//
//inline bool
//Matrix4::operator != ( const Matrix4& mat ) const
//{
//    return 0 != memcmp(this, &mat, sizeof(Matrix4));
//}
//
//inline Vector3 Matrix4::transform(const Vector3 &in) const
//{
//	Vector4 out;
//	out.x = _11*in.x + _12*in.y + _13*in.z + _14;
//	out.y = _21*in.x + _22*in.y + _23*in.z + _24;
//	out.z = _31*in.x + _32*in.y + _33*in.z + _34;
//	out.w = _41*in.x + _42*in.y + _43*in.z + _44;
//
//	return Vector3(out.x/out.w, out.y/out.w, out.z/out.w);
//}
//
//inline Vector3 Matrix4::transformNormal(const Vector3 &in) const
//{
//	Vector3 out;
//	out.x = _11*in.x + _12*in.y + _13*in.z;
//	out.y = _21*in.x + _22*in.y + _23*in.z;
//	out.z = _31*in.x + _32*in.y + _33*in.z;
//
//	return Vector3(out.x, out.y, out.z);
//}
//
//inline Vector4 Matrix4::transform(const Vector4 &in) const
//{
//	Vector4 out;
//	out.x = _11*in.x + _12*in.y + _13*in.z + _14*in.w;
//	out.y = _21*in.x + _22*in.y + _23*in.z + _24*in.w;
//	out.z = _31*in.x + _32*in.y + _33*in.z + _34*in.w;
//	out.w = _41*in.x + _42*in.y + _43*in.z + _44*in.w;
//
//	return out;
//}
//
//inline void Matrix4::transpose()
//{
//	Matrix4 out;
//	out._11 = _11;
//	out._12 = _21;
//	out._13 = _31;
//	out._14 = _41;
//
//	out._21 = _12;
//	out._22 = _22;
//	out._23 = _32;
//	out._24 = _42;
//
//	out._31 = _13;
//	out._32 = _23;
//	out._33 = _33;
//	out._34 = _43;
//
//	out._41 = _14;
//	out._42 = _24;
//	out._43 = _34;
//	out._44 = _44;
//
//	(*this) = out;
//}
//
//inline void Matrix4::makeTranslation(Real x, Real y, Real z)
//{
//	identity();
//
//	_14 = x;
//	_24 = y;
//	_34 = z;
//}
//
//inline void Matrix4::getTranslation(Real& x, Real& y, Real& z) const
//{
//	x = _14;
//	y = _24;
//	z = _34;
//}
//
inline void Matrix4::makeRotationFromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
	*this = Matrix4::ZERO;

	_11 = xAxis.x;
	_21 = xAxis.y;
	_31 = xAxis.z;

	_12 = yAxis.x;
	_22 = yAxis.y;
	_32 = yAxis.z;

	_13 = zAxis.x;
	_23 = zAxis.y;
	_33 = zAxis.z;

	_44 = 1;
}

OO_NAMESPACE_END

#endif // #ifndef OO_MATRIX4_H_
