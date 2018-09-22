// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
//#include "v2Standard.h"
#include "v2Math.h"
#include "random.h"
// from "fastmath.cpp"
extern void  build_sqrt_table();

OO_NAMESPACE_BEGIN

OO_API Real Math::PI = 4.0 * atan( 1.0 );
OO_API Real Math::TWO_PI = 2.f*Math::PI;
OO_API Real Math::EPSILON = (Real)1e-10; //(Real)1e-03;
OO_API Real Math::REAL_MAX = 10000000;//(Real)(3.402823466e+18f);
OO_API Real Math::REAL_MIN = -Math::REAL_MAX;
OO_API Real Math::LOWEPSILON = (Real)1e-04;
void Math::initialize()
{
	build_sqrt_table();
	mysrandom(1234567);
}

void Math::destroy()
{
}

int Math::rand(int min, int max)
{
	return min + (::irand() % (max - min + 1));
}

OO_NAMESPACE_END