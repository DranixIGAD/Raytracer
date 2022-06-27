#include "Box.h"
//#include <algorithm>

Box::Box()
{
	minPoint = new Vector3(0.5f, 0, 0);
	maxPoint = new Vector3(-0.5f, 1, 1);
	minPointOG = minPointBB = *minPoint;
	maxPointOG = maxPointBB = *maxPoint;
	masterTransform = nullptr; 
	masterInverse = nullptr;
	printf("[WARNING!] Using the default constructor for a box, this might not be what you want it to be...\n");
}

Box::Box(float width_x, float height_y, float depth_z)
{
	if (width_x <= 0 || height_y <= 0 || depth_z <= 0)
	{
		printf("[WARNING!] one of the dimensions of this box is not larger than 0, results could be unexpected");
	}
	
	float half_width = width_x / 2.0f;
	float half_height = height_y / 2.0f;
	float half_depth = depth_z / 2.0f;

	
	minPoint = new Vector3(-half_width, -half_height, half_depth);
	maxPoint = new Vector3(half_width, half_height, -half_depth);
	minPointOG = minPointBB = *minPoint;
	maxPointOG = maxPointBB = *maxPoint;
	masterTransform = nullptr; 
	masterInverse = nullptr; 
}

Box::~Box()
{
	delete minPoint;
	delete maxPoint;
}

bool Box::Hit(const Ray& ray)
{
	if (masterTransform != nullptr)
	{
		const Vector3 rotatedDirection = masterTransform->times(*ray.Direction, 0);
		const Vector3 rotatedOrigin = *masterTransform * *ray.Origin;	

		float dirfrac_x = 1.0f / rotatedDirection.x;
		float dirfrac_y = 1.0f / rotatedDirection.y;
		float dirfrac_z = 1.0f / rotatedDirection.z;

		float t1 = (minPoint->x - rotatedOrigin.x) * dirfrac_x;
		float t2 = (maxPoint->x - rotatedOrigin.x) * dirfrac_x;
		float t3 = (minPoint->y - rotatedOrigin.y) * dirfrac_y;
		float t4 = (maxPoint->y - rotatedOrigin.y) * dirfrac_y;
		float t5 = (minPoint->z - rotatedOrigin.z) * dirfrac_z;
		float t6 = (maxPoint->z - rotatedOrigin.z) * dirfrac_z;

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

		t = tmin;

		if (t > 0.0f)
		{
			hitPos = *ray.Origin + *ray.Direction * t;
			localHitPos = rotatedOrigin + rotatedDirection * t;
			return true;
		}
		return false;
	}

	//else
	float dirfrac_x = 1.0f / ray.Direction->x;
	float dirfrac_y = 1.0f / ray.Direction->y;
	float dirfrac_z = 1.0f / ray.Direction->z;

	float t1 = (minPoint->x - ray.Origin->x) * dirfrac_x;
	float t2 = (maxPoint->x - ray.Origin->x) * dirfrac_x;
	float t3 = (minPoint->y - ray.Origin->y) * dirfrac_y;
	float t4 = (maxPoint->y - ray.Origin->y) * dirfrac_y;
	float t5 = (minPoint->z - ray.Origin->z) * dirfrac_z;
	float t6 = (maxPoint->z - ray.Origin->z) * dirfrac_z;

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

	t = tmin;

	if (t > 0.0f)
	{
		hitPos = *ray.Origin + *ray.Direction * t;
		localHitPos = hitPos;
		return true;
	}
	return false;
}

bool Box::HitShadow(const Ray& ray, const float& distanceToLight)
{
	float tempT = NULL;

	if (masterTransform != nullptr)
	{
		const Vector3 rotatedDirection = masterTransform->times(*ray.Direction, 0);
		const Vector3 rotatedOrigin = *masterTransform * *ray.Origin;

		float dirfrac_x = 1.0f / rotatedDirection.x;
		float dirfrac_y = 1.0f / rotatedDirection.y;
		float dirfrac_z = 1.0f / rotatedDirection.z;

		float t1 = (minPoint->x - rotatedOrigin.x) * dirfrac_x;
		float t2 = (maxPoint->x - rotatedOrigin.x) * dirfrac_x;
		float t3 = (minPoint->y - rotatedOrigin.y) * dirfrac_y;
		float t4 = (maxPoint->y - rotatedOrigin.y) * dirfrac_y;
		float t5 = (minPoint->z - rotatedOrigin.z) * dirfrac_z;
		float t6 = (maxPoint->z - rotatedOrigin.z) * dirfrac_z;

		float tmin = std::max(std::max(std::min(t1, t2),std::min(t3, t4)), std::min(t5, t6));
		float tmax = std::min(std::min(std::max(t1, t2),std::max(t3, t4)), std::max(t5, t6));

		if (tmax < 0)
		{
			tempT = tmax;
			return false;
		}

		if (tmin > tmax)
		{
			tempT = tmax;
			return false;
		}

		tempT = tmin;

		const Vector3 ShadowIntersectionPoint = (*ray.Origin + (tempT * *ray.Direction));
		const Vector3 toIntersectionVectorLength = ShadowIntersectionPoint - *ray.Origin;
		const float toIntersection = toIntersectionVectorLength.Magnitude();

		if (tempT >= 0 && toIntersection <= distanceToLight)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	float dirfrac_x = 1.0f / ray.Direction->x;
	float dirfrac_y = 1.0f / ray.Direction->y;
	float dirfrac_z = 1.0f / ray.Direction->z;

	float t1 = (minPoint->x - ray.Origin->x) * dirfrac_x;
	float t2 = (maxPoint->x - ray.Origin->x) * dirfrac_x;
	float t3 = (minPoint->y - ray.Origin->y) * dirfrac_y;
	float t4 = (maxPoint->y - ray.Origin->y) * dirfrac_y;
	float t5 = (minPoint->z - ray.Origin->z) * dirfrac_z;
	float t6 = (maxPoint->z - ray.Origin->z) * dirfrac_z;

	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));
	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));

	if (tmax < 0)
	{
		tempT = tmax;
		return false;
	}

	if (tmin > tmax)
	{
		tempT = tmax;
		return false;
	}


	tempT = tmin;

	const Vector3 ShadowIntersectionPoint = (*ray.Origin + (tempT * *ray.Direction));
	const Vector3 toIntersectionVectorLength = ShadowIntersectionPoint - *ray.Origin;
	const float toIntersection = toIntersectionVectorLength.Magnitude();

	if (tempT >= 0 && toIntersection <= distanceToLight)
	{
		return true;
	}
	else
	{
		return false;
	}

}

Vector3 Box::getNormal()
{
	if (localHitPos.x >= minPoint->x - 1e-4f && localHitPos.x <= maxPoint->x + 1e-4f &&
		localHitPos.y >= minPoint->y - 1e-4f && localHitPos.y <= maxPoint->y + 1e-4f &&
	   (localHitPos.z <= minPoint->z + 1e-4f && localHitPos.z >= minPoint->z - 1e-4f))
	{
		Vector3 normal(0, 0, 1);
		
		if (masterInverse != nullptr)
		{
			normal = masterInverse->times(normal, 0);
			return normal;			
		}
		
		return normal;
	}

	if (localHitPos.x >= minPoint->x - 1e-4f && localHitPos.x <= minPoint->x + 1e-4f &&
		localHitPos.y >= minPoint->y - 1e-4f && localHitPos.y <= maxPoint->y + 1e-4f &&
	   (localHitPos.z <= minPoint->z + 1e-4f && localHitPos.z >= maxPoint->z - 1e-4f))
	{
		Vector3 normal(-1, 0, 0);

		if (masterInverse != nullptr)
		{
			normal = masterInverse->times(normal, 0);
			return normal;
		}

		return normal;

	}

	if (localHitPos.x >= maxPoint->x - 1e-4f && localHitPos.x <= maxPoint->x + 1e-4f &&
		localHitPos.y >= minPoint->y - 1e-4f && localHitPos.y <= maxPoint->y + 1e-4f &&
	   (localHitPos.z <= minPoint->z + 1e-4f && localHitPos.z >= maxPoint->z - 1e-4f))
	{
		Vector3 normal(1, 0, 0);

		if (masterInverse != nullptr)
		{
			normal = masterInverse->times(normal, 0);
			return normal;
		}

		return normal;
	}

	if (localHitPos.x >= minPoint->x - 1e-4f && localHitPos.x <= maxPoint->x + 1e-4f &&
		localHitPos.y >= maxPoint->y - 1e-4f && localHitPos.y <= maxPoint->y + 1e-4f &&
	   (localHitPos.z <= minPoint->z + 1e-4f && localHitPos.z >= maxPoint->z - 1e-4f))
	{
		Vector3 normal(0, 1, 0);

		if (masterInverse != nullptr)
		{
			normal = masterInverse->times(normal, 0);
			return normal;
		}

		return normal;
	}

	if (localHitPos.x >= minPoint->x - 1e-4f && localHitPos.x <= maxPoint->x + 1e-4f &&
		localHitPos.y >= minPoint->y - 1e-4f && localHitPos.y <= minPoint->y + 1e-4f &&
	   (localHitPos.z <= minPoint->z + 1e-4f && localHitPos.z >= maxPoint->z - 1e-4f))
	{
		Vector3 normal(0, -1, 0);

		if (masterInverse != nullptr)
		{
			normal = masterInverse->times(normal, 0);
			return normal;
		}

		return normal;
	}

	if (localHitPos.x >= minPoint->x - 1e-4f && localHitPos.x <= maxPoint->x + 1e-4f &&
		localHitPos.y >= minPoint->y - 1e-4f && localHitPos.y <= maxPoint->y + 1e-4f &&
	   (localHitPos.z <= maxPoint->z + 1e-4f && localHitPos.z >= maxPoint->z - 1e-4f))
	{
		Vector3 normal(0, 0, -1);

		if (masterInverse != nullptr)
		{
			normal = masterInverse->times(normal, 0);
			return normal;
		}

		return normal;
	}

	printf("something went wrong");
	return Vector3(0);
}

Vector3 Box::get2ndRefractPoint(const Ray& ray)
{

	float dirfrac_x = 1.0f / ray.Direction->x;
	float dirfrac_y = 1.0f / ray.Direction->y;
	float dirfrac_z = 1.0f / ray.Direction->z;

	float t1 = (minPoint->x - ray.Origin->x) * dirfrac_x;
	float t2 = (maxPoint->x - ray.Origin->x) * dirfrac_x;
	float t3 = (minPoint->y - ray.Origin->y) * dirfrac_y;
	float t4 = (maxPoint->y - ray.Origin->y) * dirfrac_y;
	float t5 = (minPoint->z - ray.Origin->z) * dirfrac_z;
	float t6 = (maxPoint->z - ray.Origin->z) * dirfrac_z;

	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	return Vector3(*ray.Origin + *ray.Direction * tmax);
}

void Box::set_rotation_x(float angle)
{
	Matrix44 rotation;
	rotation.makeXRotation(angle);

	Matrix44 transposed(rotation);
	transposed.transpose();

	if (masterTransform == nullptr)
	{
		masterTransform = new Matrix44(rotation);
		masterInverse = new Matrix44(transposed);
		redo_BB(*masterInverse);
	}
	else
	{
		*masterTransform = rotation * *masterTransform;
		*masterInverse = *masterInverse * transposed;
	}

	redo_BB(*masterInverse);
}

void Box::set_rotation_y(float angle)
{
	Matrix44 rotation;
	rotation.makeYRotation(angle);
	
	Matrix44 transposed(rotation);
	transposed.transpose();

	if (masterTransform == nullptr)
	{
		masterTransform = new Matrix44(rotation);
		masterInverse = new Matrix44(transposed);
		redo_BB(*masterInverse);
	}
	else
	{
		*masterTransform = rotation * *masterTransform;
		*masterInverse = *masterInverse * transposed;
	}

	redo_BB(*masterInverse);
}

void Box::set_rotation_z(float angle)
{
	Matrix44 rotation;
	rotation.makeZRotation(angle);

	Matrix44 transposed(rotation);
	transposed.transpose();

	if (masterTransform == nullptr)
	{
		masterTransform = new Matrix44(rotation);
		masterInverse = new Matrix44(transposed);
		redo_BB(*masterInverse);
	}
	else
	{
		*masterTransform = rotation * *masterTransform;
		*masterInverse = *masterInverse * transposed;
	}

	redo_BB(*masterInverse);
}

void Box::set_translation(float x_trans, float y_trans, float z_trans)
{
	Matrix44 translation;	
	translation.makeTranslation(-x_trans, -y_trans, -z_trans);

	Matrix44 opposite;
	opposite.makeTranslation(x_trans, y_trans, z_trans);
	

	if (masterTransform == nullptr)
	{
		masterTransform = new Matrix44(translation);
		masterInverse = new Matrix44(opposite);
	}
	else
	{
		*masterTransform = translation * *masterTransform;
		*masterInverse = *masterInverse * opposite;
	}
	
	redo_BB(*masterTransform);
}