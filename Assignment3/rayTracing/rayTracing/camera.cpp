
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

//---------------Calculate pixel position and toss a ray through the pixel----------------
//create a ray that passes through a pixel
Raytray Camera::rayPixel(int i, int j)
{
	//auto pixel = getPixel(i, j);
	glm::vec3 pixel{ i,j, 1 };
	glm::vec3 direction = pixel - position;
	glm::vec3 normDirection = glm::normalize(direction);
	return Raytray(pixel, normDirection);
}

//get the pixel's cordinate according to the image
glm::vec3 Camera::getPixel(int i, int j)
{
	auto imageCordinates = glm::vec3(getImagePixel(i, j), focal);
	return position + imageCordinates;
}

//get the pixel within the image
glm::vec2 Camera::getImagePixel(int i, int j)
{
	auto x = width * ((float)i / pixelWidth - 0.5f) + 0.5f;
	auto y = height * ((float)i / pixelHeight - 0.5f) + 0.5f;
	return glm::vec2(x, y);
}
