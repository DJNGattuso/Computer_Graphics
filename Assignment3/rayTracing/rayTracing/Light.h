#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Light
{
public:
	Light(glm::vec3 pos, glm::vec3 col);

	//Getters and setters
	glm::vec3 getPosition();
	glm::vec3 getColour();

private:
	glm::vec3 position;
	glm::vec3 colour;
};

#endif