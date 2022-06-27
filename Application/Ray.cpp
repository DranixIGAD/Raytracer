#include "Ray.h"
#include <iostream>


Ray::Ray()
{
	printf("[WARNING!] using default constructor for a ray, this might not be what you want it to be...\n");
	Origin = new Vector3(0);
	Direction = new Vector3(0);
}

Ray::Ray(const Vector3& direction, const Vector3& beginpos)
	: Origin(new Vector3(beginpos)),
	Direction(new Vector3(direction))
{
}

Ray::~Ray()
{
	delete Origin;
	delete Direction;
}