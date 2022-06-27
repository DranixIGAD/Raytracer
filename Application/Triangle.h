#pragma once
#include "Surface.h"

class Triangle : public Surface
{
public:

	Triangle();
	Triangle(Vector3 pa, Vector3 pb, Vector3 pc);
	~Triangle();

	bool Hit(const Ray& ray);
	bool HitShadow(const Ray& ray, const float& distanceToLight);
	Vector3 getNormal();

	Vector3* pA;
	Vector3* pB;
	Vector3* pC;
};

