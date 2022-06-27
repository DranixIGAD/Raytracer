#include "Camera.h"
#include <iostream>

Camera::~Camera()
{
	delete pos;
	//delete u,v,w;
	delete TL;
	delete BR;
}

void Camera::setRotationVW(float /*degrees*/)
{
	//// changing V
	//v.z += sin(degrees * (pi / 180.0f));
	//v.y = v.z - cos(degrees * (pi / 180.0f));

	//// changing W
	//w.y -= sin(degrees * (pi / 180.0f));
	//w.z = w.z - cos(degrees * (pi / 180.0f));
}

void Camera::setRotationUV(float /*degrees*/)
{
}

void Camera::setRotationUW(float /*degrees*/)
{
}

void Camera::createViewplane(float d, float width, float height)
{
	float halfWidth = width / 2;
	if (static_cast<int>(width) % 2 == 1)
	{
		printf("View plane width is not nicely divided by 2, problems could exits...\n");
	}

	float halfHeight = height / 2;
	if (static_cast<int>(height) % 2 == 1)
	{
		printf("View plane height is not nicely divided by 2, problems could exits...\n");
	}

	TL = new Vector3(pos->x - halfWidth, pos->y + halfHeight, pos->z - d);
	BR = new Vector3(pos->x + halfWidth, pos->y - halfHeight, pos->z - d);
	D = d;
}
