//Nicholas Gattuso 40007087
//referenced to-https://github.com/wishedeom/COMP371_A3/tree/master/COMP371_A3
//and from-https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
//and from-https://www.ics.uci.edu/~gopi/CS211B/RayTracing%20tutorial.pdf

#include <iostream>
#include <vector>
#include <math.h>
#include "CImg.h"
#include "sceneLoader.h"
#include "Camera.h"
#include "Sphere.h"
#include "Raytracing.h"
#include "Triangles.h"
#include "Plane.h"
#include "Light.h"

using namespace std;
Camera camera;
std::vector<Sphere> sphereObjects;
std::vector<Triangle> triangleObjects;
Plane plane;
std::vector<Light> lightObjects;

void build1()
{
	//-----------------------------------------Create camera object and image space-------------------------------------------------
	Camera cam(glm::vec3{ 0,0,0 }, 60, 1000, 1.33);
	camera = cam;

	//----------------------------------------------Create Sphere Object------------------------------------------------------------
	Sphere sphere1(glm::vec3{ 0,6,-40 }, 2, glm::vec3{ 0.1,0.5,0.5 }, glm::vec3{ 0.4, 0.6, 0.2 }, glm::vec3{ 0.2, 0.5, 0.5 }, 1);
	Sphere sphere2(glm::vec3{ 0,3,-40 }, 3, glm::vec3{ 0.3, 0.15, 0.2 }, glm::vec3{ 0.1, 0.22, 0.29 }, glm::vec3{ 0.2, 0.7, 0.2 }, 1);
	Sphere sphere3(glm::vec3{ 0, -3, -40 }, 5, glm::vec3{ 0.1, 0.15, 0.7 }, glm::vec3{ 0.8, 0.22, 0.29 }, glm::vec3{ 0.2, 0.7, 0.8 }, 1);

	//place sphere objects in a vector
	sphereObjects.emplace_back(sphere1);
	sphereObjects.emplace_back(sphere2);
	sphereObjects.emplace_back(sphere3);

	bool sphereIntersect = false;

	//----------------------------------------------Create Triangle Object------------------------------------------------------------
	Triangle triangle1(glm::vec3{ 1,7,-40 }, glm::vec3{ 1,5,-40 }, glm::vec3{ 5,6,-40 }, glm::vec3{ 0.5,0.2,0.7 }, glm::vec3{ 0.2, 0.4, 0.2 }, glm::vec3{ 0.1, 0.1, 0.2 }, 0.5);

	//place sphere objects in a vector
	triangleObjects.emplace_back(triangle1);

	bool triIntersect = false;

	//---------------------------------------------------Creating Plane Object----------------------------------------------
	Plane pla(glm::vec3{ 0,1,0 }, glm::vec3{ 0,-5,0 }, glm::vec3{ 0.8,0.8,0.8 }, glm::vec3{ 0.1,0.1,0.1 },
		glm::vec3{ 0.7,0.7,0.7 }, 6);
	bool planeInter = false;

	plane = pla;

	//----------------------------------------------Creating the Light Object---------------------------------------------
	Light light(glm::vec3{ 15, 12, -3 }, glm::vec3{ 0.3, 0.9, 0.9 });

	//place light in vector
	lightObjects.emplace_back(light);
}
void build4()
{
	//-----------------------------------------Create camera object and image space-------------------------------------------------
	Camera cam(glm::vec3{ 0,0,0 }, 60, 700, 1.5);
	camera = cam;

	//----------------------------------------------Create Triangle Object------------------------------------------------------------
	Triangle triangle1(glm::vec3{ -15,20,-35 }, glm::vec3{ -15,0,-35 }, glm::vec3{ 10,10,-20 }, glm::vec3{ 0.1,0.1,0.1 }, glm::vec3{ 0.2, 0.6, 0.4 }, glm::vec3{ 0.2, 0.1, 0.4 }, 4);
	Triangle triangle2(glm::vec3{ -10,10,-25 }, glm::vec3{ 15,0,-35 }, glm::vec3{ 15,20,-35 }, glm::vec3{ 0.1,0.1,0.1 }, glm::vec3{ 0.2, 0.15, 0.4 }, glm::vec3{ 0.2, 0.1, 0.4 }, 4);

	
	//place sphere objects in a vector
	triangleObjects.emplace_back(triangle1);
	triangleObjects.emplace_back(triangle2);


	//---------------------------------------------------Creating Plane Object----------------------------------------------
	Plane pla(glm::vec3{ 0,1,0 }, glm::vec3{ 0,-5,0 }, glm::vec3{ 0.2,0.1,0.7 }, glm::vec3{ 0.3,0.2,0.3 },
		glm::vec3{ 0.5,0.2,0.2 }, 6);

	plane = pla;

	//----------------------------------------------Creating the Light Object---------------------------------------------
	Light light(glm::vec3{ 0, 10, -15 }, glm::vec3{ 0.9, 0.9, 0.9 });

	//place light in vector
	lightObjects.emplace_back(light);
}
int main()
{
	cout << "main program for ray tracing" << endl;
	//for testing purpose

	//------------------------------------Read text file and get the object variables----------------------------------------------
	Scene loadtext("scene.txt");

	if (loadtext.getLoad()) { cout << "read succesfully" << endl; }
	else { cout << "Read failed" << endl; }

	//must add fetching of variables from scene instead of hardcoding it
	//build1();
	build4();

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
	

	//------------------------------------Send rays, calculate intersections, get pixel colour---------------------------------
	int intersectedObject = 0; glm::vec3 nearestPoint{ 0,0,0 }; bool shadowIntersect = false; float b = (1e-3);
	bool sphereIntersect = false; bool triIntersect = false; bool planeInter = false;

	for (int h = 0; h <= imageHeight - 1; h++) //loop through every height column
	{
		for (int w = 0; w <= imageWidth - 1; w++) //loop through every width row
		{
			//reset boolean values for every pixel
			sphereIntersect = false;
			triIntersect = false;
			planeInter = false;
			shadowIntersect = false;
			
			//reset intersectedObject
			intersectedObject = 0;

			//get Camera space
			float pX = (2 * ((w + 0.5) / imageWidth) - 1) * tan((camera.getFOV()) / 2 * 3.14159 / 180) * aspectRatio;
			float pY = (1 - 2 * ((h + 0.5) / imageHeight)) * tan((camera.getFOV()) / 2 * 3.14159 / 180);
			glm::vec3 rayDirection = glm::vec3{ pX, pY, -1 } - camera.getPosition();
			rayDirection = glm::normalize(rayDirection);

			glm::vec3 colour{ 0.0, 0.0, 0.0 };

			//-------toss ray and check for intersection-----
			//for spheres
			float nearestSphere = 1000000; int nearestSphereIndex;
			if (sphereObjects.size() != 0) {
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
			}

			//for triangles
			float nearestTri = 1000000; int nearestTriIndex;
			for (int i = 0; i <= triangleObjects.size() - 1; i++)
			{
				if (triangleObjects[i].triInter(camera.getPosition(), rayDirection)) //if it intersects
				{
					triIntersect = true;
					if (triangleObjects[i].getInterDis() <= nearestTri)
					{
						nearestTri = triangleObjects[i].getInterDis();
						nearestTriIndex = i;
					}
				}
			}

			//for plane
			planeInter = plane.planeInter(plane.getNOR(), plane.getPosition(), camera.getPosition(), rayDirection);

			//------------------------------------------Check for nearest----------------------------------------
			//----------if all three objects are intersected------------------
			if (triIntersect && sphereIntersect && planeInter)
			{
				if (nearestTri <= nearestSphere) //case if sphere is nearest
				{
					if (nearestSphere <= plane.getInterDis()) { intersectedObject = 1; nearestPoint = sphereObjects[nearestSphereIndex].getNearest(); }
				}
				else if (nearestSphere <= nearestTri) //case if triangle is nearest
				{
					if (nearestTri <= plane.getInterDis()) { intersectedObject = 2; nearestPoint = triangleObjects[nearestTriIndex].getNearest();}
				}
				else { intersectedObject = 3;}
			}
			//-------if only tri and sphere are intersected------------
			else if (triIntersect && sphereIntersect)
			{
				if (nearestSphere <= nearestTri) //case if sphere is nearest
				{
					intersectedObject = 1; nearestPoint = sphereObjects[nearestSphereIndex].getNearest();
				}
				else { intersectedObject = 2; nearestPoint = triangleObjects[nearestTriIndex].getNearest();}
			}
			//-------if only tri and plane are intersected-------------
			else if (triIntersect && planeInter)
			{
				if (plane.getInterDis() <= nearestTri) //case if plane is nearest
				{
					intersectedObject = 3;
				}
				else { intersectedObject = 2; nearestPoint = triangleObjects[nearestTriIndex].getNearest();}
			}
			//-------if only sphere and plane are intersected---------
			else if (sphereIntersect && planeInter)
			{
				if (plane.getInterDis() <= nearestSphere) //case if plane is nearest
				{
					intersectedObject = 3;
				}
				else { intersectedObject = 1; nearestPoint = sphereObjects[nearestSphereIndex].getNearest();}
			}
			//---------only 1 intersection occured-------
			else if (triIntersect) { intersectedObject = 2; nearestPoint = triangleObjects[nearestTriIndex].getNearest();}
			else if (sphereIntersect) { intersectedObject = 1; nearestPoint = sphereObjects[nearestSphereIndex].getNearest();}
			else if (planeInter) { intersectedObject = 3; }
			//------------no intersections------------
			else {colour = { 0.0, 0.0, 0.0 };}

			//-------------------------------Set the shadow and lights to determnine the colour------------------------------
			for (int i = 0; i <= lightObjects.size() - 1; i++)
			{
				shadowIntersect = false;
				//--------------check if theres a shadow----------------
				if (intersectedObject != 0) 
				{
					glm::vec3 lightRayDirection = glm::normalize(lightObjects[i].getPosition() - nearestPoint);

					//check if spheres
					if (sphereObjects.size() != 0) {
						for (int i = 0; i <= sphereObjects.size() - 1; i++)
						{
							if (sphereObjects[i].sphereInter(nearestPoint + (b*lightRayDirection), lightRayDirection)) {
								shadowIntersect = true;
							}
						}
					}

					//check for triangles
					for (int i = 0; i <= triangleObjects.size() -1 ; i++) 
					{
						if (triangleObjects[i].triInter(nearestPoint + (0.0001f*lightRayDirection), lightRayDirection)) {
							shadowIntersect = true;
						}
					}
				}

				//------calculate colour of pixel based on the light and shadow---------
				if (sphereIntersect || triIntersect || planeInter) //an intersection occured
				{
					if (intersectedObject == 1) //sphere
					{
						colour += sphereObjects[nearestSphereIndex].getAmbient();
						if (!shadowIntersect)
						{
							colour += sphereObjects[nearestSphereIndex].sphereLight(lightObjects[i].getPosition(), rayDirection, lightObjects[i].getColour());
						}
						
					}
					else if (intersectedObject == 2) //triangle
					{
						colour += triangleObjects[nearestTriIndex].getAmbient();
						if (!shadowIntersect) {
							colour += triangleObjects[nearestTriIndex].triLight(lightObjects[i].getPosition(), rayDirection, lightObjects[i].getColour());
						}
					}
					else if (intersectedObject == 3) //plane
					{
						colour += plane.getAmbient();
						if (!shadowIntersect) {
							colour += plane.planeLight(lightObjects[i].getPosition(), rayDirection, lightObjects[i].getColour());
						}
					}
				}
			}
			
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