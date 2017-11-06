#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "objectPrim.h"
#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Sphere : Objprim
{
public:
	Sphere(glm::vec3 center, float radius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 spe, float shine);

	//Getters and setters
	glm::vec3 getCenter();
	float getRadius();
	void setCenter(glm::vec3 centre);
	void setRadius(float radius);

private:
	glm::vec3 center;
	float radius;
};

#endif