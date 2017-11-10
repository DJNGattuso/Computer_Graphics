
#include "Sphere.h"
#include <iostream>

Sphere::Sphere( glm::vec3 cent, float rad, glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shini)
{
	center = cent;
	radius = rad;
	ambient = amb;
	diffuse = dif;
	specular = spe;
	shine = shini;
}

//------------------------Getters------------------------
glm::vec3 Sphere::getCenter() { return center; }
float Sphere::getRadius() { return radius; }
glm::vec3 Sphere::getAmbient() { return ambient; }
float Sphere::getInterDis() { return intersectDis; }


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
	intersectPoint = rayPoint + direction * intersectDis;
	return true;
}

//referenced by Daniel Pri
glm::vec3 Sphere::sphereLight(glm::vec3 lightPosition, glm::vec3 direction, glm::vec3 lightColour)
{
	glm::vec3 normal = glm::normalize(center - intersectPoint);
	glm::vec3 negRay = -direction;

	glm::vec3 lightDirection = glm::normalize(intersectPoint - lightPosition);
	glm::vec3 reflection = glm::reflect(lightDirection, normal);
	float lighToNormal = glm::dot(normal, lightDirection);
	float refToRay = glm::dot(reflection, negRay);

	if (lighToNormal < 0) { lighToNormal = 0; }
	if (refToRay < 0) { refToRay = 0; }
	refToRay = pow(refToRay, shine);

	return (lightColour*(diffuse*lighToNormal + specular*refToRay));
}