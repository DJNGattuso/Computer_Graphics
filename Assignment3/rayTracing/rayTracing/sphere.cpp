
#include "Sphere.h"
#include <iostream>

Sphere::Sphere(const glm::vec3 cent, const float radius, glm::vec3 amb, const glm::vec3 dif, const glm::vec3 spe, const float shini)
	: Objprim(amb, dif, spe, shini) 
{
	setCenter(cent);
	setRadius(radius);
}


glm::vec3 Sphere::getCenter() const { return center; }

float Sphere::getRadius() const { return radius; }


void Sphere::setCenter(const glm::vec3 cent) { center = cent; }


void Sphere::setRadius(const float rad)
{
	if (radius <= 0.)
	{
		throw std::logic_error("Radius must be positive.");
	}
	radius = rad;
}
