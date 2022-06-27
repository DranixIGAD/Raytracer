#include "Triangle.h"

Triangle::Triangle()
{
	pA = new Vector3(0);
	pB = new Vector3(1, 0, 0);
	pC = new Vector3(0, 1, 0);
	printf("[WARNING!] Using the default constructor for a triangle, this might not be what you want it to be...\n");
}

Triangle::Triangle(Vector3 pa, Vector3 pb, Vector3 pc)
{
	pA = new Vector3(pa);
	pB = new Vector3(pb);
	pC = new Vector3(pc);	
}

Triangle::~Triangle()
{
}

bool Triangle::Hit(const Ray& ray)
{
	const float a = pA->x - pB->x;
	const float b = pA->y - pB->y;
	const float c = pA->z - pB->z;
	
	const float d = pA->x - pC->x;
	const float e = pA->y - pC->y;
	const float f = pA->z - pC->z;
	
	const float g = NULL /*Xd*/;
	const float h = NULL /*Yd*/;
	const float i = NULL /*Zd*/;
	
	const float j = pA->x /* - Xe*/;
	const float k = pA->y /* - Ye*/;
	const float l = pA->z /* - Ze*/;

	const float ei_minus_hf = e * i - h * f;
	const float gf_minus_di = g * f - d * i;
	const float dh_minus_eg = d * h - e * g;
	const float ak_minus_jb = a * k - j * b;
	const float jc_minus_al = j * c - a * l;
	const float bl_minus_kc = b * l - k * c;

	const float M = (a * ei_minus_hf + b * gf_minus_di + c * bl_minus_kc);

	
	t = (f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / M;
	
	if (t >= 0)
	{
		hitPos = *ray.Origin + t * *ray.Direction;
		
		const float Y = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / M;
		if (Y < 0 || Y > 1)
		{
			return false;
		}

		const float B = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / M;
		if (B < 0 || B > 1 - Y)
		{
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool Triangle::HitShadow(const Ray& /*ray*/, const float& /*distanceToLight*/)
{
	return false;
}

Vector3 Triangle::getNormal()
{
	Vector3 d1 = *pB - *pA;
	Vector3 d2 = *pC - *pA;

	Vector3 d3 = d1.Cross(d2);
	d3.Normalize();
	
	return d3;
}
