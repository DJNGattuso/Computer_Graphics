#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Sphere
{
public:
	Sphere(glm::vec3 center, float radius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 spe, float shine);

	//Getters
	glm::vec3 getCenter();
	float getRadius();
	glm::vec3 getAmbient();
	float getInterDis();
	glm::vec3 getNearest();

	//Intersection Function
	bool sphereInter(glm::vec3 rayPoint, glm::vec3 direction);

	//Light Function
	glm::vec3 sphereLight(glm::vec3 lightPosition, glm::vec3 direction, glm::vec3 lightColour);

private:
	//object variables
	glm::vec3 center;
	float radius;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shine;

	//light and intersection variables
	float intersectDis;
	glm::vec3 intersectPoint;
};

#endif