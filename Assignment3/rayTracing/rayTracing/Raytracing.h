#pragma once
#ifndef RAYTRAY_H
#define RAYTRAY_H

#include <iostream>
#include <string>
#include <fstream>
#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Raytray
{
public:
	Raytray(glm::vec3 origin, glm::vec3 direction);

	//--------Getters-------
	glm::vec3 getOri();
	glm::vec3 getDi();
	glm::vec3 getPoint(float t);
	//--------Setters-------
	void setOri(glm::vec3 origin);
	void setDi(glm::vec3 direction);

private:
	glm::vec3 origin;
	glm::vec3 direction;
};

#endif