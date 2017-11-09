
#include "Sphere.h"
#include <iostream>

Sphere::Sphere( glm::vec3 cent, float rad, glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shini)
	: Objprim(amb, dif, spe, shini) 
{
	center = cent;
	radius = rad;
}

//------------------------Getters------------------------
glm::vec3 Sphere::getCenter() { return center; }
float Sphere::getRadius() { return radius; }
glm::vec3 Sphere::getAmbient() { return Objprim::getAmbient(); }
float Sphere::getInterDis() { return intersectDis; }
//------------------------Setters-------------------------
void Sphere::setCenter(glm::vec3 cent) { center = cent; }
void Sphere::setRadius(float rad){
	radius = rad;
}

//from-https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool Sphere::sphereInter(glm::vec3 rayPoint, glm::vec3 direction)
{
	float t0, t1;
	glm::vec3 distOrigPos = center - rayPoint;
	float tca = glm::dot(distOrigPos, direction);

	// if (tca < 0) return false;
	float d2 = glm::dot(distOrigPos, distOrigPos) - tca * tca;
	if (d2 > (radius * radius)) { return false; }

	float thc = sqrt((radius * radius) - d2);
	t0 = tca - thc;
	t1 = tca + thc;
	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) { return false; } // both t0 and t1 are negative 
	}

	intersectDis = t0;

	return true;
}
