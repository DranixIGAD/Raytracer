#pragma once
#include "Surface.h"

class Box :	public Surface
{
public:
	Box();
	Box(float width_x, float height_y, float depth_z);
	~Box();

	bool Hit(const Ray& ray);
	bool HitShadow(const Ray& ray, const float& distanceToLight);
	Vector3 getNormal();
	Vector3 get2ndRefractPoint(const Ray& ray);
	void set_rotation_x(float angle);
	void set_rotation_y(float angle);
	void set_rotation_z(float angle);
	void set_translation(float x_trans, float y_trans, float z_trans);
	
	Vector3* minPoint;
	Vector3* maxPoint;
	Matrix44* masterTransform;
	Matrix44* masterInverse;
	Vector3 localHitPos;
};

