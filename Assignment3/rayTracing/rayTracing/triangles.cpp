
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


//applying Möller-Trumbore 
//referenced to-https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
bool Triangle::triInter(glm::vec3 orig, glm::vec3 dir, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	glm::vec3 v1v2 = v2 - v1;
	glm::vec3 v1v3 = v3 - v1;
	glm::vec3 pvec = glm::cross(dir,v1v3);
	float det = glm::dot(v1v2,pvec);

	// if the determinant is negative the triangle is backfacing
	// if the determinant is close to 0, the ray misses the triangle
	if (det < 1e-8) return false;

	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < 1e-8) return false;
 

	float invDet = 1 / det;

	glm::vec3 tvec = orig - v1;
	float u = glm::dot(tvec,pvec) * invDet;
	if (u < 0 || u > 1) return false;

	glm::vec3 qvec = glm::cross(tvec,v1v2);
	float v = glm::dot(dir,qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	float intersectDis = glm::dot (v1v3, qvec) * invDet;
	return true;
}
