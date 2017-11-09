#pragma once
#ifndef PLANE_H
#define PLANE_H

#include "objectPrim.h"
#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Plane : Objprim
{
public:
	Plane(glm::vec3 nor, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 spe, float shine);

	//Getters and setters
	glm::vec3 getNOR();
	glm::vec3 getPosition();
	glm::vec3 getAmbient();
	float getInterDis();

	//Intersection Function
	bool planeInter(glm::vec3 n, glm::vec3 worldOri, glm::vec3 rayOri, glm::vec3 direction);

private:
	glm::vec3 nor;
	glm::vec3 position;
	float intersectDis;
};

#endif