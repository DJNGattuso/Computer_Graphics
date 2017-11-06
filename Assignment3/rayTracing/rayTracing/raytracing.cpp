
#include "Raytracing.h"


Raytray::Raytray(const glm::vec3 origin, glm::vec3 direction)
{
	setOri(origin);
	setDi(direction);
}

//---------------------------------Getters-------------------------------
glm::vec3 Raytray::getOri(){ return origin; }
glm::vec3 Raytray::getDi(){ return direction; }
glm::vec3 Raytray::getPoint(float distance)
{
	return origin + direction * distance;
}

//--------------------------------Setters------------------------------
void Raytray::setOri(glm::vec3 ori) { origin = ori; }
void Raytray::setDi(glm::vec3 di){ direction = di; }
