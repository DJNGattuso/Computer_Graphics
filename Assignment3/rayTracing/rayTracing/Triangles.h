#pragma once
#pragma once
#ifndef TRIANGLES_H
#define TRIANGLES_H

#include "objectPrim.h"
#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Triangle : Objprim
{
public:
	Triangle(glm::vec3 vectex1, glm::vec3 vectex2, glm::vec3 vectex3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 spe, float shine);

	//Getters and setters
	glm::vec3 getVertex1();
	glm::vec3 getVertex2();
	glm::vec3 getVertex3();
	glm::vec3 getAmbient();
	float getInterDis();


	//Intersection Function
	bool triInter(glm::vec3 orig, glm::vec3 dir, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

private:
	glm::vec3 vertex1;
	glm::vec3 vertex2;
	glm::vec3 vertex3;
	float intersectDis;
};

#endif