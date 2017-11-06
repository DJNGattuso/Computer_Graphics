
#include "Sphere.h"
#include <iostream>

Sphere::Sphere( glm::vec3 cent, float radius, glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shini)
	: Objprim(amb, dif, spe, shini) 
{
	setCenter(cent);
	setRadius(radius);
}

//------------------------Getters------------------------
glm::vec3 Sphere::getCenter() { return center; }
float Sphere::getRadius() { return radius; }
//------------------------Setters-------------------------
void Sphere::setCenter(glm::vec3 cent) { center = cent; }
void Sphere::setRadius(float rad)
{
	radius = rad;
}
