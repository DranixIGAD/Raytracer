#pragma once
#include "../myMath/Vector3.h"

class Ray
{
public:
	Ray();
	Ray(const Vector3& direction, const Vector3& beginpos);
	~Ray();
	
	Vector3* Origin;
	Vector3* Direction;
};

