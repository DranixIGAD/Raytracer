#include "Plane.h"

Plane::Plane()
{
	basePos = new Vector3(0, 0, 0);
	normal = new Vector3(0, 1, 0);
}

Plane::Plane(float x, float y, float z)
{
	basePos = new Vector3(x, y, z);
	normal = new Vector3(0, 1, 0);
}

Plane::~Plane()
{
	delete normal;
	delete basePos;
}

bool Plane::Hit(const Ray& ray)
{
	t = ((*basePos - *ray.Origin) * *normal) / (*ray.Direction * *normal);


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

bool Plane::HitShadow(const Ray&, const float&)
{
	return false;
}

Vector3 Plane::getNormal()
{
	return *normal;
}

void Plane::setNormal(float x, float y, float z)
{
	normal = new Vector3(x, y, z);
	normal->Normalize();
}
