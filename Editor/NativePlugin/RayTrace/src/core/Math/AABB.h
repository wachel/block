#pragma once


#include "v2Vector3.h"
#include "v2MathCommon.h"
OO_NAMESPACE_BEGIN

class OO_API AABB
{
public:
	Vector3 vMin, vMax;

public:
	AABB() {}
	AABB(const Vector3 &ivMin, const Vector3 &ivMax) : vMin(ivMin), vMax(ivMax) {}

	void reset();
	bool isValid() const {
		return getDx()>=0.f && getDy()>=0.f && getDz()>=0.f;
	}

	void addPoint(const Vector3& v);
	void unionBox(const AABB& box);

	bool isIntersected(const AABB& box) const;
	bool isPointInside(const Vector3& v) const {
		return v.x > vMin.x && v.x < vMax.x
			&& v.y > vMin.y && v.y < vMax.y
			&& v.z > vMin.z && v.z < vMax.z;
	}

	void move(const Vector3 offset) {
		vMin += offset;
		vMax += offset;
	}

	Real getDx() const {
		return vMax.x - vMin.x;
	}
	Real getDy() const {
		return vMax.y - vMin.y;
	}
	Real getDz() const {
		return vMax.z - vMin.z;
	}
	Vector3 getSize() const {
		return Vector3(getDx(), getDy(), getDz());
	}

	Vector3 getCenter() const {
		return (vMin + vMax)/2.f;
	}

	Real getMaxEdgeLength() const {
		return std::max(std::max(getDx(), getDy()), getDz());
	}

	Real getDigonalLengthSq() const {
		Real dx = getDx();
		Real dy = getDy();
		Real dz = getDz();
		return dx*dx + dy*dy + dz*dz;
	}
	Real getDigonalLength() const {
		return Math::rSqrt(getDigonalLengthSq());
	}

	// generate axis-aligned bounding-box from a bounding-sphere.
	//void fromBSphere(const BSphere &bs);

	void inflate(Real x, Real y, Real z) {
		vMin -= Vector3(x, y, z);
		vMax += Vector3(x, y, z);
	}

	void deflate(Real x, Real y, Real z) {
		vMin += Vector3(x, y, z);
		vMax -= Vector3(x, y, z);
	}

	CullState cullAABB(const AABB &box) const {
		if (!isIntersected(box))
			return CS_Outside;
		else if (isPointInside(box.vMin) && isPointInside(box.vMax))
			return CS_Inside;
		else
			return CS_Intersect;
	}

	// @params pts should be an 8-elems array.
	void getAllPoints(Vector3 *pts) const {
		ooAssert(pts);

		// z-min rect.
		pts[0] = Vector3(vMin.x, vMin.y, vMin.z);
		pts[1] = Vector3(vMax.x, vMin.y, vMin.z);
		pts[2] = Vector3(vMax.x, vMax.y, vMin.z);
		pts[3] = Vector3(vMin.x, vMax.y, vMin.z);

		// z-max rect.
		pts[4] = Vector3(vMin.x, vMin.y, vMax.z);
		pts[5] = Vector3(vMax.x, vMin.y, vMax.z);
		pts[6] = Vector3(vMax.x, vMax.y, vMax.z);
		pts[7] = Vector3(vMin.x, vMax.y, vMax.z);
	}

	// transform all points, return a new box.
	//AABB transform(const Matrix4 &mat) const {
	//	Vector3 pts[8];
	//	getAllPoints(pts);

	//	AABB box;
	//	box.reset();
	//	for (int i = 0; i < 8; ++i) {
	//		pts[i] = mat.transform(pts[i]);
	//		box.addPoint(pts[i]);
	//	}

	//	return box;
	//}
};

OO_NAMESPACE_END