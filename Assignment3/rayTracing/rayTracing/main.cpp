//Nicholas Gattuso 40007087


#include <iostream>
#include <string>
#include <fstream>
#include "sceneLoader.h";
#include "CImg.h";
using namespace std;

int main()
{
	cout << "main program for ray tracing" << endl; 
	//for testing purpose

	Scene loadtext("scene.txt");

	if (loadtext.getLoad()) { cout << "read succesfully" << endl; }
	else { cout << "Read failed" << endl; }

	int n;
	cin >> n;


	return 0;
}