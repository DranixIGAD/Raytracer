#pragma once

#include "../myMath/Vector3.h"

class Camera
{
public:

	Camera()
		: pos(new Vector3(0))
		, TL(nullptr)
		, BR(nullptr)
		, D(NULL)
	{
	};
	
	Camera(float x, float y, float z)
		: pos(new Vector3(x, y, z))
		, TL(nullptr)
		, BR(nullptr)
		, D(NULL)
	{
	};
	
	~Camera();

	Vector3* pos;
	//Vector3* u;
	//Vector3* v;
	//Vector3* w;
	Vector3* TL;
	Vector3* BR;

	void setRotationVW(float degrees);
	void setRotationUV(float degrees);
	void setRotationUW(float degrees);

	void createViewplane(float d, float width, float height);
	float D;
};

