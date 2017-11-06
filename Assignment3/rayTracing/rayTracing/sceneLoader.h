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
		string scenePath = "scene.txt";
		string sceneCode;
		std::ifstream sceneStream(scenePath, ios::in);

		if (sceneStream.is_open()) {
			string Line = "";
			while (getline(sceneStream, Line))
				sceneCode += "\n" + Line;
			sceneStream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory ?\n", scenePath.c_str());
			getchar();
			exit(-1);
		}
	}

};




#endif