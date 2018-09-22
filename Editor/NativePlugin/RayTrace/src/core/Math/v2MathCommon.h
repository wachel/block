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

/** ���ý����
*/
enum CullState
{
	CS_Inside		= 1 << 3,	///< object bounding box is totally inside the frustum
	CS_Outside		= 1 << 4,   ///< object bounding box is outside the frustum
	CS_Intersect	= 1 << 1,   ///< �Ȳ�Inside�ֲ�Outside

	//CS_InsideSlow,  // OBB is inside frustum, but it took extensive testing to determine this
	//CS_OutsideSlow, // OBB is outside frustum, but it took extensive testing to determine this
};

////////////////////////////////////////////////////////////////////////////////
/** @page exp-templ-to-py ����C++ģ�浽Python
	
	����Pythonû��C++������ģ���ʽ��C++ģ�浼����Pythonʱ��������ת����
	ѭ��Ĺ������£�

	- �� C++: Point<int>��������Python��Ϊ��PointInt��
	- �� C++: Point<Real>��������Python��Ϊ��PointReal��

	������ʷԭ����Щģ�����ΪReal��ģ�棬������Pythonʱû��Real��׺��
	�磺Rect<Real>������Python��Rect��������RectReal��Ϊ�˼�����Щ
	Python�ಢû��ȡ�������Ժ���ʹ�ù淶���������磺RectReal, PointReal��
*/

////////////////////////////////////////////////////////////////////////////////
// Point

/** 2D�еĵ㡣

	@par Python
		ʹ��ʱ��ע�����ֵĸı䣬�����@ref exp-templ-to-py��
*/
template <class T>
class Point
{
public:
	/// ������
	static const Point<T> ZERO;

	//@{
	/** ����ֵ��*/
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

/** 2D�еĳߴ硣

	@par Python
		ʹ��ʱ��ע�����ֵĸı䣬�����@ref exp-templ-to-py��
*/
template <class T>
class Size
{
public:
	/// ������
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

/** �����ࡣ

	Rect �������¹��б�����
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
	/// ������
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
	/// ���캯����
	Rect() {}
	Rect(T iminX, T iminY, T imaxX, T imaxY) : minX(iminX), minY(iminY), maxX(imaxX), maxY(imaxY) {}
	Rect(const Point<T> &iptMin, const Point<T> &iptMax ) : ptMin(iptMin), ptMax(iptMax) {}
	//@}

	//@{
	/// �Ƚϡ�
	bool operator == ( const Rect<T>& rhs) const {
		return minX == rhs.minX && maxX == rhs.maxX && minY == rhs.minY && maxY == rhs.maxY;
	}
	bool operator != ( const Rect<T>& rhs) const {
		return !(*this == rhs);
	}
	//@}

	//@{ 
	/// ���¸�ֵ��
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

	/** ���ȡ�
		
		@return ��ȡ�
	*/
	T width() const {
		return maxX - minX;
	}

	/** ��߶ȡ�
		
		@return �߶ȡ�
	*/
	T height() const {
		return maxY - minY;
	}

	/** ��λ����
		
		��λ�����ܱ�֤��������Ĵ�С��ϵ����ȷ�ģ�maxX > minX, maxY > minY.
	*/
	void normalize() {
		if (minX > maxX)
			std::swap(minX, maxX);
		if (minY > maxY)
			std::swap(minY, maxY);
	}

	/** �Ƿ�Ϊ��λ���ġ�
		
		@return �Ƿ�Ϊ��λ���ġ�
	*/
	bool isNormalized() const {
		return minX <= maxX && minY <= maxY;
	}

	/** �ж�����Ƿ�ա�
		
		@return ����Ƿ�ա�
	*/
	bool isEmpty() const {
		return width() == 0 || height() == 0;
	}

	/** �Ѿ�������Ϊ���޴�

		���ú���θ����궼Ϊ��Ӧ�������͵����ֵ����Сֵ��
	*/
	void infinite() {
		minX = minY = std::numeric_limits<T>::min();
		maxX = maxY = std::numeric_limits<T>::max();
	}

	/** �����Լ�Ϊ�������εĽ�����

		@param r1 һ�����Ρ�
		@param r2 һ�����Ρ�

		@return �Ƿ��ཻ��

		��������β��ཻ���Լ����Ϊһ����Ч���Σ�����������Ч��
	*/
	inline bool intersect(const Rect<T> &r1, const Rect<T> &r2);

	/** �����Լ�Ϊ�������εĲ�����
		
		@param r1 һ�����Ρ�
		@param r2 һ�����Ρ�
	*/
	inline void unionRect(const Rect<T> &r1, const Rect<T> &r2);

	/** �ƶ����Ρ�
		
		@param offsetX Xƫ��ֵ��
		@param offsetY Yƫ��ֵ��
	*/
	void move(T offsetX, T offsetY) {
		minX += offsetX;
		minY += offsetY;
		maxX += offsetX;
		maxY += offsetY;
	}
	/** �ƶ����Ρ�
		
		@param offset ƫ��ֵ��
	*/
	void move(const Point<T> &offset) {
		move(offset.x, offset.y);
	}

	/** �ƶ����ε�Ŀ�ĵ�
		
		@param x,y Ŀ�ĵأ�Ҳ���Ǿ���(minX, minY)��
	*/
	void moveTo(T x, T y) {
		move(x - minX, y - minY);
	}
	/** �ƶ����ε�Ŀ�ĵ�
		
		@param pt Ŀ�ĵأ�Ҳ���Ǿ���(minX, minY)��
	*/
	void moveTo(const Point<T> pt) {
		move(pt - ptMin);
	}

	/** �Ŵ���Ρ�
		
		@param iminX �Ŵ�ֵ��
		@param iminY �Ŵ�ֵ��
		@param imaxX �Ŵ�ֵ��
		@param imaxY �Ŵ�ֵ��

		�磺inflate(1, 1, 1, 1) ��ʾ�����ı߶���������1��
	*/
	void inflate(T iminX, T iminY, T imaxX, T imaxY) {
		minX -= iminX;
		minY -= iminY;
		maxX += imaxX;
		maxY += imaxY;
	}

	/** �Ŵ���Ρ�
		
		@param iminX ��Сֵ��
		@param iminY ��Сֵ��
		@param imaxX ��Сֵ��
		@param imaxY ��Сֵ��

		�磺deflate(1, 1, 1, 1) ��ʾ�����ı߶���������С1��
	*/
	void deflate(T iminX, T iminY, T imaxX, T imaxY) {
		minX += iminX;
		minY += iminY;
		maxX -= imaxX;
		maxY -= imaxY;
	}

	/** �жϵ��Ƿ��ھ����С�
		
		@param x ��λ�á�
		@param y ��λ�á�
		@return �Ƿ��ھ����С�

		�жϹ�ʽΪ��x >= minX && x < maxX && y >= minY && y < maxY��ע
		���жϷ�Χ�ǲ��������ϵıߵġ�
	*/
	bool isPointIn(T x, T y) const {
		// 04/5/17
		// old: return x >= minX && x <= maxX && y >= minY && y <= maxY;
		return x >= minX && x < maxX && y >= minY && y < maxY;
	}
	/** �жϵ��Ƿ��ھ����С�
		
		@param pt ��λ�á�
		@return �Ƿ��ھ����С�

		�жϹ�ʽΪ��x >= minX && x < maxX && y >= minY && y < maxY��ע
		���жϷ�Χ�ǲ��������ϵıߵġ�
	*/
	bool isPointIn(const Point<T> &pt) const {
		return isPointIn(pt.x, pt.y);
	}

	/** �жϾ����Ƿ��ھ����С�
		
		@param rc һ�����Ρ�
		@return �Ƿ��ھ����С�
	*/
	bool isRectIn(const Rect<T> rc) const {
		return rc.minX >= minX && rc.minY >= minY && rc.maxX <= maxX && rc.maxY <= maxY;
	}

	/** ������е㡣
		
		@return �е㡣
	*/
	Point<T> center() const {
		return Point<T>(minX + width()/2, minY + height()/2);
	}

	/** ���š�
		
		@param w ��ȱ�����
		@param h �߶ȱ�����
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
