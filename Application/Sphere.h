#pragma once
#include "Surface.h"

class Sphere : public Surface
{
public:

	Sphere();
	Sphere(float x, float y, float z, float Radius);
	~Sphere();

	bool Hit(const Ray& ray);
	bool HitShadow(const Ray& ray, const float& distanceToLight);
	Vector3 getNormal();
	Vector3 get2ndRefractPoint(const Ray& ray);
	Vector3* center;

	float radius;
};

