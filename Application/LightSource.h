#pragma once
#include "../myMath/Vector3.h"

class LightSource
{
public:
	LightSource();
	LightSource(float x, float y, float z);
	~LightSource();

	Vector3* pos;

	void setLightIntensity(float value);
	float lightIntensity;
	
};

