#include "Renderer.h"

#include "Color.h"
#include "Camera.h"
#include "Surface.h"
#include "Sphere.h"
#include "Plane.h"
#include "LightSource.h"
#include "Ray.h"
#include "Box.h"
#include "../myMath/Matrix44.h"
#include "BVH.h"
#include "myContainer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

int main()
{
	printf("I have started running...\n");
	
	printf("Defining image size...\n");
	const int screenWidth  = 3840 / 2;
	const int screenHeight = 2160 / 2;

	const int totalPixels = screenHeight * screenWidth;

	printf("Making empty pixels for %i pixels and filling them with the background color...\n", totalPixels);
	color* image = new color[totalPixels];
	const color bgColor(0x32, 0x4b, 0x77);
	for (int i = 0; i < totalPixels; i++)
	{
		image[i] = bgColor;
	}
	
	printf("Making a camera...\n");
	Camera* camera = new Camera(0.0f, 7.0f, 1.0f);		// cornell box camera

	printf("Making a view plane...\n");
	 
	float angle = 90.0f;
	float d = screenHeight / tanf(angle / 2);
	camera->createViewplane(d, screenWidth, screenHeight);
	
	const Vector3 look_at_point(0.0f, 0.0f, -9.0f);
	Matrix44 lookAt(*camera->pos, look_at_point);		// cornell box look at



	// Finite Objects ---------------------------------
	// Objects[Objects.Last()]->

	myContainer<Surface*> Objects;

	const int numberOfObjects = 50; // times itself, currently 2500 // 50 * 50 == 2500
	
	for (int i = 0; i < numberOfObjects; i++)
	{
		for (int j = 0; j < numberOfObjects; j++)
		{
			if ((i < 24 || i > 26) || (j > 48 || j < 45))
			{
				const float width = static_cast<float>(rand() % 3 + 1);
				const float height = static_cast<float>(rand() % 5 + 1);
				const float depth = static_cast<float>(rand() % 3 + 1);

				Objects.push(new Box(width, height, depth));

				const float x = static_cast<float>(i) * 2.5f - numberOfObjects /2.0f * 2.5f;
				const float z = static_cast<float>(j) * 2.5f - numberOfObjects * 2.5f;

				Objects[Objects.Last()]->set_translation(x, 0, z);

				const float rotX = static_cast<float>(rand() % 2 - 1);
				const float rotZ = static_cast<float>(rand() % 4 - 2);
				const float rotY = static_cast<float>(rand() % 6 - 3);
				
				Objects[Objects.Last()]->set_rotation_x(rotX);
				Objects[Objects.Last()]->set_rotation_z(rotZ);
				Objects[Objects.Last()]->set_rotation_y(rotY);

				const uint8_t R = static_cast<uint8_t>(rand() % 235 + 20);
				const uint8_t G = static_cast<uint8_t>(rand() % 235 + 20);
				const uint8_t B = static_cast<uint8_t>(rand() % 235 + 20);

				Objects[Objects.Last()]->setColor(R, G, B);
			}
		}
	}

	Objects.push(new Sphere(0.0f, 1.5f, -9.0f, 1.5f));
	Objects[Objects.Last()]->setColor(0xff, 0xff, 0xff);
	Objects[Objects.Last()]->setPhong(1000);
	Objects[Objects.Last()]->setPattern(Surface::Patterns::mirror);
	Objects[Objects.Last()]->setReflectiveness(0.5f);


	// infinite Objects -----------------------
	// InfiniteObjects[InfiniteObjects.Last()]->

	myContainer<Surface*> InfiniteObjects;

	InfiniteObjects.push(new Plane());												// bottom plane
	InfiniteObjects[InfiniteObjects.Last()]->setColor(0xf1, 0xf1, 0xf1);
	
	// BVH ------------------------------------
	printf("Building BVH... \n");

	BVH<Surface> Hierarchy(Objects);
	
	printf("Successfully build BVH... \n");
	// lights ---------------------------------

	const int numberOfLights = 1;
	LightSource* Lights[numberOfLights];

	printf("Making %i lights...\n", numberOfLights);

	int currentLight = 0;

	Lights[currentLight] = new LightSource(-50.0f, 60.0f, 100.0f);
	Lights[currentLight]->setLightIntensity(0.8f);

	// ---------------------------------
	
	printf("Starting pixel calculations...\n");
	int currentPixel = 0;
	for (int i = 0; i < screenHeight; i++)
	{
		for (int j = 0; j < screenWidth; j++)
		{			
			const float u = camera->TL->x + ((camera->BR->x - camera->TL->x) * (static_cast<float>(j) + 0.5f)) / screenWidth;
			const float v = camera->BR->y + ((camera->TL->y - camera->BR->y) * (static_cast<float>(i) + 0.5f)) / screenHeight;

			Vector3 viewRayDirection(u, -v, -camera->D);

			viewRayDirection = lookAt.times(viewRayDirection, 0);
			viewRayDirection.Normalize();

			Ray viewRay(viewRayDirection, *camera->pos);

			float t = INFINITY;
			
			bool viewRayHit = Hierarchy.checkHit(viewRay);
			
			if (viewRayHit)
			{
				t = Hierarchy.closest()->t;
				image[currentPixel] = Hierarchy.closest()->getColor(viewRay, Lights, numberOfLights, Hierarchy, InfiniteObjects);
			}

			if (InfiniteObjects.getSize() > 0)
			{
				int closestHittable = NULL;	// defined here so I can use it in line 253

				viewRayHit = false;
				for (int k = 0; k < InfiniteObjects.getSize(); k++)
				{
					if (InfiniteObjects[k]->Hit(viewRay) && InfiniteObjects[k]->t < t)
					{
						closestHittable = k;	// if I would have defined it here it would not be accessible in line 253 because of scope
						viewRayHit = true;
						t = InfiniteObjects[k]->t;
					}
				}

				if (viewRayHit)
				{
					image[currentPixel] = InfiniteObjects[closestHittable]->getColor(viewRay, Lights, numberOfLights, Hierarchy, InfiniteObjects); // this is line 253
				}
			}
			
			currentPixel++;
		}

		if (i % 100 == 0)
		{
			const float progress = (static_cast<float>(i) / screenHeight) * 100;
			printf("%f %%\n", progress);
		}
	}
	printf("\n100 %% !\n\n");
	printf("calling IMAGE.bmp creation...\n");
	stbi_write_bmp("IMAGE.bmp", screenWidth, screenHeight, 3, image);


	printf("[ending reached] I have stopped running\n\n");
}