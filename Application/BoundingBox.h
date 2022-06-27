#pragma once
#include "../myMath/Vector3.h"
#include "../myMath/Matrix44.h"
#include "Ray.h"
#include <algorithm>

template <class objectType>
class BoundingBox
{
public:

	BoundingBox();
	BoundingBox(Vector3 minpoint, Vector3 maxpoint);
	BoundingBox(objectType* objects[], int array_size);
	BoundingBox(myContainer<objectType*> objects);
	~BoundingBox();
	
	bool Hit(const Ray& ray);


	Vector3 minPoint;
	Vector3 maxPoint;
};

template <class objectType>
BoundingBox<objectType>::BoundingBox()
{
}

template <class objectType>
BoundingBox<objectType>::BoundingBox(Vector3 minpoint, Vector3 maxpoint) : minPoint(Vector3(minpoint)), maxPoint(Vector3(maxpoint))
{
}

template <class objectType>
BoundingBox<objectType>::BoundingBox(objectType* objects[], int array_size)
{
	minPoint = Vector3(INFINITY, INFINITY, -INFINITY);
	maxPoint = Vector3(-INFINITY, -INFINITY, INFINITY);

	for (int i = 0; i < array_size; i++)
	{
		if (minPoint.x > objects[i]->minPointBB.x) { minPoint.x = objects[i]->minPointBB.x;	}
		if (minPoint.y > objects[i]->minPointBB.y) { minPoint.y = objects[i]->minPointBB.y; }
		if (minPoint.z < objects[i]->minPointBB.z) { minPoint.z = objects[i]->minPointBB.z; }

		if (maxPoint.x < objects[i]->maxPointBB.x) { maxPoint.x = objects[i]->maxPointBB.x; }
		if (maxPoint.y < objects[i]->maxPointBB.y) { maxPoint.y = objects[i]->maxPointBB.y; }
		if (maxPoint.z > objects[i]->maxPointBB.z) { maxPoint.z = objects[i]->maxPointBB.z; }
	}
}

template <class objectType>
BoundingBox<objectType>::BoundingBox(myContainer<objectType*> objects)
{
	minPoint = Vector3(INFINITY, INFINITY, -INFINITY);
	maxPoint = Vector3(-INFINITY, -INFINITY, INFINITY);

	for (int i = 0; i < objects.getSize(); i++)
	{
		if (minPoint.x > objects[i]->minPointBB.x)
		{
			minPoint.x = objects[i]->minPointBB.x;
		}
		if (minPoint.y > objects[i]->minPointBB.y) { minPoint.y = objects[i]->minPointBB.y; }
		if (minPoint.z < objects[i]->minPointBB.z) { minPoint.z = objects[i]->minPointBB.z; }

		if (maxPoint.x < objects[i]->maxPointBB.x) { maxPoint.x = objects[i]->maxPointBB.x; }
		if (maxPoint.y < objects[i]->maxPointBB.y) { maxPoint.y = objects[i]->maxPointBB.y; }
		if (maxPoint.z > objects[i]->maxPointBB.z) { maxPoint.z = objects[i]->maxPointBB.z; }
	}
}

template <class objectType>
BoundingBox<objectType>::~BoundingBox()
{
}

template <class objectType>
bool BoundingBox<objectType>::Hit(const Ray& ray)
{
	float t;

	float dirfrac_x = 1.0f / ray.Direction->x;
	float dirfrac_y = 1.0f / ray.Direction->y;
	float dirfrac_z = 1.0f / ray.Direction->z;

	float t1 = (minPoint.x - ray.Origin->x) * dirfrac_x;
	float t2 = (maxPoint.x - ray.Origin->x) * dirfrac_x;
	float t3 = (minPoint.y - ray.Origin->y) * dirfrac_y;
	float t4 = (maxPoint.y - ray.Origin->y) * dirfrac_y;
	float t5 = (minPoint.z - ray.Origin->z) * dirfrac_z;
	float t6 = (maxPoint.z - ray.Origin->z) * dirfrac_z;

	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	if (tmax <= 0)
	{
		t = tmax;
		return false;
	}

	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	//!! for some reason tmax works here instead of the tmin here like in box.cpp
	t = tmax;

	if (t > 0.0f)
	{
		return true;
	}
	return false;
}

