// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
//#include "v2Standard.h"
#include "v2Vector3.h"

OO_NAMESPACE_BEGIN

// predefined specific vectors
const Vector3 Vector3::ZERO(0, 0, 0);
const Vector3 Vector3::ONE(1, 1, 1);
const Vector3 Vector3::UNIT_X(1, 0, 0);
const Vector3 Vector3::UNIT_Y(0, 1, 0);
const Vector3 Vector3::UNIT_Z(0, 0, 1);

std::ostream& operator<< (std::ostream& stream, const Vector3& v)
{
	stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return stream;
}



OO_NAMESPACE_END