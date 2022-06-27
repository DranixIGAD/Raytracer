#pragma once
#include "Surface.h"

class Plane : public Surface
{
public:

	Plane();
	Plane(float x, float y, float z);
	~Plane();

	bool Hit(const Ray &ray);
	bool HitShadow(const Ray& ray, const float& distanceToLight);
	Vector3 getNormal();
	void setNormal(float x, float y, float z);

	Vector3* basePos;
	Vector3* normal;

};

