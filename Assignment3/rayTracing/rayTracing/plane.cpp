
#include "Plane.h"
#include <iostream>

Plane::Plane(glm::vec3 n, glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 spe, float shine)
	: Objprim(ambient, diffuse, spe, shine)
{
	position = pos;
	nor = n;
}

//------------------------Getters------------------------
glm::vec3 Plane::getNOR() { return nor; }
glm::vec3 Plane::getPosition() { return position; }
glm::vec3 Plane::getAmbient() { return Objprim::getAmbient(); }
float Plane::getInterDis() { return intersectDis; }


//from-https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
bool Plane::planeInter(glm::vec3 n, glm::vec3 worldOri, glm::vec3 rayOri, glm::vec3 direction)
{
	
	n = glm::normalize(n);
	direction = glm::normalize(direction);

	float denom = glm::dot(n, direction);
	if (denom > 1e-6) 
	{
		glm::vec3 p0l0 = worldOri - rayOri;
		intersectDis = glm::dot(p0l0, n) / denom;
		return (intersectDis >= 0);
	}
		return false;
}
