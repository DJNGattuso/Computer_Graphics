#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>
#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Camera
{
public:
	Camera(glm::vec3 pos, int FOV, int F, double A)
	{
		position = pos;
		fov = FOV;
		f = F;
		a = A;
	}

private:
	glm::vec3 position;
	int fov;
	int f;
	double a;

};
#endif