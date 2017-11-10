#pragma once

#ifndef PLANE_H
#define PLANE_H

#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Plane 
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

	//light function
	glm::vec3 planeLight(glm::vec3 lightPosition, glm::vec3 direction, glm::vec3 lightColour);

private:
	glm::vec3 nor;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shine;

	float intersectDis;
	glm::vec3 intersectPoint;
};

#endif