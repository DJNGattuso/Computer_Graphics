#pragma once
#ifndef SPHERE_H
#define SPHERE_H


#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Sphere
{
public:
	Sphere(const glm::vec3 center, const float radius, glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 spe, const float shine);

	//Getters and setters
	glm::vec3 getCenter() const;
	float getRadius() const;
	void setCenter(const glm::vec3 centre);
	void setRadius(const float radius);

private:
	glm::vec3 center;
	float radius;
};

#endif