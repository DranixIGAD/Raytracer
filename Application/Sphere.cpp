#include "Sphere.h"
#include <cmath>
#include <iostream>

Sphere::Sphere()
{
	center = new Vector3(0);
	radius = 1;
	minPointOG = minPointBB = *center - Vector3(radius, radius, -radius);
	maxPointOG = maxPointBB = *center + Vector3(radius, radius, -radius);
	printf("[WARNING!] Using the default constructor for a sphere, this might not be what you want it to be...\n");
}

Sphere::Sphere(float x, float y, float z, float Radius)
{
	center = new Vector3(x, y ,z);
	radius = Radius;
	minPointBB = *center - Vector3(radius, radius, -radius);
	maxPointBB = *center + Vector3(radius, radius, -radius);
}

Sphere::~Sphere()
{
	delete center;
}

bool Sphere::Hit(const Ray& ray)
{
	//float B = 2 * (viewRayDirection) * (cameraPos - *center);
	//float A = ((viewRayDirection) * (viewRayDirection));

	float C = ((*ray.Origin - *center) * (*ray.Origin - *center) - (radius * radius));
	float E = (*ray.Direction) * (*ray.Origin - *center);
	
	//float D = (B * B) - 4 * A * C;
	
	t = (-1 * E + sqrt( E * E - ((*ray.Direction) * (*ray.Direction)) * C)) / ((*ray.Direction) * (*ray.Direction));
	
	if (t > (-1 * E - sqrt(E * E - ((*ray.Direction) * (*ray.Direction)) * C)) / ((*ray.Direction) * (*ray.Direction)))
	{
		t = (-1 * E - sqrt(E * E - ((*ray.Direction) * (*ray.Direction)) * C)) / ((*ray.Direction) * (*ray.Direction));
	}


	
	if (t >= 0)
	{
		hitPos = *ray.Origin + (t * *ray.Direction);
		return true;
	}
	else
	{
		return false;
	}
}

bool Sphere::HitShadow(const Ray& ray, const float& distanceToLight)
{
	float C = ((*ray.Origin - *center) * (*ray.Origin - *center) - (radius * radius));
	float E = (*ray.Direction) * (*ray.Origin - *center);

	//float D = (B * B) - 4 * A * C;

	float b;
	const float bPlus = (-1 * E + sqrt(E * E - ((*ray.Direction) * (*ray.Direction)) * C)) / ((*ray.Direction) * (*ray.Direction));
	const float bMinus = (-1 * E - sqrt(E * E - ((*ray.Direction) * (*ray.Direction)) * C)) / ((*ray.Direction) * (*ray.Direction));

	if (bPlus > bMinus)
	{
		b = bMinus;
	}
	else
	{
		b = bPlus;
	}
	
	const Vector3 ShadowIntersectionPoint = (*ray.Origin + (b * *ray.Direction));
	const Vector3 toIntersectionVectorLength = ShadowIntersectionPoint - *ray.Origin;
	const float toIntersection = toIntersectionVectorLength.Magnitude();
		
	if (b >= 0 && toIntersection <= distanceToLight)
	{
		t = b;
		return true;
	}
	else
	{
		return false;
	}
}

Vector3 Sphere::getNormal()
{
	return (hitPos - *center) / radius;
}

Vector3 Sphere::get2ndRefractPoint(const Ray& ray)
{
	Vector3 origin(hitPos + getNormal() * -1e-4f);

	float C = ((origin - *center) * (origin - *center) - (radius * radius));
	float E = (*ray.Direction) * (origin - *center);

	float t1 = (-1 * E + sqrt(E * E - ((*ray.Direction) * (*ray.Direction)) * C)) / ((*ray.Direction) * (*ray.Direction));

	float t2 = (-1 * E - sqrt(E * E - ((*ray.Direction) * (*ray.Direction)) * C)) / ((*ray.Direction) * (*ray.Direction));


	if (t1 > 0)
	{
		return Vector3(*ray.Origin + (t1 * *ray.Direction));
	}
	else
	{
		return Vector3(*ray.Origin + (t2 * *ray.Direction));
	}
}
