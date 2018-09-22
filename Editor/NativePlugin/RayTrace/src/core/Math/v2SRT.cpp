// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
//#include "v2Standard.h"
#include "v2SRT.h"
/*
R1 = R0*R10
T1 = T0 + R0*S0*T10
S1 = S0*S10

R10 = _R0*R1
T10 = _R0*(T1-T0)/S0
S10 = S0/S1
*/
OO_NAMESPACE_BEGIN

// predefined specific vectors
// fix bug: 静态变量初始化顺序是不能控制的，SRT::IDENTITY在debug版比Vector3::ONE还更早初始化！
// old : const SRT SRT::IDENTITY(Vector3::ONE, Quaternion::IDENTITY, Vector3::ZERO);
const SRT SRT::IDENTITY(Vector3(1.f, 1.f, 1.f), Quaternion(0.f, 0.f, 0.f, 1.f), Vector3(0.f, 0.f, 0.f));

void SRT::absToRel(const SRT& base, const SRT& abs)
{
	const Vector3 &T0 = base.t;
	const Vector3 &S0= base.s;
	const Quaternion &R0 = base.r;

	const Vector3 &T1 = abs.t;
	const Vector3 &S1= abs.s;
	const Quaternion &R1 = abs.r;

	Vector3 T10, S10;
	Quaternion R10, _R0;

	_R0 = R0.getConjugate();

	S10 = S1/S0;
	R10 = _R0*R1;

	T10 = _R0.transform(T1 - T0)/S0;

	*this = SRT(S10, R10, T10);
}

void SRT::relToAbs(const SRT& base, const SRT& rel)
{
	const Vector3 &T0 = base.t;
	const Vector3 &S0= base.s;
	const Quaternion &R0 = base.r;

	const Vector3 &T10 = rel.t;
	const Vector3 &S10= rel.s;
	const Quaternion &R10 = rel.r;

	Vector3 T1, S1;
	Quaternion R1;

	// R1.
	R1 = R0*R10;

	// S1
	S1 = S10*S0;

	// T1
	T1 = T0 + R0.transform(S0*T10);

	*this = SRT(S1, R1, T1);
}

//Matrix4 SRT::toMatrix() const
//{
//	Matrix4 mat;
//	Matrix4 matWorld = Matrix4::IDENTITY;
//
//	// scaling.
//	mat.makeScaling(s);
//	matWorld = mat*matWorld;
//
//	// rotation
//	mat.fromQuaternion(r);
//	matWorld = mat*matWorld;
//
//	// translation.
//	mat.makeTranslation(t);
//	matWorld = mat*matWorld;
//
//	return matWorld;
//}

void SRT::lerpSRT(const SRT& so, const SRT& de, Real ft)
{
	ooAssert(ft >= 0.f && ft <= 1.0f);
	// lerp scale
	s = so.s + (de.s - so.s)*ft;
	// slerp rotation
	r.slerp(so.r,de.r,ft);
	// lerp translation
	t = so.t + (de.t - so.t)*ft;
}

//////////////////////////////////////////////////////////////////////////
OO_NAMESPACE_END