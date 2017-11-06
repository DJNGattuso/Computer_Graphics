//Nicholas Gattuso 40007087
//referenced to and modified from-https://github.com/wishedeom/COMP371_A3/tree/master/COMP371_A3

#include <iostream>
#include <string>
#include <fstream>
#include "sceneLoader.h"
#include "CImg.h"
#include "Camera.h"
#include "Sphere.h"
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
	Sphere sphere2(glm::vec3{ 0,3,-40 }, 3, glm::vec3{0.3, 0.15, 0.2}, glm::vec3{ 0.1, 0.22, 0.29 }, glm::vec3{ 0.2, 0.7, 0.2 }, 1);


	//NEED TO DO
	//Go through each pixel in the image -> make a loop throughout the image, for every pixel do the following:
	//calculate the pixel and its ray direction using camera.rayPixel
	//toss the ray using raytray.getPoint
	//check if there's an intersection
	//set the colour of the pixel

	//-------------------------------------------------Save image----------------------------------------------------------
	//Creates an image with three channels and sets it to black
	cimg_library::CImg<float> image(camera.getWidth(), camera.getHeight(), 1, 3, 0);
	
	//need to compute the colours

	//Save out the image in BMP format. Pixel values must be in the range [0,255]
	image.save("render.bmp");

	//Display the rendered image on screen
	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed())
		main_disp.wait();

	return 0;
}