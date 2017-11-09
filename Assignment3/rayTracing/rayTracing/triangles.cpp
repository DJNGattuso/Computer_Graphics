
#include "Triangles.h"
#include <iostream>

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 spe, float shine)
	: Objprim(ambient, diffuse, spe, shine)
{
	vertex1 = v1; vertex2 = v2; vertex3 = v3;
}

//------------------------Getters------------------------
glm::vec3 Triangle::getVertex1() { return vertex1; }
glm::vec3 Triangle::getVertex2() { return vertex2; }
glm::vec3 Triangle::getVertex3() { return vertex3; }
glm::vec3 Triangle::getAmbient() { return Objprim::getAmbient(); }
float Triangle::getInterDis() { return intersectDis; }


//from-https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool Triangle::triInter(glm::vec3 rayPoint, glm::vec3 direction)
{
	return true;
}
