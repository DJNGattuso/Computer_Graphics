
#include "Triangles.h"
#include <iostream>

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shini)
{
	vertex1 = v1; vertex2 = v2; vertex3 = v3;
	ambient = amb;
	diffuse = dif;
	specular = spe;
	shine = shini;
}

//------------------------Getters------------------------
glm::vec3 Triangle::getVertex1() { return vertex1; }
glm::vec3 Triangle::getVertex2() { return vertex2; }
glm::vec3 Triangle::getVertex3() { return vertex3; }
glm::vec3 Triangle::getAmbient() { return ambient; }
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

	intersectDis = glm::dot (v1v3, qvec) * invDet;
	intersectPoint = orig + dir * intersectDis;
	return true;
}

//assistance by daniel pri
glm::vec3 Triangle::triLight(glm::vec3 lightPosition, glm::vec3 direction, glm::vec3 lightColour)
{
	glm::vec3 normal = glm::normalize(glm::cross((vertex2 - vertex1), (vertex3 - vertex1)));
	normal = -normal;
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
