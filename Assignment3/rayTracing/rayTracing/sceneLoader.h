#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <string>
#include <fstream>
#include </Users/Nicholas/Programming/Computer_Graphics/Assignment3/rayTracing/glm/glm.hpp>

class Scene
{
public:
	Scene(const char* scenePath)
{}

	bool getLoad(){ return loaded;}

	void getCamera()
	{
		std::cout << "camera position is: " << camPos.x << " " << camPos.y << " " << camPos.z << std::endl;
		std::cout << "camera fov is: " << camFov << std::endl;
		std::cout << "camera f is: " << camF << std::endl;
		std::cout << "camera a is: " << camA << std::endl;
	}

private:
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