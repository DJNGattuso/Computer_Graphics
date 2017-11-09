
#include "Light.h"
#include <iostream>

Light::Light(glm::vec3 pos, glm::vec3 col)
{
	position = pos;
	colour = col;
}

//------------------------Getters------------------------
glm::vec3 Light::getPosition() { return position; }
glm::vec3 Light::getColour() { return colour; }