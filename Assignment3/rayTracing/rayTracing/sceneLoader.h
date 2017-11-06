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
	{
		//Read the scene file
		std::string sceneCode;
		std::ifstream sceneStream(scenePath, std::ios::in);

		if (sceneStream.is_open()) {
			std::string Line = "";
			int lineCount = 0; int maxCount = 0; int objectType = 0;
			//--------------------loop through the entire txt--------------------
			while (getline(sceneStream, Line))
			{
				sceneCode += "\n" + Line;
				int lineLength = Line.length() - 1;
				
				//---------first line of the txt file------------
				if (lineCount == 0)
				{
					objectNumb = Line[0] - '0';
					objectsCheck = 0;
					lineCount++;
				}
				else //all other lines
				{
					if (maxCount == 0) //next object
					{ 
						if (Line == "camera") { objectType = 1; maxCount = 4; }
						else if (Line == "triangle") { objectType = 2; maxCount = 7; }
						else if (Line == "sphere") { objectType = 3; maxCount = 6; }
						else if (Line == "light") { objectType = 4; maxCount = 2; }
						else if (Line == "plane") { objectType = 5; maxCount = 5; }
						else { objectType = 0; }
						objectsCheck++;
					}
					else //within an objects definition
					{
						/*
						//Camera
						if (objectType == 1)
						{
							if (maxCount == 4)
							{
								camPos.x = Line[5] - '0'; camPos.y = Line[6] - '0'; camPos.z = Line[7] - '0';
								maxCount--;
							}
							else if (maxCount == 3)
							{
								camFov = std::stoi(Line.substr(5, lineLength));
								maxCount--;
							}
							else if (maxCount == 2)
							{
								camF = std::stoi(Line.substr(3, lineLength));
								maxCount--;
							}
							else if (maxCount == 1)
							{
								camA = std::stod(Line.substr(3, lineLength));
								maxCount--;
							}
						}
						//triangle
						else if (objectType == 2)
						{
							if (maxCount == 7)
							{
								v1.x = Line[4] - '0'; v1.y = Line[6] - '0'; v1.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 6)
							{
								v2.x = Line[4] - '0'; v2.y = Line[6] - '0'; v2.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 5)
							{
								v3.x = Line[4] - '0'; v3.y = Line[6] - '0'; v3.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 4)
							{
								amb.x = Line[4] - '0'; amb.y = Line[6] - '0'; amb.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 3)
							{
								dif.x = Line[4] - '0'; dif.y = Line[6] - '0'; dif.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 2)
							{
								spe.x = Line[4] - '0'; spe.y = Line[6] - '0'; spe.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 1)
							{
								sphi = std::stoi(Line.substr(3, lineLength));
								maxCount--;
							}
						}
						//sphere
						else if (objectType == 3)
						{
							if (maxCount == 6)
							{
								sphPos.x = Line[4] - '0'; sphPos.y = Line[6] - '0'; sphPos.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 5)
							{
								radius = Line[5] - '0';
								maxCount--;
							}
							else if (maxCount == 4)
							{
								sphAmb.x = Line[4] - '0'; sphAmb.y = Line[6] - '0'; sphAmb.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 3)
							{
								sphDif.x = Line[4] - '0'; sphDif.y = Line[6] - '0'; sphDif.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 2)
							{
								sphSpe.x = Line[4] - '0'; sphSpe.y = Line[6] - '0'; sphSpe.z = Line[9] - '0';
								maxCount--;
							}
							else if (maxCount == 1)
							{
								sphSphi = Line[5] - '0';
								maxCount--;
							}
						}*/
					}
				}
			}
			sceneStream.close();
			loaded = true;
		}
		else {
			printf("Impossible to open %s. Are you in the right directory ?\n");
			loaded = false;
			exit(-1);
		}
	}

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