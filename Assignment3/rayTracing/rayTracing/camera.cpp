
#include "Camera.h"
#include <iostream>
#include <string>
#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

int Camera::pixelWidth = 800;
int Camera::pixelHeight = 800;

Camera::Camera(glm::vec3 pos, int FOV, int F, double A)
{
	position = pos;
	fov = FOV;
	focal = F;
	aspectRatio = A;

	setImage();
}

void Camera::setImage()
{
	height = focal * std::atan(fov / 2);
	width = height * aspectRatio;
}


//--------------------------------------Getter functions--------------------------------
glm::vec3 Camera::getPosition()
{
	return position;
}

float Camera::getFOV()
{
	return fov;
}

float Camera::getFocal()
{
	return focal;
}

float Camera::getAspect() {
	return aspectRatio;
}

float Camera::getWidth() {
	return width;
}

float Camera::getHeight() {
	return height;
}