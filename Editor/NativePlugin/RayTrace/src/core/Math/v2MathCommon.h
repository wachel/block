// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_MATH_COMMON_H_
#define OO_MATH_COMMON_H_

#include "../Common/v2ObjStd.h"
#include "v2Vector3.h"
#include "v2Vector4.h"
#include "v2Matrix4.h"
#include "../Math2/Vector4.h"

OO_NAMESPACE_BEGIN

/** 剪裁结果。
*/
enum CullState
{
	CS_Inside		= 1 << 3,	///< object bounding box is totally inside the frustum
	CS_Outside		= 1 << 4,   ///< object bounding box is outside the frustum
	CS_Intersect	= 1 << 1,   ///< 既不Inside又不Outside

	//CS_InsideSlow,  // OBB is inside frustum, but it took extensive testing to determine this
	//CS_OutsideSlow, // OBB is outside frustum, but it took extensive testing to determine this
};

////////////////////////////////////////////////////////////////////////////////
/** @page exp-templ-to-py 导出C++模版到Python
	
	由于Python没有C++那样的模版格式，C++模版导出到Python时作了名字转换，
	循序的规律如下：

	- 如 C++: Point<int>，导出到Python后为：PointInt。
	- 如 C++: Point<Real>，导出到Python后为：PointReal。

	由于历史原因有些模版参数为Real的模版，导出到Python时没有Real后缀，
	如：Rect<Real>导出到Python叫Rect，而不是RectReal。为了兼容这些
	Python类并没有取消，但以后请使用规范的类名，如：RectReal, PointReal。
*/

////////////////////////////////////////////////////////////////////////////////
// Point

/** 2D中的点。

	@par Python
		使用时请注意名字的改变，看这里：@ref exp-templ-to-py。
*/
template <class T>
class Point
{
public:
	/// 常量。
	static const Point<T> ZERO;

	//@{
	/** 坐标值。*/
	T x;
	T y; 
	//@}

	Point() {}
	Point(T ix, T iy) {
		set(ix, iy);
	}
	Point(const Point& rhs) {
		set(rhs.x, rhs.y);
	}
	Point& operator= (const Point& rhs) {
		set(rhs.x, rhs.y);
		return *this;
	}
	bool operator == ( const Point<T>& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator != ( const Point<T>& rhs) const {
		return !(*this == rhs);
	}

    // assignment operators
	Point<T>& operator += ( const Point<T>& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Point<T>& operator -= ( const Point<T>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	
    // unary operators
	Point<T> operator + () const {
		return *this;
	}
	Point<T> operator - () const {
		return Point<T>(-x, -y);
	}
	
    // binary operators
	Point<T> operator + ( const Point<T>& rhs) const {
		return Point<T>(x + rhs.x, y + rhs.y);
	}
	Point<T> operator - ( const Point<T>& rhs) const {
		return Point<T>(x - rhs.x, y - rhs.y);
	}

	//@{
	/** Same documentation for both members. Details */
	void set(T ix, T iy) {
		x = ix;
		y = iy;
	}

	void move(T offsetX, T offsetY) {
		x += offsetX;
		y += offsetY;
	}
	//@}
};

template<class T>
const Point<T> Point<T>::ZERO(0, 0);

/////////////////////////////////////////////////////////////////////////////////////////
// Size

/** 2D中的尺寸。

	@par Python
		使用时请注意名字的改变，看这里：@ref exp-templ-to-py。
*/
template <class T>
class Size
{
public:
	/// 常量。
	static const Size<T> ZERO;

	T cx, cy;

	Size() {}
	Size(T icx, T icy) : cx(icx), cy(icy) {}

	Size& operator= (const Size<T>& rhs) {
		set(rhs.cx, rhs.cy);
		return *this;
	}
	bool operator == ( const Size<T>& rhs) const {
		return cx == rhs.cx && cy == rhs.cy;
	}
	bool operator != ( const Size<T>& rhs) const {
		return !(*this == rhs);
	}

	void set(T icx, T icy) {
		cx = icx;
		cy = icy;
	}

	void zero() {
		cx = 0;
		cy = 0;
	}
	bool isZero() const {
		return cx == 0 && cy == 0;
	}

	void increase(T dx, T dy) {
		cx += dx;
		cy += dy;
	}

	void decrease(T dx, T dy) {
		cx -= dx;
		cy -= dy;
	}
};

template<class T>
const Size<T> Size<T>::ZERO(0, 0);

/////////////////////////////////////////////////////////////////////////////////////////
// Rect

/** 矩形类。

	Rect 包含以下公有变量：
	@code
	union {
		struct {
			T minX, minY, maxX, maxY;
		};
		struct {
			Point<T> ptMin, ptMax;
		};
	};
	@endcode

*/
template <class T>
class Rect
{
public:
	/// 常量。
	static const Rect<T> ZERO;

	union {
		struct {
			T minX, minY, maxX, maxY;
		};
		struct {
			Point<T> ptMin, ptMax;
		};
	};

	//@{ 
	/// 构造函数。
	Rect() {}
	Rect(T iminX, T iminY, T imaxX, T imaxY) : minX(iminX), minY(iminY), maxX(imaxX), maxY(imaxY) {}
	Rect(const Point<T> &iptMin, const Point<T> &iptMax ) : ptMin(iptMin), ptMax(iptMax) {}
	//@}

	//@{
	/// 比较。
	bool operator == ( const Rect<T>& rhs) const {
		return minX == rhs.minX && maxX == rhs.maxX && minY == rhs.minY && maxY == rhs.maxY;
	}
	bool operator != ( const Rect<T>& rhs) const {
		return !(*this == rhs);
	}
	//@}

	//@{ 
	/// 重新附值。
	void set(T iminX, T iminY, T imaxX, T imaxY) {
		minX = iminX;
		minY = iminY;
		maxX = imaxX;
		maxY = imaxY;
	}

	void set(const Point<T> &iptMin, const Point<T> &iptMax) {
		ptMin = iptMin;
		ptMax = iptMax;
	}
	//@}

	/** 求宽度。
		
		@return 宽度。
	*/
	T width() const {
		return maxX - minX;
	}

	/** 求高度。
		
		@return 高度。
	*/
	T height() const {
		return maxY - minY;
	}

	/** 单位化。
		
		单位化后能保证矩形坐标的大小关系是正确的，maxX > minX, maxY > minY.
	*/
	void normalize() {
		if (minX > maxX)
			std::swap(minX, maxX);
		if (minY > maxY)
			std::swap(minY, maxY);
	}

	/** 是否为单位化的。
		
		@return 是否为单位化的。
	*/
	bool isNormalized() const {
		return minX <= maxX && minY <= maxY;
	}

	/** 判断面积是否空。
		
		@return 面积是否空。
	*/
	bool isEmpty() const {
		return width() == 0 || height() == 0;
	}

	/** 把矩形设置为无限大。

		调用后矩形个坐标都为相应数据类型的最大值或最小值。
	*/
	void infinite() {
		minX = minY = std::numeric_limits<T>::min();
		maxX = maxY = std::numeric_limits<T>::max();
	}

	/** 设置自己为求两矩形的交集。

		@param r1 一个矩形。
		@param r2 一个矩形。

		@return 是否相交。

		如果两矩形不相交则自己会变为一个无效矩形，所有属性无效。
	*/
	inline bool intersect(const Rect<T> &r1, const Rect<T> &r2);

	/** 设置自己为求两矩形的并集。
		
		@param r1 一个矩形。
		@param r2 一个矩形。
	*/
	inline void unionRect(const Rect<T> &r1, const Rect<T> &r2);

	/** 移动矩形。
		
		@param offsetX X偏移值。
		@param offsetY Y偏移值。
	*/
	void move(T offsetX, T offsetY) {
		minX += offsetX;
		minY += offsetY;
		maxX += offsetX;
		maxY += offsetY;
	}
	/** 移动矩形。
		
		@param offset 偏移值。
	*/
	void move(const Point<T> &offset) {
		move(offset.x, offset.y);
	}

	/** 移动矩形到目的地
		
		@param x,y 目的地，也就是矩形(minX, minY)。
	*/
	void moveTo(T x, T y) {
		move(x - minX, y - minY);
	}
	/** 移动矩形到目的地
		
		@param pt 目的地，也就是矩形(minX, minY)。
	*/
	void moveTo(const Point<T> pt) {
		move(pt - ptMin);
	}

	/** 放大矩形。
		
		@param iminX 放大值。
		@param iminY 放大值。
		@param imaxX 放大值。
		@param imaxY 放大值。

		如：inflate(1, 1, 1, 1) 表示矩形四边都向外扩大1。
	*/
	void inflate(T iminX, T iminY, T imaxX, T imaxY) {
		minX -= iminX;
		minY -= iminY;
		maxX += imaxX;
		maxY += imaxY;
	}

	/** 放大矩形。
		
		@param iminX 缩小值。
		@param iminY 缩小值。
		@param imaxX 缩小值。
		@param imaxY 缩小值。

		如：deflate(1, 1, 1, 1) 表示矩形四边都向中心缩小1。
	*/
	void deflate(T iminX, T iminY, T imaxX, T imaxY) {
		minX += iminX;
		minY += iminY;
		maxX -= imaxX;
		maxY -= imaxY;
	}

	/** 判断点是否在矩形中。
		
		@param x 点位置。
		@param y 点位置。
		@return 是否在矩形中。

		判断公式为：x >= minX && x < maxX && y >= minY && y < maxY，注
		意判断范围是不包括右上的边的。
	*/
	bool isPointIn(T x, T y) const {
		// 04/5/17
		// old: return x >= minX && x <= maxX && y >= minY && y <= maxY;
		return x >= minX && x < maxX && y >= minY && y < maxY;
	}
	/** 判断点是否在矩形中。
		
		@param pt 点位置。
		@return 是否在矩形中。

		判断公式为：x >= minX && x < maxX && y >= minY && y < maxY，注
		意判断范围是不包括右上的边的。
	*/
	bool isPointIn(const Point<T> &pt) const {
		return isPointIn(pt.x, pt.y);
	}

	/** 判断矩形是否在矩形中。
		
		@param rc 一个矩形。
		@return 是否在矩形中。
	*/
	bool isRectIn(const Rect<T> rc) const {
		return rc.minX >= minX && rc.minY >= minY && rc.maxX <= maxX && rc.maxY <= maxY;
	}

	/** 求矩形中点。
		
		@return 中点。
	*/
	Point<T> center() const {
		return Point<T>(minX + width()/2, minY + height()/2);
	}

	/** 缩放。
		
		@param w 宽度比例。
		@param h 高度比例。
	*/
	template<class T2>
	void scale( T2 w, T2 h) {
		minX = minX*w;
		minY = minY*h;
		maxX = maxX*w;
		maxY = maxY*h;
	}
};

template<class T>
const Rect<T> Rect<T>::ZERO(0, 0, 0, 0);

template <class T>
bool Rect<T>::intersect(const Rect<T> &r1, const Rect<T> &r2)
{
	minX = std::max(r1.minX, r2.minX);
	minY = std::max(r1.minY, r2.minY);
	
	maxX = std::min(r1.maxX, r2.maxX);
	maxY = std::min(r1.maxY, r2.maxY);

	return isNormalized();
}

template <class T>
void Rect<T>::unionRect(const Rect<T> &r1, const Rect<T> &r2)
{
	minX = std::min(r1.minX, r2.minX);
	minY = std::min(r1.minY, r2.minY);
	maxX = std::max(r1.maxX, r2.maxX);
	maxY = std::max(r1.maxY, r2.maxY);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Plane

class OO_API Plane
{
public:
	Vector3 normal;
	Real dist;

public:
	Plane() {}
	Plane( const Vector3 &n, Real d) : normal(n), dist(d) {}

	// unary operators
	Plane operator + () const {
		return *this;
	}

	Plane operator - () const {
		return Plane(-normal, -dist);
	}

	// binary operators
	bool operator == ( const Plane& p) const {
		return normal == p.normal && dist == p.dist;
	}
	bool operator != ( const Plane& p) const {
		return normal != p.normal || dist != p.dist;
	}

	Plane operator + ( const Plane& p) const {
		return Plane(normal + p.normal, dist + p.dist);
	}
	Plane& operator += ( const Plane& p) {
		*this = *this+p;
		return *this;
	}

	Plane operator - ( const Plane& p) const {
		return Plane(normal - p.normal, dist - p.dist);
	}
	Plane& operator -= ( const Plane& p) {
		*this = *this-p;
		return *this;
	}

	Plane operator * ( Real rhs) const {
		return Plane(normal*rhs, dist*rhs);
	}
	Plane& operator *= ( Real rhs) {
		*this = *this * rhs;
		return *this;
	}

	Plane operator / ( Real rhs) const {
		return Plane(normal/rhs, dist/rhs);
	}
	Plane& operator /= ( Real rhs) {
		*this = *this / rhs;
		return *this;
	}

	/// ax + by + cz + dw
	Real dot( const Vector4 &v) const {
		return normal.x * v.x + normal.y * v.y + normal.z * v.z - dist * v.w;
	}

	/// ax + by + cz + d
	Real dotCoord( const Vector3 &v) const {
		return normal.x * v.x + normal.y * v.y + normal.z * v.z - dist;
	}

	/// ax + by + cz
	Real dotNormal( const Vector3 &v) const {
		return normal.x * v.x + normal.y * v.y + normal.z * v.z;
	}

	/// Normalize plane (so that |normal| == 1)
	void normalize()
	{
		Real d = Math::rInvSqrt(normal.lengthSq());
		*this = *this * d;
		//D3DXPlaneNormalize( (D3DXPLANE *)this,(D3DXPLANE *)this);
	}

//	/** Find the intersection between a plane and a line.  If the line is
//	parallel to the plane, NULL is returned.
//	*/
//	bool intersectLine(Vector3 &out, const Vector3 &v1, const Vector3 &v2)
//	{
//		return D3DXPlaneIntersectLine
//			( (D3DXVECTOR3*)&out, (CONST D3DXPLANE *)this, (CONST D3DXVECTOR3 *)&v1,
//			(CONST D3DXVECTOR3 *)&v2) != 0;
//	}

	/// Construct a plane from a point and a normal
	void fromPointNormal(const Vector3 &p, const Vector3 &n)
	{
		normal = n;
		dist = p.dot(normal);
	}

	/// Construct a plane from 3 points
	//  changed be eva for right hand system
	//void fromPoints(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);

//	/** Transform a plane by a matrix.  The vector (a,b,c) must be normal.
//	M should be the inverse transpose of the transformation desired.
//	*/
//	void transform(const Matrix4 &m)
//	{
//		D3DXPlaneTransform( (D3DXPLANE *)this, (CONST D3DXPLANE *)this, (CONST D3DXMATRIX *)&m );
//	}

	Vector3 getPointOnPlane() const {
		return normal*dist;
	}
};

OO_NAMESPACE_END

#endif
