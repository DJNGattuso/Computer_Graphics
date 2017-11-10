
#include "Plane.h"
#include <iostream>

Plane::Plane() {}

Plane::Plane(glm::vec3 n, glm::vec3 pos, glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shini)
{
	position = pos;
	nor = n;
	ambient = amb;
	diffuse = dif;
	specular = spe;
	shine = shini;
}

//------------------------Getters------------------------
glm::vec3 Plane::getNOR() { return nor; }
glm::vec3 Plane::getPosition() { return position; }
glm::vec3 Plane::getAmbient() { return ambient; }
float Plane::getInterDis() { return intersectDis; }


//from-https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
bool Plane::planeInter(glm::vec3 n, glm::vec3 worldOri, glm::vec3 rayOri, glm::vec3 direction)
{
	
	n = glm::normalize(n);

	float denom = glm::dot(n, direction);
	if (abs(denom) > 1e-6) 
	{
		glm::vec3 p0l0 = worldOri - rayOri;
		intersectDis = glm::dot(p0l0, n) / denom;
		intersectPoint = rayOri + direction * intersectDis;
		return (intersectDis >= 0);
	}
		return false;
}

glm::vec3 Plane::planeLight(glm::vec3 lightPosition, glm::vec3 direction, glm::vec3 lightColour)
{
	glm::vec3 normal = glm::normalize(nor);
	glm::vec3 negRay = -direction;

	glm::vec3 lightDirection = glm::normalize(intersectPoint - lightPosition);
	glm::vec3 reflection = glm::reflect(lightDirection, normal);
	float lighToNormal = glm::dot(normal, lightDirection);
	float refToRay = glm::dot(reflection, negRay);

	if (lighToNormal < 0) { lighToNormal = 0; }
	if (refToRay < 0) { refToRay = 0; }
	refToRay = pow(refToRay, shine);

	return (lightColour*(diffuse*lighToNormal + specular*refToRay));
}