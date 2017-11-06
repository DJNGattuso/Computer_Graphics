#pragma once
#ifndef OBJPRIM_H
#define OBJPRIM_H

#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>
#include "Raytracing.h"

class Objprim
{
protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shine;

public:
	Objprim(const glm::vec3 ambientColour, const glm::vec3 diffuseColour, const glm::vec3 specularColour, const float shininess);
	Objprim();
	//---------set an intersection----------
	//virtual std::pair<bool, float> intersection(Raytray ray) = 0;
	//----------Getters--------
	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;
	float getShininess() const;
	//----------Setters---------
	void setAmbientColour(const glm::vec3 ambientColour);
	void setDiffuseColour(const glm::vec3 diffuseColour);
	void setSpecularColour(const glm::vec3 specularColour);
	void setShininess(const float shininess);
};

#endif