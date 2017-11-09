//Nicholas Gattuso 40007087
//referenced to-https://github.com/wishedeom/COMP371_A3/tree/master/COMP371_A3
//and from-https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection


#include <iostream>
#include <vector>
#include <math.h>
#include "CImg.h"
#include "sceneLoader.h"
#include "Camera.h"
#include "Sphere.h"
#include "Raytracing.h"
#include "Triangles.h"

using namespace std;

int main()
{
	cout << "main program for ray tracing" << endl;
	//for testing purpose

	//------------------------------------Read text file and get the object variables----------------------------------------------
	Scene loadtext("scene.txt");

	if (loadtext.getLoad()) { cout << "read succesfully" << endl; }
	else { cout << "Read failed" << endl; }

	//must add fetching of variables from scene instead of hardcoding it
	//-----------------------------------------Create camera object and image space-------------------------------------------------
	Camera camera(glm::vec3{ 0,0,0 }, 60, 1000, 1.33);

	//----------------------------------------------Create Sphere Object------------------------------------------------------------
	Sphere sphere1(glm::vec3{ 0,6,-40 }, 2, glm::vec3{ 0.1,0.5,0.5 }, glm::vec3{ 0.4, 0.6, 0.2 }, glm::vec3{ 0.2, 0.5, 0.5 }, 1);
	Sphere sphere2(glm::vec3{ 0,3,-40 }, 3, glm::vec3{ 0.3, 0.15, 0.2 }, glm::vec3{ 0.1, 0.22, 0.29 }, glm::vec3{ 0.2, 0.7, 0.2 }, 1);
	Sphere sphere3(glm::vec3{ 0, -3, -40 }, 5, glm::vec3{ 0.1, 0.15, 0.7 }, glm::vec3{ 0.8, 0.22, 0.29 }, glm::vec3{ 0.2, 0.7, 0.8 }, 1);
	
	//place sphere objects in a vector
	std::vector<Sphere> sphereObjects;
	sphereObjects.emplace_back(sphere1);
	sphereObjects.emplace_back(sphere2);
	sphereObjects.emplace_back(sphere3);

	bool sphereIntersect = false;

	//----------------------------------------------Create Triangle Object------------------------------------------------------------
	Triangle triangle1(glm::vec3{ 1,7,-40 }, glm::vec3{ 1,5,-40 }, glm::vec3{ 5,6,-40 }, glm::vec3{ 0.5,0.2,0.7 }, glm::vec3{ 0.2, 0.4, 0.2 }, glm::vec3{ 0.1, 0.1, 0.2 }, 0.5);
	
	//place sphere objects in a vector
	std::vector<Triangle> triangleObjects;
	triangleObjects.emplace_back(triangle1);

	bool triIntersect = false;

	//NEED TO DO
	//Add the other objects (triangles, objs, plane, light)
	//Fix the scene to get the objects from the scene instead

	//--------------------------------------------------Define the Image-------------------------------------------------------
	//Creates an image with three channels and sets it to black
	cimg_library::CImg<float> image(camera.getWidth(), camera.getHeight(), 1, 3, 0);


	//NEED TO DO
	//Go through each pixel in the image -> make a loop throughout the image, for every pixel do the following:
	float imageWidth = camera.getWidth(); float imageHeight = camera.getHeight();
	float aspectRatio = imageWidth / imageHeight;
	glm::vec3 camPos = camera.getPosition();
	
	for (int h = 0; h <= imageHeight - 1; h++) //loop through every height column
	{
		for (int w = 0; w <= imageWidth - 1; w++) //loop through every width row
		{
			sphereIntersect = false;
			triIntersect = false;

			//get Camera space
			float pX = (2 * ((w + 0.5) / imageWidth) - 1) * tan((camera.getFOV()) / 2 * 3.14159 / 180) * aspectRatio;
			float pY = (1 - 2 * ((h + 0.5) / imageHeight)) * tan((camera.getFOV()) / 2 * 3.14159 / 180);
			glm::vec3 rayDirection = glm::vec3{ pX, pY, -1 } - camera.getPosition();
			rayDirection = glm::normalize(rayDirection);

			//get a ray
			//Raytray ray = camera.rayPixel(w, h); //ray receives the point (w,h) and the direction

			//calculate distance to know how far to check
			//glm::vec2 distance{(w - camPos.x), (h - camPos.y)};
			//float dista = sqrt((distance.x)*(distance.x) + (distance.y)*(distance.y));

			glm::vec3 colour{ 1.0, 1.0, 1.0 };
			//-------toss ray and check for intersection-----

			//for spheres
			float nearestSphere = 1000000; int nearestSphereIndex;
			for (int i = 0; i <= sphereObjects.size() - 1; i++)
			{
				if (sphereObjects[i].sphereInter(camera.getPosition(), rayDirection)) //if it intersects
				{
					sphereIntersect = true;
					if (sphereObjects[i].getInterDis() <= nearestSphere)
					{
						nearestSphere = sphereObjects[i].getInterDis();
						nearestSphereIndex = i;
					}
				}
			}

			float nearestTri = 1000000; int nearestTriIndex;
			for (int i = 0; i <= triangleObjects.size() - 1; i++)
			{
				if (triangleObjects[i].triInter(camera.getPosition(), rayDirection, triangleObjects[i].getVertex1(), 
					triangleObjects[i].getVertex2(), triangleObjects[i].getVertex3())) //if it intersects
				{
					triIntersect = true;
					if (triangleObjects[i].getInterDis() <= nearestTri)
					{
						nearestTri = triangleObjects[i].getInterDis();
						nearestTriIndex = i;
					}
				}
			}

			if (triIntersect && sphereIntersect)
			{
				if (nearestSphere <= nearestTri){ colour = sphereObjects[nearestSphereIndex].getAmbient(); }
				else { colour = triangleObjects[nearestTriIndex].getAmbient(); }
			}
			else if (triIntersect){ colour = triangleObjects[nearestTriIndex].getAmbient(); }
			else if (sphereIntersect) { colour = sphereObjects[nearestSphereIndex].getAmbient(); }

			//Store the colour of the pixel
			float color[3]{ colour.x, colour.y, colour.z };
			image.draw_point(w, h, color);
		}
	}


	//-------------------------------------------------Save image---------------------------------------------------------
	//Save out the image in BMP format. Pixel values must be in the range [0,255]
	image.normalize(0, 255);
	image.save("render.bmp");

	//Display the rendered image on screen
	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed())
		main_disp.wait();

	return 0;
}