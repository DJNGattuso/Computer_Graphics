//Nicholas Gattuso 40007087


#include <iostream>
#include <string>
#include <fstream>
#include "sceneLoader.h"
#include "CImg.h"
#include "Camera.h"
using namespace std;

int main()
{
	cout << "main program for ray tracing" << endl; 
	//for testing purpose

	//------------------------------------Read text file and get the object variables----------------------------------------------
	Scene loadtext("scene.txt");

	if (loadtext.getLoad()) { cout << "read succesfully" << endl; }
	else { cout << "Read failed" << endl; }

	
	//-----------------------------------------Create camera object and image space-------------------------------------------------
	Camera camera(glm::vec3{ 0,0,0 }, 60, 1000, 1.33);




	int n;
	cin >> n;


	return 0;
}