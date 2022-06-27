#include "LightSource.h"
#include <iostream>

LightSource::LightSource()
{
	pos = new Vector3(0);
	lightIntensity = 1;
}

LightSource::LightSource(float x, float y, float z)
{
	pos = new Vector3(x, y, z);
	lightIntensity = 1;
}

LightSource::~LightSource()
{
	delete pos;
}

void LightSource::setLightIntensity(float value)
{
	if (value < 0)
	{
		printf("[WARNING!] value for lightIntensity was negative and has been set to 0.1\n");
		lightIntensity = 0.1f;
	}
	else 
	{
		lightIntensity = value;
	}
}
