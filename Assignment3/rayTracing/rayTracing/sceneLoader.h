#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <string>
#include <fstream>

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
			int lineCount = 0; int maxCount = 0;
			//--------------------loop through the entire txt--------------------
			while (getline(sceneStream, Line))
			{
				sceneCode += "\n" + Line;
				
				//---------first line of the txt file------------
				//if (lineCount == 0)
				//{

				//}
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
	bool loaded;

};




#endif