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
								camFov = Line[5,6] - '0';
								maxCount--;
							}
							else if (maxCount == 2)
							{
								camF = Line[3:lineLength] - '0';
								maxCount--;
							}
							else if (maxCount == 1)
							{
								camA = Line[3:lineLength] - '0';
								maxCount--;
							}
						}
						//triangle
						else if (objectType == 2)
						{
							if (maxCount == 4)
							{
								camPos.x = Line[5] - '0'; camPos.y = Line[6] - '0'; camPos.z = Line[7] - '0';
								maxCount--;
							}
							else if (maxCount == 3)
							{
								camFov = Line[5, 6] - '0';
								maxCount--;
							}
							else if (maxCount == 2)
							{
								camF = Line[3:lineLength] - '0';
								maxCount--;
							}
							else if (maxCount == 1)
							{
								camA = Line[3:lineLength] - '0';
								maxCount--;
							}
						}
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


};




#endif