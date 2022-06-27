#include "Surface.h"
#include <iostream>
#include <algorithm>

Surface::Surface()
{
	Color.R = 0xff;
	Color.G = 0x00;
	Color.B = 0xff;

	Color2.R = 0xff;
	Color2.G = 0x00;
	Color2.B = 0xff;

	SpecularColor.R = 0x7F;
	SpecularColor.G = 0x7F;
	SpecularColor.B = 0x7F;

	AmbientColor.R = OriginalAmbientColor.R = 0xff;
	AmbientColor.G = OriginalAmbientColor.G = 0x00;
	AmbientColor.B = OriginalAmbientColor.B = 0xff;

	Phong = 100;
	ambientIntensity = 0.05f;

	chosenPattern = Patterns::plain;

	tempR = 0x00;
	tempG = 0x00;
	tempB = 0x00;

	currentReflectionDepth = 0;
	reflectionDepth = 3;
	reflectiveness = 0.5f;
	ior = 1.5f;
	
	t = NULL;
}

bool Surface::Hit(const Ray&)
{
	printf("[WARNING!] You are using a probably wrong hit function...\n");
	return false;
}

bool Surface::HitShadow(const Ray&, const float&)
{
	printf("[WARNING!] You are using a probably wrong hitShadow function...\n");
	return false;
}

color Surface::getColor(const Ray& ray, LightSource* lights[], int numberOfLights, myContainer<Surface*>& objects)
{
	Vector3 _temp_baseColor(0);

	switch (chosenPattern)
	{
	case  Patterns::mirror:
		{
			Vector3 reflectionDirection = *ray.Direction - 2 * (*ray.Direction * getNormal()) * getNormal();
			reflectionDirection.Normalize();
			const Vector3 reflectionOrigin = hitPos /*+ getNormal() * 1e-4f*/;
			const Ray reflectionRay(reflectionDirection, reflectionOrigin);
			
			Vector3 temp_Remember_Hitpos(hitPos);
			float t1 = INFINITY;
			bool hitSomething = false;
			int smallestK = NULL;
			for (int k = 0; k < objects.getSize(); k++)
			{
				if (objects[k]->Hit(reflectionRay) && objects[k]->t < t1)
				{
					smallestK = k;
					hitSomething = true;
					t1 = objects[k]->t;
				}
			}

			float kdR = 0;
			float kdG = 0;
			float kdB = 0;
			const float otherReflect = 1 - reflectiveness;

			if (hitSomething)
			{
				const Vector3 tempColor = objects[smallestK]->getMirrorColor(reflectionRay, lights, numberOfLights, objects, currentReflectionDepth);
				currentReflectionDepth = 0;	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

				kdB += static_cast<float>(tempColor.z);
				kdG += static_cast<float>(tempColor.y);
				kdR += static_cast<float>(tempColor.x);
			}
			else
			{
				const color bgColor(0x32, 0x4b, 0x77);			//temp

				kdR += static_cast<float>(bgColor.R);
				kdG += static_cast<float>(bgColor.G);
				kdB += static_cast<float>(bgColor.B);
			}

			hitPos = temp_Remember_Hitpos;

			color kd1 = getBaseColor();

			AmbientColor.R = static_cast<uint8_t>(kdR * reflectiveness) + static_cast<uint8_t>(static_cast<float>(OriginalAmbientColor.R)* otherReflect);
			AmbientColor.G = static_cast<uint8_t>(kdG * reflectiveness) + static_cast<uint8_t>(static_cast<float>(OriginalAmbientColor.G)* otherReflect);
			AmbientColor.B = static_cast<uint8_t>(kdB * reflectiveness) + static_cast<uint8_t>(static_cast<float>(OriginalAmbientColor.B)* otherReflect);


			_temp_baseColor.x += otherReflect * static_cast<float>(kd1.R) + reflectiveness * kdR;
			_temp_baseColor.y += otherReflect * static_cast<float>(kd1.G) + reflectiveness * kdG;
			_temp_baseColor.z += otherReflect * static_cast<float>(kd1.B) + reflectiveness * kdB;

			break;
		}
	case Patterns::refractor:
		{
			float N = 1 / ior;
			float C1 = getNormal() * *ray.Direction;
			float C2 = sqrtf(1 - (N * N) * (1 - C1 * C1));
			Vector3 newDirection(N * (*ray.Direction + C1 * getNormal()) - getNormal() * C2);
			newDirection.Normalize();
			
			Ray RefractionRay(newDirection, hitPos - 1 * getNormal());

			Vector3 temp_Hitpos = hitPos;

			hitPos = get2ndRefractPoint(RefractionRay);
			

			N = ior;
			C1 =  -1 * getNormal() * *RefractionRay.Direction;
			
			float A = 1 - (N * N) * (1 - C1 * C1);
			if (A < 0)
			{
				C2 = 0;
			}
			else 
			{
				C2 = sqrtf(A);		//can become -nan(ind) when reached critical angle
			}
			
			newDirection = (N * (*RefractionRay.Direction + C1 * -1 * getNormal()) - -1 * getNormal() * C2);

			newDirection.Normalize(); 
			Ray ExitRefractionRay(newDirection, hitPos + getNormal() * 1e-4f);


			float t1 = INFINITY;
			bool hitSomething = false;
			int smallestK = NULL;
			for (int k = 0; k < objects.getSize(); k++)
			{
				if (objects[k]->Hit(ExitRefractionRay) && objects[k]->t < t1)
				{
					smallestK = k;
					hitSomething = true;
					t1 = objects[k]->t;
				}
			}

			if (hitSomething)
			{
				const Vector3 tempColor = objects[smallestK]->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, currentReflectionDepth);
				currentReflectionDepth = 0;
				_temp_baseColor.x += static_cast<float>(tempColor.x);
				_temp_baseColor.y += static_cast<float>(tempColor.y);
				_temp_baseColor.z += static_cast<float>(tempColor.z);
			}
			else
			{
				const color bgColor(0x32, 0x4b, 0x77);

				_temp_baseColor.x += static_cast<float>(bgColor.R);
				_temp_baseColor.y += static_cast<float>(bgColor.G);
				_temp_baseColor.z += static_cast<float>(bgColor.B);
			}
			
			hitPos = temp_Hitpos;
			break;
		}
	default:
		{
			color kd1 = getBaseColor();

			_temp_baseColor.x += static_cast<float>(kd1.R);
			_temp_baseColor.y += static_cast<float>(kd1.G);
			_temp_baseColor.z += static_cast<float>(kd1.B);

			break;
		}
	}

	for (int i = 0; i < numberOfLights; i++)
	{

		Vector3 shadowDirection = *lights[i]->pos - hitPos + getNormal() * 1e-4f;
		const Vector3 shadowOrigin = hitPos + getNormal() * 1e-4f;

		const float distanceToLight = shadowDirection.Magnitude();
		shadowDirection.Normalize();

		const Ray shadowRay(shadowDirection, shadowOrigin);


		bool hits = false;
		for (int k = 0; k < objects.getSize(); k++)
		{
			if (objects[k]->HitShadow(shadowRay, distanceToLight) && objects[k]->chosenPattern != Patterns::refractor)
			{
				hits = true; 
				break;
			}
		}

		if (!hits)
		{
			Vector3 l = *lights[i]->pos - hitPos;
			l.Normalize();

			Vector3 h = -1 * (*ray.Direction - l);
			h.Normalize();

			float nl = getNormal() * l;
			float nh = getNormal() * h;

			tempR += static_cast<float>(_temp_baseColor.x) * lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.R)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
			tempG += static_cast<float>(_temp_baseColor.y) * lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.G)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
			tempB += static_cast<float>(_temp_baseColor.z) * lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.B)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
														   
		}
	}

	tempR += static_cast<float>(AmbientColor.R)* ambientIntensity;
	tempG += static_cast<float>(AmbientColor.G)* ambientIntensity;
	tempB += static_cast<float>(AmbientColor.B)* ambientIntensity;


	if (tempR > 0xff)
	{
		tempR = 0xff;
	}

	if (tempG > 0xff)
	{
		tempG = 0xff;
	}

	if (tempB > 0xff)
	{
		tempB = 0xff;
	}


	RealColor.R = static_cast<uint8_t>(tempR);
	RealColor.G = static_cast<uint8_t>(tempG);
	RealColor.B = static_cast<uint8_t>(tempB);

	tempR = tempB = tempG = 0;
	AmbientColor = OriginalAmbientColor;


	return RealColor;
}

color Surface::getBaseColor()
{
	switch (chosenPattern)
	{
	case Patterns::checkered:
	{
		int checker = (static_cast<int>(hitPos.x) + static_cast<int>(hitPos.z) + static_cast<int>(hitPos.y)) % 2;
		if (hitPos.x < 0) checker += 1;
		if (hitPos.z > 0) checker -= 1; 
			
		if (checker == 0 || checker == 2)
		{
			return Color;
		}
		else
		{
			return Color2;
		}
	}

	default:
		return Color;
	}
}

Vector3 Surface::getMirrorColor(const Ray& ray, LightSource* lights[], int numberOfLights, myContainer<Surface*>& objects, int& current_reflection_depth)
{
	Vector3 _temp_baseColor(0);
	Vector3 temp_ambientColor(0);
	Vector3 temp_Color(0);

	if (current_reflection_depth < reflectionDepth)
	{
		current_reflection_depth++;

		switch (chosenPattern)
		{
		case Patterns::mirror:
			{
				Vector3 reflectionDirection = *ray.Direction - 2 * (*ray.Direction * getNormal()) * getNormal();
				reflectionDirection.Normalize();
				const Vector3 reflectionOrigin = hitPos + getNormal() * 1e-4f;
				const Ray reflectionRay(reflectionDirection, reflectionOrigin);

				float t1 = INFINITY;
				bool hitSomething = false;
				int smallestK = NULL;
				for (int k = 0; k < objects.getSize(); k++)
				{
					if (objects[k]->Hit(reflectionRay) && objects[k]->t < t1)
					{
						smallestK = k;
						hitSomething = true;
						t1 = objects[k]->t;
					}
				}

				float kdR = 0;
				float kdG = 0;
				float kdB = 0;
				const float otherReflect = 1 - reflectiveness;

				if (hitSomething)
				{
					const Vector3 tempColor = objects[smallestK]->getMirrorColor(reflectionRay, lights, numberOfLights, objects, current_reflection_depth);

					kdB += static_cast<float>(tempColor.z);
					kdG += static_cast<float>(tempColor.y);
					kdR += static_cast<float>(tempColor.x);
				}
				else
				{
					const color bgColor(0x32, 0x4b, 0x77);

					kdR += static_cast<float>(bgColor.R);
					kdG += static_cast<float>(bgColor.G);
					kdB += static_cast<float>(bgColor.B);
				}

				color kd1 = getBaseColor();

				temp_ambientColor.x = (kdR * reflectiveness) + static_cast<float>(OriginalAmbientColor.R)* otherReflect;
				temp_ambientColor.y = (kdG * reflectiveness) + static_cast<float>(OriginalAmbientColor.G)* otherReflect;
				temp_ambientColor.z = (kdB * reflectiveness) + static_cast<float>(OriginalAmbientColor.B)* otherReflect;


				_temp_baseColor.x += otherReflect * static_cast<float>(kd1.R) + reflectiveness * kdR;
				_temp_baseColor.y += otherReflect * static_cast<float>(kd1.G) + reflectiveness * kdG;
				_temp_baseColor.z += otherReflect * static_cast<float>(kd1.B) + reflectiveness * kdB;
			}

			break;

		case Patterns::refractor:
		{
			float N = 1 / ior;
			float C1 = getNormal() * *ray.Direction;
			float C2 = sqrtf(1 - (N * N) * (1 - C1 * C1));
			Vector3 newDirection(N * (*ray.Direction + C1 * getNormal()) - getNormal() * C2);
			newDirection.Normalize();

			Ray RefractionRay(newDirection, hitPos - 1 * getNormal());

			Vector3 temp_Hitpos = hitPos;

			hitPos = get2ndRefractPoint(RefractionRay);


			N = ior / 1;
			C1 = 1 * getNormal() * *RefractionRay.Direction;
			C2 = sqrtf(1 - (N * N) * (1 - C1 * C1));
			newDirection = (N * (*RefractionRay.Direction + C1 * -1 * getNormal()) - -1 * getNormal() * C2);

			newDirection.Normalize(); 	
			Ray ExitRefractionRay(newDirection, hitPos + getNormal() * 1e-4f);


			float t1 = INFINITY;
			bool hitSomething = false;
			int smallestK = NULL;
			for (int k = 0; k < objects.getSize(); k++)
			{
				if (objects[k]->Hit(ExitRefractionRay) && objects[k]->t < t1)
				{
					smallestK = k;
					hitSomething = true;
					t1 = objects[k]->t;
				}
			}

			if (hitSomething)
			{
				const Vector3 tempColor = objects[smallestK]->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, currentReflectionDepth);
				currentReflectionDepth = 0;
				_temp_baseColor.x += static_cast<float>(tempColor.x);
				_temp_baseColor.y += static_cast<float>(tempColor.y);
				_temp_baseColor.z += static_cast<float>(tempColor.z);
			}
			else
			{
				const color bgColor(0x32, 0x4b, 0x77);

				_temp_baseColor.x += static_cast<float>(bgColor.R);
				_temp_baseColor.y += static_cast<float>(bgColor.G);
				_temp_baseColor.z += static_cast<float>(bgColor.B);
			}

			hitPos = temp_Hitpos;
			break;
		}

		default:
			{
				color kd1 = getBaseColor();

				_temp_baseColor.x += static_cast<float>(kd1.R);
				_temp_baseColor.y += static_cast<float>(kd1.G);
				_temp_baseColor.z += static_cast<float>(kd1.B);

				temp_ambientColor.x = (static_cast<float>(OriginalAmbientColor.R));
				temp_ambientColor.y = (static_cast<float>(OriginalAmbientColor.G));
				temp_ambientColor.z = (static_cast<float>(OriginalAmbientColor.B));
			}

			break;
		}

		for (int i = 0; i < numberOfLights; i++)
		{

			Vector3 shadowDirection = *lights[i]->pos - hitPos + getNormal() * 1e-4f;
			const Vector3 shadowOrigin = hitPos + getNormal() * 1e-4f;

			const float distanceToLight = shadowDirection.Magnitude();
			shadowDirection.Normalize();

			const Ray shadowRay(shadowDirection, shadowOrigin);


			bool hits = false;
			for (int k = 0; k < objects.getSize(); k++)
			{
				if (objects[k]->HitShadow(shadowRay, distanceToLight))
				{
					hits = true;
					break;
				}
			}

			if (!hits)
			{
				Vector3 l = *lights[i]->pos - hitPos;
				l.Normalize();

				Vector3 h = -1 * (*ray.Direction - l);
				h.Normalize();

				float nl = getNormal() * l;
				float nh = getNormal() * h;

				temp_Color.x += static_cast<float>(_temp_baseColor.x)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.R)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.y += static_cast<float>(_temp_baseColor.y)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.G)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.z += static_cast<float>(_temp_baseColor.z)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.B)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);

			}
		}

		temp_Color.x += temp_ambientColor.x * ambientIntensity;
		temp_Color.y += temp_ambientColor.y * ambientIntensity;
		temp_Color.z += temp_ambientColor.z * ambientIntensity;


		if (temp_Color.x > 0xff)
		{
			temp_Color.x = 0xff;
		}

		if (temp_Color.y > 0xff)
		{
			temp_Color.y = 0xff;
		}

		if (temp_Color.z > 0xff)
		{
			temp_Color.z = 0xff;
		}

		return temp_Color;
	}
	else
	{
		color kd1 = getBaseColor();

		_temp_baseColor.x += static_cast<float>(kd1.R);
		_temp_baseColor.y += static_cast<float>(kd1.G);
		_temp_baseColor.z += static_cast<float>(kd1.B);

		const float otherReflect = 1 - reflectiveness;

		temp_ambientColor.x = (static_cast<float>(OriginalAmbientColor.R)* otherReflect);
		temp_ambientColor.y = (static_cast<float>(OriginalAmbientColor.G)* otherReflect);
		temp_ambientColor.z = (static_cast<float>(OriginalAmbientColor.B)* otherReflect);

		for (int i = 0; i < numberOfLights; i++)
		{

			Vector3 shadowDirection = *lights[i]->pos - hitPos + getNormal() * 1e-4f;
			const Vector3 shadowOrigin = hitPos + getNormal() * 1e-4f;

			const float distanceToLight = shadowDirection.Magnitude();
			shadowDirection.Normalize();

			const Ray shadowRay(shadowDirection, shadowOrigin);


			bool hits = false;
			for (int k = 0; k < objects.getSize(); k++)
			{
				if (objects[k]->HitShadow(shadowRay, distanceToLight) && objects[k]->chosenPattern != Patterns::refractor)
				{
					hits = true;
					break;
				}
			}

			if (!hits)
			{
				Vector3 l = *lights[i]->pos - hitPos;
				l.Normalize();

				Vector3 h = -1 * (*ray.Direction - l);
				h.Normalize();

				float nl = getNormal() * l;
				float nh = getNormal() * h;

				temp_Color.x += static_cast<float>(_temp_baseColor.x)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.R)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.y += static_cast<float>(_temp_baseColor.y)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.G)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.z += static_cast<float>(_temp_baseColor.z)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.B)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);

			}
		}
		
		temp_Color.x += temp_ambientColor.x * ambientIntensity;
		temp_Color.y += temp_ambientColor.y * ambientIntensity;
		temp_Color.z += temp_ambientColor.z * ambientIntensity;


		if (temp_Color.x > 0xff)
		{
			temp_Color.x = 0xff;
		}

		if (temp_Color.y > 0xff)
		{
			temp_Color.y = 0xff;
		}

		if (temp_Color.z > 0xff)
		{
			temp_Color.z = 0xff;
		}

		return temp_Color;
	}
}

void Surface::setColor(uint8_t R, uint8_t G, uint8_t B)
{
	Color.R = AmbientColor.R = OriginalAmbientColor.R = R;
	Color.G = AmbientColor.G = OriginalAmbientColor.G = G;
	Color.B = AmbientColor.B = OriginalAmbientColor.B = B;
}

void Surface::setSpecularColor(uint8_t R, uint8_t G, uint8_t B)
{
	SpecularColor.R = R;
	SpecularColor.G = G;
	SpecularColor.B = B;
}

void Surface::setAmbientColor(uint8_t R, uint8_t G, uint8_t B)
{
	AmbientColor.R = R;
	AmbientColor.G = G;
	AmbientColor.B = B;
}

void Surface::setPhong(float value)
{
	if (value < 0)
	{
		printf("[WARNING!] value for Phong was negative and has been set to 1\n");
		Phong = 1;
	}
	else
	{
		Phong = value;
	}
	
}

void Surface::setAmbientIntensity(float value)
{
	if (value < 0)
	{
		printf("[WARNING!] value for ambientIntensity was negative and has been set to 1\n");
		ambientIntensity = 1;
	}
	else
	{
		ambientIntensity = value;
	}

	tempR = static_cast<float>(AmbientColor.R)* ambientIntensity;
	tempG = static_cast<float>(AmbientColor.G)* ambientIntensity;
	tempB = static_cast<float>(AmbientColor.B)* ambientIntensity;
}

void Surface::setReflectiveness(float value)
{
	if (value < 0)
	{
		printf("[WARNING!] value for reflectiveness was lower than 0 and has been set to 0\n");
		reflectiveness = 0;
	}
	else if(value > 1)
	{
		printf("[WARNING!] value for reflectiveness was higher than 1 and has been set to 1\n");
		reflectiveness = 1;
	}
	else
	{
		reflectiveness = value;
	}

}

void Surface::setIor(float value)
{
	if (value < 0)
	{
		printf("[WARNING!] value for ior was negative and has been set to 1\n");
		ior = 1;
	}
	else
	{
		ior = value;
	}

}

Vector3 Surface::getNormal()
{
	printf("[WARNING!] You are using a probably wrong hit function...\n");
	return Vector3(0);
}

Vector3 Surface::get2ndRefractPoint(const Ray& /*ray*/)
{
	printf("[WARNING!] You are using a probably wrong get2ndRefractPoint function...\n");
	return Vector3(0);
}

void Surface::set_rotation_x(float /*angle*/)
{
	printf("[WARNING!] You are trying to rotate something that shouldn't be able to rotate \n");
}

void Surface::set_rotation_y(float /*angle*/)
{
	printf("[WARNING!] You are trying to rotate something that shouldn't be able to rotate \n");
}

void Surface::set_rotation_z(float /*angle*/)
{
	printf("[WARNING!] You are trying to rotate something that shouldn't be able to rotate \n");
}

void Surface::set_translation(float /*x_trans*/, float /*y_trans*/, float /*z_trans*/)
{
	printf("[WARNING!] You are trying to translate something that shouldn't be able to translate \n");
}

void Surface::redo_BB(Matrix44 masterTransform)
{
	Vector3 transformed_og_minpoint = masterTransform * minPointOG;
	Vector3 transformed_og_maxpoint = masterTransform * maxPointOG;

	Vector3 corner1 = transformed_og_minpoint;

	Vector3 corner2(minPointOG.x, minPointOG.y, maxPointOG.z);
	corner2 = masterTransform * corner2;

	Vector3 corner3(maxPointOG.x, minPointOG.y, maxPointOG.z);
	corner3 = masterTransform * corner3;

	Vector3 corner4(maxPointOG.x, minPointOG.y, minPointBB.z);
	corner4 = masterTransform * corner4;

	Vector3 corner5(minPointOG.x, maxPointOG.y, minPointOG.z);
	corner5 = masterTransform * corner5;

	Vector3 corner6(minPointOG.x, maxPointOG.y, maxPointOG.z);
	corner6 = masterTransform * corner6;

	Vector3 corner7 = transformed_og_maxpoint;

	Vector3 corner8(maxPointOG.x, maxPointOG.y, minPointOG.z);
	corner8 = masterTransform * corner8;


	//minPoint aabb
	minPointBB.x = std::min(std::min(std::min(corner1.x, corner2.x), std::min(corner3.x, corner4.x)), std::min(std::min(corner5.x, corner6.x), std::min(corner7.x, corner8.x)));
	minPointBB.y = std::min(std::min(std::min(corner1.y, corner2.y), std::min(corner3.y, corner4.y)), std::min(std::min(corner5.y, corner6.y), std::min(corner7.y, corner8.y)));
	maxPointBB.z = std::max(std::max(std::max(corner1.z, corner2.z), std::max(corner3.z, corner4.z)), std::max(std::max(corner5.z, corner6.z), std::max(corner7.z, corner8.z)));
				   
	//maxPoint aabb
	maxPointBB.x = std::max(std::max(std::max(corner1.x, corner2.x), std::max(corner3.x, corner4.x)), std::max(std::max(corner5.x, corner6.x), std::max(corner7.x, corner8.x)));
	maxPointBB.y = std::max(std::max(std::max(corner1.y, corner2.y), std::max(corner3.y, corner4.y)), std::max(std::max(corner5.y, corner6.y), std::max(corner7.y, corner8.y)));
	maxPointBB.z = std::min(std::min(std::min(corner1.z, corner2.z), std::min(corner3.z, corner4.z)), std::min(std::min(corner5.z, corner6.z), std::min(corner7.z, corner8.z)));
}

color Surface::getColor(const Ray& ray, LightSource* lights[], int numberOfLights, myContainer<Surface*>& objects, myContainer<Surface*>& InfiniteObjects)
{
	Vector3 _temp_baseColor(0);

	switch (chosenPattern)
	{
	case  Patterns::mirror:
	{
		Vector3 reflectionDirection = *ray.Direction - 2 * (*ray.Direction * getNormal()) * getNormal();
		reflectionDirection.Normalize();
		const Vector3 reflectionOrigin = hitPos /*+ getNormal() * 1e-4f*/;
		const Ray reflectionRay(reflectionDirection, reflectionOrigin);

		Vector3 temp_Remember_Hitpos(hitPos);
		float t1 = INFINITY;
		bool hitSomething = false;
		int smallestK = NULL;
		for (int k = 0; k < objects.getSize(); k++)
		{
			if (objects[k]->Hit(reflectionRay) && objects[k]->t < t1)
			{
				smallestK = k;
				hitSomething = true;
				t1 = objects[k]->t;
			}
		}

		bool hitInfinite = false;
		for (int k = 0; k < InfiniteObjects.getSize(); k++)
		{
			if (InfiniteObjects[k]->Hit(reflectionRay) && InfiniteObjects[k]->t < t1)
			{
				smallestK = k;
				hitSomething = true;
				hitInfinite = true;
				t1 = InfiniteObjects[k]->t;
			}
		}

		float kdR = 0;
		float kdG = 0;
		float kdB = 0;
		const float otherReflect = 1 - reflectiveness;

		if (hitSomething)
		{
			if (!hitInfinite)
			{
				const Vector3 tempColor = objects[smallestK]->getMirrorColor(reflectionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

				kdB += static_cast<float>(tempColor.z);
				kdG += static_cast<float>(tempColor.y);
				kdR += static_cast<float>(tempColor.x);
			}
			else
			{
				const Vector3 tempColor = InfiniteObjects[smallestK]->getMirrorColor(reflectionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

				kdB += static_cast<float>(tempColor.z);
				kdG += static_cast<float>(tempColor.y);
				kdR += static_cast<float>(tempColor.x);
			}
			currentReflectionDepth = 0;

		}
		else
		{
			const color bgColor(0x32, 0x4b, 0x77);

			kdR += static_cast<float>(bgColor.R);
			kdG += static_cast<float>(bgColor.G);
			kdB += static_cast<float>(bgColor.B);
		}

		hitPos = temp_Remember_Hitpos;

		color kd1 = getBaseColor();

		AmbientColor.R = static_cast<uint8_t>(kdR * reflectiveness) + static_cast<uint8_t>(static_cast<float>(OriginalAmbientColor.R)* otherReflect);
		AmbientColor.G = static_cast<uint8_t>(kdG * reflectiveness) + static_cast<uint8_t>(static_cast<float>(OriginalAmbientColor.G)* otherReflect);
		AmbientColor.B = static_cast<uint8_t>(kdB * reflectiveness) + static_cast<uint8_t>(static_cast<float>(OriginalAmbientColor.B)* otherReflect);


		_temp_baseColor.x += otherReflect * static_cast<float>(kd1.R) + reflectiveness * kdR;
		_temp_baseColor.y += otherReflect * static_cast<float>(kd1.G) + reflectiveness * kdG;
		_temp_baseColor.z += otherReflect * static_cast<float>(kd1.B) + reflectiveness * kdB;

		break;
	}
	case Patterns::refractor:
	{
		float N = 1 / ior;
		float C1 = getNormal() * *ray.Direction;
		float C2 = sqrtf(1 - (N * N) * (1 - C1 * C1));
		Vector3 newDirection(N * (*ray.Direction + C1 * getNormal()) - getNormal() * C2);
		newDirection.Normalize();

		Ray RefractionRay(newDirection, hitPos - 1 * getNormal());

		Vector3 temp_Hitpos = hitPos;

		hitPos = get2ndRefractPoint(RefractionRay);


		N = ior;
		C1 = -1 * getNormal() * *RefractionRay.Direction;

		float A = 1 - (N * N) * (1 - C1 * C1);
		if (A < 0)
		{
			C2 = 0;
		}
		else
		{
			C2 = sqrtf(A);		//can become -nan(ind) when reached critical angle
		}

		newDirection = (N * (*RefractionRay.Direction + C1 * -1 * getNormal()) - -1 * getNormal() * C2);

		newDirection.Normalize(); 
		Ray ExitRefractionRay(newDirection, hitPos + getNormal() * 1e-4f);


		float t1 = INFINITY;
		bool hitSomething = false;
		int smallestK = NULL;
		for (int k = 0; k < objects.getSize(); k++)
		{
			if (objects[k]->Hit(ExitRefractionRay) && objects[k]->t < t1)
			{
				smallestK = k;
				hitSomething = true;
				t1 = objects[k]->t;
			}
		}

		bool hitInfinite = false;
		for (int k = 0; k < InfiniteObjects.getSize(); k++)
		{
			if (InfiniteObjects[k]->Hit(ExitRefractionRay) && InfiniteObjects[k]->t < t1)
			{
				smallestK = k;
				hitSomething = true;
				hitInfinite = true;
				t1 = InfiniteObjects[k]->t;
			}
		}

		if (hitSomething)
		{
			if (!hitInfinite)
			{
				const Vector3 tempColor = objects[smallestK]->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

				_temp_baseColor.x += static_cast<float>(tempColor.z);
				_temp_baseColor.y += static_cast<float>(tempColor.y);
				_temp_baseColor.z += static_cast<float>(tempColor.x);
			}
			else
			{
				const Vector3 tempColor = InfiniteObjects[smallestK]->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

				_temp_baseColor.x += static_cast<float>(tempColor.z);
				_temp_baseColor.y += static_cast<float>(tempColor.y);
				_temp_baseColor.z += static_cast<float>(tempColor.x);
			}

			currentReflectionDepth = 0;
		}
		else
		{
			const color bgColor(0x32, 0x4b, 0x77);

			_temp_baseColor.x += static_cast<float>(bgColor.R);
			_temp_baseColor.y += static_cast<float>(bgColor.G);
			_temp_baseColor.z += static_cast<float>(bgColor.B);
		}

		hitPos = temp_Hitpos;
		break;
	}
	default:
	{
		color kd1 = getBaseColor();

		_temp_baseColor.x += static_cast<float>(kd1.R);
		_temp_baseColor.y += static_cast<float>(kd1.G);
		_temp_baseColor.z += static_cast<float>(kd1.B);

		break;
	}
	}

	for (int i = 0; i < numberOfLights; i++)
	{

		Vector3 shadowDirection = *lights[i]->pos - hitPos + getNormal() * 1e-4f;
		const Vector3 shadowOrigin = hitPos + getNormal() * 1e-4f;

		const float distanceToLight = shadowDirection.Magnitude();
		shadowDirection.Normalize();

		const Ray shadowRay(shadowDirection, shadowOrigin);


		bool hits = false;
		for (int k = 0; k < objects.getSize(); k++)
		{
			if (objects[k]->HitShadow(shadowRay, distanceToLight) && objects[k]->chosenPattern != Patterns::refractor)
			{
				hits = true;
				break;
			}
		}

		for (int k = 0; k < InfiniteObjects.getSize(); k++)
		{
			if (InfiniteObjects[k]->HitShadow(shadowRay, distanceToLight) && InfiniteObjects[k]->chosenPattern != Patterns::refractor)
			{
				hits = true;
				break;
			}
		}

		if (!hits)
		{
			Vector3 l = *lights[i]->pos - hitPos;
			l.Normalize();

			Vector3 h = -1 * (*ray.Direction - l);
			h.Normalize();

			float nl = getNormal() * l;
			float nh = getNormal() * h;

			tempR += static_cast<float>(_temp_baseColor.x)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.R)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
			tempG += static_cast<float>(_temp_baseColor.y)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.G)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
			tempB += static_cast<float>(_temp_baseColor.z)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.B)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);

		}
	}

	tempR += static_cast<float>(AmbientColor.R)* ambientIntensity;
	tempG += static_cast<float>(AmbientColor.G)* ambientIntensity;
	tempB += static_cast<float>(AmbientColor.B)* ambientIntensity;


	if (tempR > 0xff)
	{
		tempR = 0xff;
	}

	if (tempG > 0xff)
	{
		tempG = 0xff;
	}

	if (tempB > 0xff)
	{
		tempB = 0xff;
	}


	RealColor.R = static_cast<uint8_t>(tempR);
	RealColor.G = static_cast<uint8_t>(tempG);
	RealColor.B = static_cast<uint8_t>(tempB);

	tempR = tempB = tempG = 0;
	AmbientColor = OriginalAmbientColor;


	return RealColor;
}

Vector3 Surface::getMirrorColor(const Ray& ray, LightSource* lights[], int numberOfLights, myContainer<Surface*>& objects, myContainer<Surface*>& InfiniteObjects, int& current_reflection_depth)
{
	Vector3 _temp_baseColor(0);
	Vector3 temp_ambientColor(0);
	Vector3 temp_Color(0);

	if (current_reflection_depth < reflectionDepth)
	{
		current_reflection_depth++;

		switch (chosenPattern)
		{
		case Patterns::mirror:
		{
			Vector3 reflectionDirection = *ray.Direction - 2 * (*ray.Direction * getNormal()) * getNormal();
			reflectionDirection.Normalize();
			const Vector3 reflectionOrigin = hitPos + getNormal() * 1e-4f;
			const Ray reflectionRay(reflectionDirection, reflectionOrigin);

			float t1 = INFINITY;
			bool hitSomething = false;
			int smallestK = NULL;
			for (int k = 0; k < objects.getSize(); k++)
			{
				if (objects[k]->Hit(reflectionRay) && objects[k]->t < t1)
				{
					smallestK = k;
					hitSomething = true;
					t1 = objects[k]->t;
				}
			}

			bool hitInfinite = false;
			for (int k = 0; k < InfiniteObjects.getSize(); k++)
			{
				if (InfiniteObjects[k]->Hit(reflectionRay) && InfiniteObjects[k]->t < t1)
				{
					smallestK = k;
					hitSomething = true;
					hitInfinite = true;
					t1 = InfiniteObjects[k]->t;
				}
			}

			float kdR = 0;
			float kdG = 0;
			float kdB = 0;
			const float otherReflect = 1 - reflectiveness;

			if (hitSomething)
			{
				if (!hitInfinite)
				{
					const Vector3 tempColor = objects[smallestK]->getMirrorColor(reflectionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

					kdB += static_cast<float>(tempColor.z);
					kdG += static_cast<float>(tempColor.y);
					kdR += static_cast<float>(tempColor.x);
				}
				else
				{
					const Vector3 tempColor = InfiniteObjects[smallestK]->getMirrorColor(reflectionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

					kdB += static_cast<float>(tempColor.z);
					kdG += static_cast<float>(tempColor.y);
					kdR += static_cast<float>(tempColor.x);
				}
			}
			else
			{
				const color bgColor(0x32, 0x4b, 0x77);

				kdR += static_cast<float>(bgColor.R);
				kdG += static_cast<float>(bgColor.G);
				kdB += static_cast<float>(bgColor.B);
			}

			color kd1 = getBaseColor();

			temp_ambientColor.x = (kdR * reflectiveness) + static_cast<float>(OriginalAmbientColor.R)* otherReflect;
			temp_ambientColor.y = (kdG * reflectiveness) + static_cast<float>(OriginalAmbientColor.G)* otherReflect;
			temp_ambientColor.z = (kdB * reflectiveness) + static_cast<float>(OriginalAmbientColor.B)* otherReflect;


			_temp_baseColor.x += otherReflect * static_cast<float>(kd1.R) + reflectiveness * kdR;
			_temp_baseColor.y += otherReflect * static_cast<float>(kd1.G) + reflectiveness * kdG;
			_temp_baseColor.z += otherReflect * static_cast<float>(kd1.B) + reflectiveness * kdB;
		}

		break;

		case Patterns::refractor:
		{
			float N = 1 / ior;
			float C1 = getNormal() * *ray.Direction;
			float C2 = sqrtf(1 - (N * N) * (1 - C1 * C1));
			Vector3 newDirection(N * (*ray.Direction + C1 * getNormal()) - getNormal() * C2);
			newDirection.Normalize();

			Ray RefractionRay(newDirection, hitPos - 1 * getNormal());

			Vector3 temp_Hitpos = hitPos;

			hitPos = get2ndRefractPoint(RefractionRay);


			N = ior / 1;
			C1 = 1 * getNormal() * *RefractionRay.Direction;
			C2 = sqrtf(1 - (N * N) * (1 - C1 * C1));
			newDirection = (N * (*RefractionRay.Direction + C1 * -1 * getNormal()) - -1 * getNormal() * C2);

			newDirection.Normalize();
			Ray ExitRefractionRay(newDirection, hitPos + getNormal() * 1e-4f);


			float t1 = INFINITY;
			bool hitSomething = false;
			int smallestK = NULL;
			for (int k = 0; k < objects.getSize(); k++)
			{
				if (objects[k]->Hit(ExitRefractionRay) && objects[k]->t < t1)
				{
					smallestK = k;
					hitSomething = true;
					t1 = objects[k]->t;
				}
			}

			bool hitInfinite = false;
			for (int k = 0; k < InfiniteObjects.getSize(); k++)
			{
				if (InfiniteObjects[k]->Hit(ExitRefractionRay) && InfiniteObjects[k]->t < t1)
				{
					smallestK = k;
					hitSomething = true;
					hitInfinite = true;
					t1 = InfiniteObjects[k]->t;
				}
			}

			if (hitSomething)
			{
				if (!hitInfinite)
				{
					const Vector3 tempColor = objects[smallestK]->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

					_temp_baseColor.x += static_cast<float>(tempColor.z);
					_temp_baseColor.y += static_cast<float>(tempColor.y);
					_temp_baseColor.z += static_cast<float>(tempColor.x);
				}
				else
				{
					const Vector3 tempColor = InfiniteObjects[smallestK]->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

					_temp_baseColor.x += static_cast<float>(tempColor.z);
					_temp_baseColor.y += static_cast<float>(tempColor.y);
					_temp_baseColor.z += static_cast<float>(tempColor.x);
				}
			}
			else
			{
				const color bgColor(0x32, 0x4b, 0x77);

				_temp_baseColor.x += static_cast<float>(bgColor.R);
				_temp_baseColor.y += static_cast<float>(bgColor.G);
				_temp_baseColor.z += static_cast<float>(bgColor.B);
			}

			hitPos = temp_Hitpos;
			break;
		}

		default:
		{
			color kd1 = getBaseColor();

			_temp_baseColor.x += static_cast<float>(kd1.R);
			_temp_baseColor.y += static_cast<float>(kd1.G);
			_temp_baseColor.z += static_cast<float>(kd1.B);

			temp_ambientColor.x = (static_cast<float>(OriginalAmbientColor.R));
			temp_ambientColor.y = (static_cast<float>(OriginalAmbientColor.G));
			temp_ambientColor.z = (static_cast<float>(OriginalAmbientColor.B));
		}

		break;
		}

		for (int i = 0; i < numberOfLights; i++)
		{

			Vector3 shadowDirection = *lights[i]->pos - hitPos + getNormal() * 1e-4f;
			const Vector3 shadowOrigin = hitPos + getNormal() * 1e-4f;

			const float distanceToLight = shadowDirection.Magnitude();
			shadowDirection.Normalize();

			const Ray shadowRay(shadowDirection, shadowOrigin);


			bool hits = false;
			for (int k = 0; k < objects.getSize(); k++)
			{
				if (objects[k]->HitShadow(shadowRay, distanceToLight))
				{
					hits = true;
					break;
				}
			}

			for (int k = 0; k < InfiniteObjects.getSize(); k++)
			{
				if (InfiniteObjects[k]->HitShadow(shadowRay, distanceToLight) && InfiniteObjects[k]->chosenPattern != Patterns::refractor)
				{
					hits = true;
					break;
				}
			}

			if (!hits)
			{
				Vector3 l = *lights[i]->pos - hitPos;
				l.Normalize();

				Vector3 h = -1 * (*ray.Direction - l);
				h.Normalize();

				float nl = getNormal() * l;
				float nh = getNormal() * h;

				temp_Color.x += static_cast<float>(_temp_baseColor.x)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.R)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.y += static_cast<float>(_temp_baseColor.y)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.G)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.z += static_cast<float>(_temp_baseColor.z)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.B)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);

			}
		}

		temp_Color.x += temp_ambientColor.x * ambientIntensity;
		temp_Color.y += temp_ambientColor.y * ambientIntensity;
		temp_Color.z += temp_ambientColor.z * ambientIntensity;


		if (temp_Color.x > 0xff)
		{
			temp_Color.x = 0xff;
		}

		if (temp_Color.y > 0xff)
		{
			temp_Color.y = 0xff;
		}

		if (temp_Color.z > 0xff)
		{
			temp_Color.z = 0xff;
		}

		return temp_Color;
	}
	else
	{
		color kd1 = getBaseColor();

		_temp_baseColor.x += static_cast<float>(kd1.R);
		_temp_baseColor.y += static_cast<float>(kd1.G);
		_temp_baseColor.z += static_cast<float>(kd1.B);

		const float otherReflect = 1 - reflectiveness;

		temp_ambientColor.x = (static_cast<float>(OriginalAmbientColor.R)* otherReflect);
		temp_ambientColor.y = (static_cast<float>(OriginalAmbientColor.G)* otherReflect);
		temp_ambientColor.z = (static_cast<float>(OriginalAmbientColor.B)* otherReflect);

		for (int i = 0; i < numberOfLights; i++)
		{

			Vector3 shadowDirection = *lights[i]->pos - hitPos + getNormal() * 1e-4f;
			const Vector3 shadowOrigin = hitPos + getNormal() * 1e-4f;

			const float distanceToLight = shadowDirection.Magnitude();
			shadowDirection.Normalize();

			const Ray shadowRay(shadowDirection, shadowOrigin);


			bool hits = false;
			for (int k = 0; k < objects.getSize(); k++)
			{
				if (objects[k]->HitShadow(shadowRay, distanceToLight) && objects[k]->chosenPattern != Patterns::refractor)
				{
					hits = true;
					break;
				}
			}

			if (!hits)
			{
				Vector3 l = *lights[i]->pos - hitPos;
				l.Normalize();

				Vector3 h = -1 * (*ray.Direction - l);
				h.Normalize();

				float nl = getNormal() * l;
				float nh = getNormal() * h;

				temp_Color.x += static_cast<float>(_temp_baseColor.x)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.R)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.y += static_cast<float>(_temp_baseColor.y)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.G)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.z += static_cast<float>(_temp_baseColor.z)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.B)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);

			}
		}

		temp_Color.x += temp_ambientColor.x * ambientIntensity;
		temp_Color.y += temp_ambientColor.y * ambientIntensity;
		temp_Color.z += temp_ambientColor.z * ambientIntensity;


		if (temp_Color.x > 0xff)
		{
			temp_Color.x = 0xff;
		}

		if (temp_Color.y > 0xff)
		{
			temp_Color.y = 0xff;
		}

		if (temp_Color.z > 0xff)
		{
			temp_Color.z = 0xff;
		}

		return temp_Color;
	}
}

color Surface::getColor(const Ray& ray, LightSource* lights[], int numberOfLights, BVH<Surface>& objects, myContainer<Surface*>& InfiniteObjects)
{
	Vector3 _temp_baseColor(0); 

	switch (chosenPattern)
	{
	case  Patterns::mirror:
	{
		Vector3 reflectionDirection = *ray.Direction - 2 * (*ray.Direction * getNormal()) * getNormal();
		reflectionDirection.Normalize();
		const Vector3 reflectionOrigin = hitPos /*+ getNormal() * 1e-4f*/;
		const Ray reflectionRay(reflectionDirection, reflectionOrigin);

		Vector3 temp_Remember_Hitpos(hitPos);
		float t1 = INFINITY;
		bool hitSomething = objects.checkHit(reflectionRay);
		int smallestK = NULL;

		if (hitSomething)
		{
			t1 = objects.closest()->t;
		}
			
		bool hitInfinite = false;
		for (int k = 0; k < InfiniteObjects.getSize(); k++)
		{
			if (InfiniteObjects[k]->Hit(reflectionRay) && InfiniteObjects[k]->t < t1)
			{
				smallestK = k;
				hitSomething = true;
				hitInfinite = true;
				t1 = InfiniteObjects[k]->t;
			}
		}

		float kdR = 0;
		float kdG = 0;
		float kdB = 0;
		const float otherReflect = 1 - reflectiveness;

		if (hitSomething)
		{
			if (!hitInfinite)
			{
				const Vector3 tempColor = objects.closest()->getMirrorColor(reflectionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

				kdB += static_cast<float>(tempColor.z);
				kdG += static_cast<float>(tempColor.y);
				kdR += static_cast<float>(tempColor.x);
			}
			else
			{
				const Vector3 tempColor = InfiniteObjects[smallestK]->getMirrorColor(reflectionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

				kdB += static_cast<float>(tempColor.z);
				kdG += static_cast<float>(tempColor.y);
				kdR += static_cast<float>(tempColor.x);
			}
			currentReflectionDepth = 0;	

		}
		else
		{
			const color bgColor(0x32, 0x4b, 0x77);

			kdR += static_cast<float>(bgColor.R);
			kdG += static_cast<float>(bgColor.G);
			kdB += static_cast<float>(bgColor.B);
		}

		hitPos = temp_Remember_Hitpos;

		color kd1 = getBaseColor();

		AmbientColor.R = static_cast<uint8_t>(kdR * reflectiveness) + static_cast<uint8_t>(static_cast<float>(OriginalAmbientColor.R)* otherReflect);
		AmbientColor.G = static_cast<uint8_t>(kdG * reflectiveness) + static_cast<uint8_t>(static_cast<float>(OriginalAmbientColor.G)* otherReflect);
		AmbientColor.B = static_cast<uint8_t>(kdB * reflectiveness) + static_cast<uint8_t>(static_cast<float>(OriginalAmbientColor.B)* otherReflect);


		_temp_baseColor.x += otherReflect * static_cast<float>(kd1.R) + reflectiveness * kdR;
		_temp_baseColor.y += otherReflect * static_cast<float>(kd1.G) + reflectiveness * kdG;
		_temp_baseColor.z += otherReflect * static_cast<float>(kd1.B) + reflectiveness * kdB;

		break;
	}
	case Patterns::refractor:
	{
		float N = 1 / ior;
		float C1 = getNormal() * *ray.Direction;
		float C2 = sqrtf(1 - (N * N) * (1 - C1 * C1));
		Vector3 newDirection(N * (*ray.Direction + C1 * getNormal()) - getNormal() * C2);
		newDirection.Normalize();

		Ray RefractionRay(newDirection, hitPos - 1 * getNormal());

		Vector3 temp_Hitpos = hitPos;

		hitPos = get2ndRefractPoint(RefractionRay);


		N = ior;
		C1 = -1 * getNormal() * *RefractionRay.Direction;

		float A = 1 - (N * N) * (1 - C1 * C1);
		if (A < 0)
		{
			C2 = 0;
		}
		else
		{
			C2 = sqrtf(A);		//can become -nan(ind) when reached critical angle
		}

		newDirection = (N * (*RefractionRay.Direction + C1 * -1 * getNormal()) - -1 * getNormal() * C2);

		newDirection.Normalize(); 
		Ray ExitRefractionRay(newDirection, hitPos + getNormal() * 1e-4f);


		float t1 = INFINITY;
		bool hitSomething = objects.checkHit(ExitRefractionRay);
		int smallestK = NULL;
			
		if (hitSomething)
		{
			t1 = objects.closest()->t;
		}

		bool hitInfinite = false;
		for (int k = 0; k < InfiniteObjects.getSize(); k++)
		{
			if (InfiniteObjects[k]->Hit(ExitRefractionRay) && InfiniteObjects[k]->t < t1)
			{
				smallestK = k;
				hitSomething = true;
				hitInfinite = true;
				t1 = InfiniteObjects[k]->t;
			}
		}

		if (hitSomething)
		{
			if (!hitInfinite)
			{
				const Vector3 tempColor = objects.closest()->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

				_temp_baseColor.x += static_cast<float>(tempColor.z);
				_temp_baseColor.y += static_cast<float>(tempColor.y);
				_temp_baseColor.z += static_cast<float>(tempColor.x);
			}
			else
			{
				const Vector3 tempColor = InfiniteObjects[smallestK]->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

				_temp_baseColor.x += static_cast<float>(tempColor.z);
				_temp_baseColor.y += static_cast<float>(tempColor.y);
				_temp_baseColor.z += static_cast<float>(tempColor.x);
			}

			currentReflectionDepth = 0;
		}
		else
		{
			const color bgColor(0x32, 0x4b, 0x77);

			_temp_baseColor.x += static_cast<float>(bgColor.R);
			_temp_baseColor.y += static_cast<float>(bgColor.G);
			_temp_baseColor.z += static_cast<float>(bgColor.B);
		}

		hitPos = temp_Hitpos;
		break;
	}
	default:
	{
		color kd1 = getBaseColor();

		_temp_baseColor.x += static_cast<float>(kd1.R);
		_temp_baseColor.y += static_cast<float>(kd1.G);
		_temp_baseColor.z += static_cast<float>(kd1.B);

		break;
	}
	}

	for (int i = 0; i < numberOfLights; i++)
	{

		Vector3 shadowDirection = *lights[i]->pos - hitPos + getNormal() * 1e-4f;
		const Vector3 shadowOrigin = hitPos + getNormal() * 1e-4f;

		const float distanceToLight = shadowDirection.Magnitude();
		shadowDirection.Normalize();

		const Ray shadowRay(shadowDirection, shadowOrigin);

		bool hits = objects.checkHit(shadowRay);

		if (hits && objects.closest()->t >= distanceToLight)
		{
			hits = false;
		}
		
		if (!hits)
		{
			for (int k = 0; k < InfiniteObjects.getSize(); k++)
			{
				if (InfiniteObjects[k]->HitShadow(shadowRay, distanceToLight) && InfiniteObjects[k]->chosenPattern != Patterns::refractor)
				{
					hits = true; 
					break;
				}
			}
		}

		if (!hits)
		{
			Vector3 l = *lights[i]->pos - hitPos;
			l.Normalize();

			Vector3 h = -1 * (*ray.Direction - l);
			h.Normalize();

			float nl = getNormal() * l;
			float nh = getNormal() * h;

			tempR += static_cast<float>(_temp_baseColor.x)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.R)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
			tempG += static_cast<float>(_temp_baseColor.y)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.G)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
			tempB += static_cast<float>(_temp_baseColor.z)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.B)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);

		}
	}

	tempR += static_cast<float>(AmbientColor.R)* ambientIntensity;
	tempG += static_cast<float>(AmbientColor.G)* ambientIntensity;
	tempB += static_cast<float>(AmbientColor.B)* ambientIntensity;


	if (tempR > 0xff)
	{
		tempR = 0xff;
	}

	if (tempG > 0xff)
	{
		tempG = 0xff;
	}

	if (tempB > 0xff)
	{
		tempB = 0xff;
	}


	RealColor.R = static_cast<uint8_t>(tempR);
	RealColor.G = static_cast<uint8_t>(tempG);
	RealColor.B = static_cast<uint8_t>(tempB);

	tempR = tempB = tempG = 0;
	AmbientColor = OriginalAmbientColor;


	return RealColor;
}

Vector3 Surface::getMirrorColor(const Ray& ray, LightSource* lights[], int numberOfLights, BVH<Surface>& objects, myContainer<Surface*>& InfiniteObjects, int& current_reflection_depth)
{
	Vector3 _temp_baseColor(0);
	Vector3 temp_ambientColor(0);
	Vector3 temp_Color(0);

	if (current_reflection_depth < reflectionDepth)
	{
		current_reflection_depth++;

		switch (chosenPattern)
		{
		case Patterns::mirror:
		{
			Vector3 reflectionDirection = *ray.Direction - 2 * (*ray.Direction * getNormal()) * getNormal();
			reflectionDirection.Normalize();
			const Vector3 reflectionOrigin = hitPos + getNormal() * 1e-4f;
			const Ray reflectionRay(reflectionDirection, reflectionOrigin);

			float t1 = INFINITY;
			bool hitSomething = objects.checkHit(reflectionRay);
			int smallestK = NULL;
				
			if (hitSomething)
			{
				t1 = objects.closest()->t;
			}

			bool hitInfinite = false;
			for (int k = 0; k < InfiniteObjects.getSize(); k++)
			{
				if (InfiniteObjects[k]->Hit(reflectionRay) && InfiniteObjects[k]->t < t1)
				{
					smallestK = k;
					hitSomething = true;
					hitInfinite = true;
					t1 = InfiniteObjects[k]->t;
				}
			}

			float kdR = 0;
			float kdG = 0;
			float kdB = 0;
			const float otherReflect = 1 - reflectiveness;

			if (hitSomething)
			{
				if (!hitInfinite)
				{
					const Vector3 tempColor = objects.closest()->getMirrorColor(reflectionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

					kdB += static_cast<float>(tempColor.z);
					kdG += static_cast<float>(tempColor.y);
					kdR += static_cast<float>(tempColor.x);
				}
				else
				{
					const Vector3 tempColor = InfiniteObjects[smallestK]->getMirrorColor(reflectionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

					kdB += static_cast<float>(tempColor.z);
					kdG += static_cast<float>(tempColor.y);
					kdR += static_cast<float>(tempColor.x);
				}
			}
			else
			{
				const color bgColor(0x32, 0x4b, 0x77);

				kdR += static_cast<float>(bgColor.R);
				kdG += static_cast<float>(bgColor.G);
				kdB += static_cast<float>(bgColor.B);
			}

			color kd1 = getBaseColor();

			temp_ambientColor.x = (kdR * reflectiveness) + static_cast<float>(OriginalAmbientColor.R)* otherReflect;
			temp_ambientColor.y = (kdG * reflectiveness) + static_cast<float>(OriginalAmbientColor.G)* otherReflect;
			temp_ambientColor.z = (kdB * reflectiveness) + static_cast<float>(OriginalAmbientColor.B)* otherReflect;


			_temp_baseColor.x += otherReflect * static_cast<float>(kd1.R) + reflectiveness * kdR;
			_temp_baseColor.y += otherReflect * static_cast<float>(kd1.G) + reflectiveness * kdG;
			_temp_baseColor.z += otherReflect * static_cast<float>(kd1.B) + reflectiveness * kdB;
		}

		break;

		case Patterns::refractor:
		{
			float N = 1 / ior;
			float C1 = getNormal() * *ray.Direction;
			float C2 = sqrtf(1 - (N * N) * (1 - C1 * C1));
			Vector3 newDirection(N * (*ray.Direction + C1 * getNormal()) - getNormal() * C2);
			newDirection.Normalize();

			Ray RefractionRay(newDirection, hitPos - 1 * getNormal());

			Vector3 temp_Hitpos = hitPos;

			hitPos = get2ndRefractPoint(RefractionRay);


			N = ior / 1;
			C1 = 1 * getNormal() * *RefractionRay.Direction;
			C2 = sqrtf(1 - (N * N) * (1 - C1 * C1));
			newDirection = (N * (*RefractionRay.Direction + C1 * -1 * getNormal()) - -1 * getNormal() * C2);

			newDirection.Normalize(); 	
			Ray ExitRefractionRay(newDirection, hitPos + getNormal() * 1e-4f);


			float t1 = INFINITY;
			bool hitSomething = objects.checkHit(ExitRefractionRay);
			int smallestK = NULL;
				
			if (hitSomething)
			{
				t1 = objects.closest()->t;
			}

			bool hitInfinite = false;
			for (int k = 0; k < InfiniteObjects.getSize(); k++)
			{
				if (InfiniteObjects[k]->Hit(ExitRefractionRay) && InfiniteObjects[k]->t < t1)
				{
					smallestK = k;
					hitSomething = true;
					hitInfinite = true;
					t1 = InfiniteObjects[k]->t;
				}
			}

			if (hitSomething)
			{
				if (!hitInfinite)
				{
					const Vector3 tempColor = objects.closest()->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

					_temp_baseColor.x += static_cast<float>(tempColor.z);
					_temp_baseColor.y += static_cast<float>(tempColor.y);
					_temp_baseColor.z += static_cast<float>(tempColor.x);
				}
				else
				{
					const Vector3 tempColor = InfiniteObjects[smallestK]->getMirrorColor(ExitRefractionRay, lights, numberOfLights, objects, InfiniteObjects, currentReflectionDepth);

					_temp_baseColor.x += static_cast<float>(tempColor.z);
					_temp_baseColor.y += static_cast<float>(tempColor.y);
					_temp_baseColor.z += static_cast<float>(tempColor.x);
				}
			}
			else
			{
				const color bgColor(0x32, 0x4b, 0x77);

				_temp_baseColor.x += static_cast<float>(bgColor.R);
				_temp_baseColor.y += static_cast<float>(bgColor.G);
				_temp_baseColor.z += static_cast<float>(bgColor.B);
			}

			hitPos = temp_Hitpos;
			break;
		}

		default:
		{
			color kd1 = getBaseColor();

			_temp_baseColor.x += static_cast<float>(kd1.R);
			_temp_baseColor.y += static_cast<float>(kd1.G);
			_temp_baseColor.z += static_cast<float>(kd1.B);

			temp_ambientColor.x = (static_cast<float>(OriginalAmbientColor.R));
			temp_ambientColor.y = (static_cast<float>(OriginalAmbientColor.G));
			temp_ambientColor.z = (static_cast<float>(OriginalAmbientColor.B));
		}

		break;
		}

		for (int i = 0; i < numberOfLights; i++)
		{

			Vector3 shadowDirection = *lights[i]->pos - hitPos + getNormal() * 1e-4f;
			const Vector3 shadowOrigin = hitPos + getNormal() * 1e-4f;

			const float distanceToLight = shadowDirection.Magnitude();
			shadowDirection.Normalize();

			const Ray shadowRay(shadowDirection, shadowOrigin);


			bool hits = objects.checkHit(shadowRay);

			if (hits && objects.closest()->t >= distanceToLight)
			{
				hits = false;
			}

			for (int k = 0; k < InfiniteObjects.getSize(); k++)
			{
				if (InfiniteObjects[k]->HitShadow(shadowRay, distanceToLight) && InfiniteObjects[k]->chosenPattern != Patterns::refractor)
				{
					hits = true;
					break;
				}
			}

			if (!hits)
			{
				Vector3 l = *lights[i]->pos - hitPos;
				l.Normalize();

				Vector3 h = -1 * (*ray.Direction - l);
				h.Normalize();

				float nl = getNormal() * l;
				float nh = getNormal() * h;

				temp_Color.x += static_cast<float>(_temp_baseColor.x)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.R)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.y += static_cast<float>(_temp_baseColor.y)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.G)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.z += static_cast<float>(_temp_baseColor.z)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.B)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);

			}
		}

		temp_Color.x += temp_ambientColor.x * ambientIntensity;
		temp_Color.y += temp_ambientColor.y * ambientIntensity;
		temp_Color.z += temp_ambientColor.z * ambientIntensity;


		if (temp_Color.x > 0xff)
		{
			temp_Color.x = 0xff;
		}

		if (temp_Color.y > 0xff)
		{
			temp_Color.y = 0xff;
		}

		if (temp_Color.z > 0xff)
		{
			temp_Color.z = 0xff;
		}

		return temp_Color;
	}
	else
	{
		color kd1 = getBaseColor();

		_temp_baseColor.x += static_cast<float>(kd1.R);
		_temp_baseColor.y += static_cast<float>(kd1.G);
		_temp_baseColor.z += static_cast<float>(kd1.B);

		const float otherReflect = 1 - reflectiveness;

		temp_ambientColor.x = (static_cast<float>(OriginalAmbientColor.R)* otherReflect);
		temp_ambientColor.y = (static_cast<float>(OriginalAmbientColor.G)* otherReflect);
		temp_ambientColor.z = (static_cast<float>(OriginalAmbientColor.B)* otherReflect);

		for (int i = 0; i < numberOfLights; i++)
		{

			Vector3 shadowDirection = *lights[i]->pos - hitPos + getNormal() * 1e-4f;
			const Vector3 shadowOrigin = hitPos + getNormal() * 1e-4f;

			const float distanceToLight = shadowDirection.Magnitude();
			shadowDirection.Normalize();

			const Ray shadowRay(shadowDirection, shadowOrigin);


			bool hits = objects.checkHit(shadowRay);

			if (hits && objects.closest()->t >= distanceToLight)
			{
				hits = false;
			}

			if (!hits)
			{
				Vector3 l = *lights[i]->pos - hitPos;
				l.Normalize();

				Vector3 h = -1 * (*ray.Direction - l);
				h.Normalize();

				float nl = getNormal() * l;
				float nh = getNormal() * h;

				temp_Color.x += static_cast<float>(_temp_baseColor.x)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.R)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.y += static_cast<float>(_temp_baseColor.y)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.G)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);
				temp_Color.z += static_cast<float>(_temp_baseColor.z)* lights[i]->lightIntensity* std::max(0.0f, nl) + static_cast<float>(SpecularColor.B)* lights[i]->lightIntensity* pow(std::max(0.0f, nh), Phong);

			}
		}

		temp_Color.x += temp_ambientColor.x * ambientIntensity;
		temp_Color.y += temp_ambientColor.y * ambientIntensity;
		temp_Color.z += temp_ambientColor.z * ambientIntensity;


		if (temp_Color.x > 0xff)
		{
			temp_Color.x = 0xff;
		}

		if (temp_Color.y > 0xff)
		{
			temp_Color.y = 0xff;
		}

		if (temp_Color.z > 0xff)
		{
			temp_Color.z = 0xff;
		}

		return temp_Color;
	}
}

void Surface::setPattern(Patterns pattern)
{
	chosenPattern = pattern;
}

void Surface::setColor2(uint8_t R, uint8_t G, uint8_t B)
{
	Color2.R = R;
	Color2.G = G;
	Color2.B = B;
}

void Surface::setNormal(float, float, float)
{
	printf("[WARNING!] You are using a set normal for a object that should not be able to do that...\n");
}
