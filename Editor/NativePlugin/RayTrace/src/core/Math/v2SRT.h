// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_SRT_H_
#define OO_SRT_H_

#include "v2Math.h"
#include "v2Vector3.h"
#include "v2Quaternion.h"
#include "v2Matrix4.h"

OO_NAMESPACE_BEGIN

/** SRT ��ʾһ�� Scaling-Rotationi-Translation �任��

SRT �ı任˳��ͬ����һ�������� Scaling �� Rotation �� Translation��SRT ��Ҫ��������
�Է�λ�;��Է�λ�ı任��
*/
class OO_API SRT
{
public:
	Vector3 s;    ///< ����
	Quaternion r; ///< ��ת
	Vector3 t;    ///< ƽ��

	//@{ 
	/// ������
	static const SRT IDENTITY; ///< SRT(Vector3::ONE, Quaternion::IDENTITY, Vector3::ZERO)��Ԥ���峣����
	//@}

	/** */
	SRT() {}

	/** */
	SRT(const Vector3& S_, const Quaternion& R_, const Vector3& T_) : s(S_), r(R_), t(T_) {}

	/** */
	SRT(const SRT& srt) : s(srt.s), r(srt.r), t(srt.t) {}

	SRT operator*(const SRT& rhs) const {
		SRT srt;
		srt.relToAbs(rhs, *this);
		return srt;
	}

	/** ���Ե���Ա任��

	@param base ���� SRT��Ҳ���Ǹ��ռ�������ռ�ı任��
	@param abs ���� SRT��Ҳ�����ӿռ�������ռ�ı任��

	ʹ�������� SRT �ͱ�ʾ�ӿռ䵽���ռ����Ա仯(Relative)��Ҳ�� abs=rel*base
	*/
	void absToRel(const SRT& base, const SRT& abs);
	/** ��Ե����Ա任��

	@param base ���� SRT��Ҳ���Ǹ��ռ�������ռ�ı任��
	@param rel ��� SRT��Ҳ�����ӿռ��ڸ��ռ�ı任��

	ʹ�������� SRT �ͱ�ʾ�ӿռ�������ռ�ı仯(Absolete)��Ҳ�� abs=rel*base
	*/
	void relToAbs(const SRT& base, const SRT& rel);

	/** ת��Ϊһ���任����

	@return ��ͬ�ı任����
	*/
	Matrix4 toMatrix() const;

	/** ����SRT����ʾ����ͨ��pt���axis����תangle�Ƕȡ�

	@param pt ��ͨ��������һ�㡣
	@param axis ��ת�ᡣ
	@param angle ת�ǣ����ȡ�
	*/
	void makeRotationAt(const Vector3 &pt, const Vector3 &axis, Real angle)
	{
		s = Vector3::ONE;
		r.fromAxisAngle(axis, angle);
		t = pt - r.transform(pt);
	}

	/** ��λ����		
	*/
	void identity() { *this = SRT::IDENTITY; }
	/** ���Բ�ֵ��
	
	@param so ��һ��SRT
	@param de �ڶ���SRT
	@param t  ��ֵ���ӣ�0��1��

	ʹ���������SRT�ͱ�ʾ�ڸ���������SRT֮�䰴�ղ�ֵ����t�õ�����ֵ
	*/
	void lerpSRT(const SRT& so, const SRT& de, Real t);

};

//////////////////////////////////////////////////////////////////////////
OO_NAMESPACE_END

#endif
