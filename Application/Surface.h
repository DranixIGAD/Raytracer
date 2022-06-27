#pragma once
#include "../myMath/Vector3.h"
#include "Color.h"
#include "LightSource.h"
#include "Ray.h"
#include "../myMath/Matrix44.h"
#include "myContainer.h"
#include "BVH.h"

class Surface
{
public:

	Surface();
	//virtual ~Hittable() = 0;

	virtual bool Hit(const Ray& ray);
	virtual bool HitShadow(const Ray& ray, const float& distanceToLight);
	float t; //needed here for bvh

	color getColor(const Ray& ray, LightSource* lights[], int numberOfLights, myContainer<Surface*>& objects);
	color getBaseColor();
	Vector3 getMirrorColor(const Ray& ray, LightSource* lights[], int numberOfLights, myContainer<Surface*>& objects, int& current_reflection_depth);
	void setColor(uint8_t R, uint8_t G, uint8_t B);
	void setSpecularColor(uint8_t R, uint8_t G, uint8_t B);
	void setAmbientColor(uint8_t R, uint8_t G, uint8_t B);
	void setPhong(float value);
	void setAmbientIntensity(float value);
	void setReflectiveness(float value);
	void setIor(float value);
	virtual Vector3 getNormal();
	virtual Vector3 get2ndRefractPoint(const Ray& ray);
	virtual void set_rotation_x(float angle);
	virtual void set_rotation_y(float angle);
	virtual void set_rotation_z(float angle);
	virtual void set_translation(float x_trans, float y_trans, float z_trans);
	void redo_BB(Matrix44 masterTransform);
	
	//hate this, need make time to fix needing this for just able to use infinite objects like planes
	color getColor(const Ray& ray, LightSource* lights[], int numberOfLights, myContainer<Surface*>& objects, myContainer<Surface*>& InfiniteObjects);
	Vector3 getMirrorColor(const Ray& ray, LightSource* lights[], int numberOfLights, myContainer<Surface*>& objects, myContainer<Surface*>& InfiniteObjects, int& current_reflection_depth);

	color getColor(const Ray& ray, LightSource* lights[], int numberOfLights, BVH<Surface>& objects, myContainer<Surface*>& InfiniteObjects);
	Vector3 getMirrorColor(const Ray& ray, LightSource* lights[], int numberOfLights, BVH<Surface>& objects, myContainer<Surface*>& InfiniteObjects, int& current_reflection_depth);

	
	color Color;
	color SpecularColor;
	color AmbientColor;
	color OriginalAmbientColor;

	float Phong;
	float ambientIntensity;
	float reflectiveness;
	float ior;

	color RealColor;
	Vector3 hitPos;

	enum class Patterns
	{
		plain,
		checkered,
		mirror,
		refractor,
	};
	Patterns chosenPattern;
	void setPattern(Patterns pattern);
	void setColor2(uint8_t R, uint8_t G, uint8_t B);
	color Color2;

	virtual void setNormal(float x, float y, float z);


	float tempR, tempG, tempB;
	int reflectionDepth, currentReflectionDepth;

	// locations of bounding box max and min points
	Vector3 minPointBB;
	Vector3 maxPointBB;
	Vector3 minPointOG;
	Vector3 maxPointOG;
};