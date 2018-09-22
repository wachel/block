// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_SRT_H_
#define OO_SRT_H_

#include "v2Math.h"
#include "v2Vector3.h"
#include "v2Quaternion.h"
#include "v2Matrix4.h"

OO_NAMESPACE_BEGIN

/** SRT 表示一组 Scaling-Rotationi-Translation 变换。

SRT 的变换顺序同名字一样，是先 Scaling 再 Rotation 再 Translation，SRT 主要用于求相
对方位和绝对方位的变换。
*/
class OO_API SRT
{
public:
	Vector3 s;    ///< 缩放
	Quaternion r; ///< 旋转
	Vector3 t;    ///< 平移

	//@{ 
	/// 常量。
	static const SRT IDENTITY; ///< SRT(Vector3::ONE, Quaternion::IDENTITY, Vector3::ZERO)，预定义常量。
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

	/** 绝对到相对变换。

	@param base 基础 SRT，也就是父空间在世界空间的变换。
	@param abs 绝对 SRT，也就是子空间在世界空间的变换。

	使用完后这个 SRT 就表示子空间到父空间的相对变化(Relative)，也即 abs=rel*base
	*/
	void absToRel(const SRT& base, const SRT& abs);
	/** 相对到绝对变换。

	@param base 基础 SRT，也就是父空间在世界空间的变换。
	@param rel 相对 SRT，也就是子空间在父空间的变换。

	使用完后这个 SRT 就表示子空间在世界空间的变化(Absolete)，也即 abs=rel*base
	*/
	void relToAbs(const SRT& base, const SRT& rel);

	/** 转换为一个变换矩阵。

	@return 等同的变换矩阵。
	*/
	Matrix4 toMatrix() const;

	/** 创建SRT，表示了绕通过pt点的axis轴旋转angle角度。

	@param pt 轴通过的任意一点。
	@param axis 旋转轴。
	@param angle 转角，弧度。
	*/
	void makeRotationAt(const Vector3 &pt, const Vector3 &axis, Real angle)
	{
		s = Vector3::ONE;
		r.fromAxisAngle(axis, angle);
		t = pt - r.transform(pt);
	}

	/** 单位化。		
	*/
	void identity() { *this = SRT::IDENTITY; }
	/** 线性插值。
	
	@param so 第一个SRT
	@param de 第二个SRT
	@param t  插值因子（0～1）

	使用完这个后SRT就表示在给定的两个SRT之间按照插值因子t得到的新值
	*/
	void lerpSRT(const SRT& so, const SRT& de, Real t);

};

//////////////////////////////////////////////////////////////////////////
OO_NAMESPACE_END

#endif
