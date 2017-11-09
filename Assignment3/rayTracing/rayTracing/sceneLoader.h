#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>
/*#include "Camera.h"
#include "Sphere.h"
#include "Raytracing.h"
#include "Triangles.h"
#include "Plane.h"
#include "Light.h"*/

class Scene
{
public:
	Scene(const char* scenePath);

	//--------getters-------
	bool getLoad();
	void getCamera();
	int getTotalObjects();
	//std::vector <void> getObjects();

private:
	void createScene();
	//std::vector<void> objects;
	/*
	Camera camera;
	std::vector <Light> lights;
	std::vector <Sphere> sphere;
	std::vector <Triangle> triangle;
	std::vector <Plane> planes;
	*/

	//Check if the file is loaded
	bool loaded;

	//Check total number of objects
	int objectNumb;
	int objectsCheck;

	//Camera variable
	glm::vec3 camPos;
	int camFov;
	int camF;
	double camA;

	//Triangle variable
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
	glm::vec3 amb;
	glm::vec3 dif;
	glm::vec3 spe;
	double sphi;

	//square variable
	glm::vec3 sphPos;
	int radius;
	glm::vec3 sphAmb;
	glm::vec3 sphDif;
	glm::vec3 sphSpe;
	int sphSphi;
};




#endif