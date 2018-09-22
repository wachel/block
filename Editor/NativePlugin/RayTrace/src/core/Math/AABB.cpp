//#include "v2Math.h"
#include "v2Vector3.h"
#include "AABB.h"

OO_NAMESPACE_BEGIN

void AABB::reset() 
{
	vMin.x = vMin.y = vMin.z = Math::REAL_MAX;
	vMax.x = vMax.y = vMax.z = Math::REAL_MIN;
};

void AABB::addPoint(const Vector3& v)
{	
	vMax.x = std::max(v.x,vMax.x);
	vMax.y = std::max(v.y,vMax.y);
	vMax.z = std::max(v.z,vMax.z);

	vMin.x = std::min(v.x,vMin.x);
	vMin.y = std::min(v.y,vMin.y);
	vMin.z = std::min(v.z,vMin.z);
}

void AABB::unionBox(const AABB& box)
{
	ooAssert(box.isValid());

	addPoint(box.vMax);
	addPoint(box.vMin);
}

bool AABB::isIntersected(const AABB& box) const
{
	ooAssert(box.isValid());

	if (box.vMax.x < vMin.x)
		return false;
	else if(box.vMax.y < vMin.y)
		return false;
	else if(box.vMax.z < vMin.z)
		return false;

	else if(box.vMin.x > vMax.x)
		return false;
	else if(box.vMin.y > vMax.y)
		return false;
	else if(box.vMin.z > vMax.z)
		return false;

	return true;
}

//void AABB::fromBSphere(const BSphere &bs)
//{
//	vMin.x = bs.center.x - bs.radius;
//	vMin.y = bs.center.y - bs.radius;
//	vMin.z = bs.center.z - bs.radius;
//
//	vMax.x = bs.center.x + bs.radius;
//	vMax.y = bs.center.y + bs.radius;
//	vMax.z = bs.center.z + bs.radius;
//}

OO_NAMESPACE_END