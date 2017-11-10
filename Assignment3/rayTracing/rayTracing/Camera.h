#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>
#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>
#include "Raytracing.h"

class Camera
{
public:
	//pixel space
	static int pixelWidth;
	static int pixelHeight;

	//constructor
	Camera();
	Camera(glm::vec3 pos, int FOV, int F, double A);

	//getters
	glm::vec3 getPosition();
	float getFOV();
	float getFocal();
	float getAspect();
	float getWidth();
	float getHeight();

	//pixel operations for raytracing
	glm::vec3 getPixel(int i, int j);
	Raytray rayPixel(int i,  int j);

private:
	//Camera variables
	glm::vec3 position;
	float fov;
	float focal;
	float aspectRatio;

	//image space
	void setImage();
	float width;
	float height;
	glm::vec2 getImagePixel(const int i, const int j);
};
#endif