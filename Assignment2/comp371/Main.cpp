//modified from example skeleton code
//modified from http://learnopengl.com/
//Nicholas Gattuso 40007087

#include "stdafx.h"
#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "stb_image.h"
#include "objloader.hpp"  //include the object loader
#include "CImg.h"
using namespace cimg_library;
using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

glm::mat4 projection_matrix;

bool optionRight = false, optionLeft = false, optionMid = false; //variables to track the mouse click

//Variables for camera ------------------------------------------------------------------------------------------------------
const glm::vec3 center(-30.0f, 170.0f, -50.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
float panX = 0.0f, tiltY = 0.0f, zoom = 0.0f;		//variables that will be adjusted through cursor callback
float rotAnglex = 0.0f, rotAngley = 0.0f;			//variables to be adjusted through key callback

// Callback functions + funtions------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
double oldPositiony = 0; double oldPositionx = 0;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);

//functions to generate points in map-------------------------------
vector<glm::vec3> createPoints(vector<glm::vec3> original, int skip);
vector<glm::vec3> colourPoints(vector<glm::vec3> points);
vector<glm::vec3> CatmullRomX(vector<glm::vec3> points, float step, int height, int width);

//variables to control the environment-------------------------------------------------------------------------------------------
int tag = 1; //variable that will allow to change what step is being drawn
bool getInputs = false;
int skipsize;
float stepsize;
int trackHeight = 0, trackWidth = 0;


// The MAIN function---------------------------------------------------------------------------------------------------------------
int main()
{
	std::cout << "Starting Assignment 2 Nicholas Gattuso" << std::endl;
	
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //call function to readjust size

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);					//for keyboards
	glfwSetCursorPosCallback(window, cursor_position_callback); //for cursor
	glfwSetMouseButtonCallback(window, mouse_button_callback); //for mouse buttons

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions----------------------------------------------------------------------------------
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);


	// Build and compile our shader program-----------------------------------------------------------------------------
	// Vertex shader
	// Read the Vertex Shader code from the file
	string vertex_shader_path = "vertexShaderPacman.vs.txt";
	string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

	if (VertexShaderStream.is_open()) {
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	string fragment_shader_path = "fragmentShaderPacman.fs.txt";
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader---------------------------------------------------------------------------------
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); //free up memory
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);

	
	//--------------------------------------------------Image Setup-----------------------------------------------------
	//---------------------Load image-------------------------------------
	CImg<float> image("depth.bmp");
	CImgDisplay main_disp(image, "The image");

	//---------------------Get Pixel Data-----------------------------------
	vector <glm::vec3> imagePoints;

	//--------------------Loop through each pixel in the image----------------
	for (int x = (0 - image.width() / 2); x < (image.width())/2; x++)
	{
		for (int z = (0 - image.height() / 2); z < (image.height()) / 2; z++)
		{
			float height = static_cast<float>(*image.data(x + (image.width() / 2), z + (image.height() / 2)));
			imagePoints.emplace_back(glm::vec3(x, height, z));
		}
	}

	vector <glm::vec3> pointsColour = colourPoints(imagePoints);

	GLuint VAO_Image, VBO_Image, VBO_Colour;
	glGenVertexArrays(1, &VAO_Image);
	glGenBuffers(1, &VBO_Image);
	glGenBuffers(1, &VBO_Colour);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_Image);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Image);
	glBufferData(GL_ARRAY_BUFFER, imagePoints.size() * sizeof(glm::vec3), &imagePoints.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Colour);
	glBufferData(GL_ARRAY_BUFFER, pointsColour.size() * sizeof(glm::vec3), &pointsColour.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 1);

	glBindVertexArray(0); // Unbind VAO

	//-------------------------------------------Setup for points with skip size-----------------------------------------------------
	cout << "\nPlease enter a skip-size: ";
	cin >> skipsize;

	vector<glm::vec3> skipPoints = createPoints(imagePoints, skipsize);
	vector<glm::vec3> skipColour = colourPoints(skipPoints);

	GLuint VAO_Skip, VBO_Skip, VBO_SkipColour;
	glGenVertexArrays(1, &VAO_Skip);
	glGenBuffers(1, &VBO_Skip);
	glGenBuffers(1, &VBO_SkipColour);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_Skip);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Skip);
	glBufferData(GL_ARRAY_BUFFER, skipPoints.size() * sizeof(glm::vec3), &skipPoints.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_SkipColour);
	glBufferData(GL_ARRAY_BUFFER, skipColour.size() * sizeof(glm::vec3), &skipColour.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 1);

	glBindVertexArray(0);

	//------------------------------------------Setup Catmull Rom for the x----------------------------------------------------------
	cout << "\nPlease enter a step-size: ";
	cin >> stepsize;

	cout << "Getting point catx" << endl;
	vector<glm::vec3> catXPoints = CatmullRomX(imagePoints, stepsize, image.height(), image.width());
	cout << "Succeeded" << endl;
	cout << "Getting colourPoint with vector of size: " << catXPoints.size() << endl;
	vector<glm::vec3> CatXColour = colourPoints(catXPoints);
	cout << "Succeeded" << endl;
	GLuint VAO_CatX, VBO_CatX, VBO_CatXColour;
	glGenVertexArrays(1, &VAO_CatX);
	glGenBuffers(1, &VBO_CatX);
	glGenBuffers(1, &VBO_CatXColour);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_CatX);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_CatX);
	glBufferData(GL_ARRAY_BUFFER, catXPoints.size() * sizeof(glm::vec3), &catXPoints.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_CatXColour);
	glBufferData(GL_ARRAY_BUFFER, CatXColour.size() * sizeof(glm::vec3), &CatXColour.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 1);

	glBindVertexArray(0);
	
	/*
	int width1 = 0;
	int height1 = 0;
	int nchannels = 0;
	unsigned char *data = stbi_load("colouredImage.jpg", &width1, &height1, &nchannels, 4); 
	
	if (data) 
	{
		cout << "Success to load image" << endl;
	}
	else { cout << "Failed to load image" << endl; }

	stbi_image_free(data);
	*/


	//get the uniform locations from the shader ----------------------------------------------------------------------------------
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");
	GLuint object_type_loc = glGetUniformLocation(shaderProgram, "object_type");

	//enable for depth testing
	glEnable(GL_BLEND | GL_DEPTH_TEST);


	// Game loop------------------------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		if (getInputs)
		{
			cout << "\nPlease enter a skip-size: ";
			cin >> skipsize;

			skipPoints = createPoints(imagePoints, skipsize);
			skipColour = colourPoints(skipPoints);

			// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
			glBindVertexArray(VAO_Skip);

			glBindBuffer(GL_ARRAY_BUFFER, VBO_Skip);
			glBufferData(GL_ARRAY_BUFFER, skipPoints.size() * sizeof(glm::vec3), &skipPoints.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, VBO_SkipColour);
			glBufferData(GL_ARRAY_BUFFER, skipColour.size() * sizeof(glm::vec3), &skipColour.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 1);

			glBindVertexArray(0); // Unbind VAO

			getInputs = false;
		}

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		
		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//set the camera-----------------------------------------------------------------------------------------
		glm::mat4 view_matrix;
		glm::mat4 model_matrix;
		glm::vec3 eye(0.0f + panX, 700.0f + tiltY, 0.0f + zoom);
		view_matrix = glm::lookAt(eye, center, up);
		
		/*
		//adjust camera based on mouse actions
		view_matrix = glm::translate(view_matrix, glm::vec3(panX, 0.0f, 0.0f));
		view_matrix = glm::rotate(view_matrix, glm::radians(tiltY), glm::vec3(0.0f, 0.0f, 1.0f));
		//adjust camera based on key actions
		view_matrix = glm::rotate(view_matrix, glm::radians(rotAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
		view_matrix = glm::rotate(view_matrix, glm::radians(rotAngley), glm::vec3(0.0f, 1.0f, 0.0f));
		*/
		
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		
		switch (tag)
		{
		case 1:
			glBindVertexArray(VAO_Image);
			glDrawArrays(GL_POINTS, 0, imagePoints.size());
			glBindVertexArray(0);
			break;
		case 2:
			glBindVertexArray(VAO_Skip);
			glDrawArrays(GL_POINTS, 0, skipPoints.size());
			glBindVertexArray(0);
			break;
		case 3:
			glBindVertexArray(VAO_CatX);
			glDrawArrays(GL_POINTS, 0, catXPoints.size());
			glBindVertexArray(0);
			break;
		}
		
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;

}

//------------------------------------------Function to generate image points based on a skip----------------------------------------
vector<glm::vec3> createPoints(vector<glm::vec3> original, int skip)
{
	vector<glm::vec3> newPoints;

	//------------------------Loop through each pixel in the image----------------------------
	for (int i = 0; i < original.size(); i += skip)
	{
		newPoints.emplace_back(original[i]);
	}

	return newPoints;
}

//------------------------------------------Function to generate image points based on a skip----------------------------------------
vector<glm::vec3> colourPoints(vector<glm::vec3> points)
{
	vector<glm::vec3> pointsColour;

	for (int i = 0; i <= points.size() - 1; i++)
	{
		//------------------------Setup to get colour for each point---------------------------
		if (points[i].y < 67) { pointsColour.emplace_back(glm::vec3(0.0, 0.0, 1.0)); }
		else if (67 <= points[i].y && points[i].y < 85) { pointsColour.emplace_back(glm::vec3(0.2, 0.0, 0.8)); }
		else if (85 <= points[i].y && points[i].y < 100) { pointsColour.emplace_back(glm::vec3(0.4, 0.0, 0.6)); }
		else if (100 <= points[i].y && points[i].y < 120) { pointsColour.emplace_back(glm::vec3(0.6, 0.0, 0.4)); }
		else if (120 <= points[i].y && points[i].y < 140) { pointsColour.emplace_back(glm::vec3(0.8, 0.2, 0.2)); }
		else if (140 <= points[i].y && points[i].y < 160) { pointsColour.emplace_back(glm::vec3(0.8, 0.4, 0.1)); }
		else if (160 <= points[i].y && points[i].y < 180) { pointsColour.emplace_back(glm::vec3(1.0, 0.5, 0.1)); }
		else if (180 <= points[i].y && points[i].y < 200) { pointsColour.emplace_back(glm::vec3(1.0, 0.7, 0.0)); }
		else if (200 <= points[i].y && points[i].y < 220) { pointsColour.emplace_back(glm::vec3(0.7, 0.8, 0.0)); }
		else if (220 <= points[i].y && points[i].y < 240) { pointsColour.emplace_back(glm::vec3(0.5, 0.9, 0.0)); }
		else { pointsColour.emplace_back(glm::vec3(0.0, 1.0, 0.0)); }
	}
	return pointsColour;
}

//------------------------------------------Function to generate Catmull Rom points based on a skip----------------------------------------
//referenced for equation from: http://hawkesy.blogspot.ca/2010/05/catmull-rom-spline-curve-implementation.html
vector<glm::vec3> CatmullRomX(vector<glm::vec3> points, float step, int height, int width)
{
	glm::vec3 point0, point1, point2, point3;
	vector<glm::vec3> catmullRom;
	int index = 0;

	for (int j = 0; j <= height - 1; j++) //start at first row, go to next row until last
	{
		index = j;
		cout << "first loop" << endl;
		for (int i = 0; i < ((height * width) - (5 * height)); i += height) //start at first column, go to next column until 4th to last
		{
			point0 = points[index];
			point1 = points[index + height];
			point2 = points[index + height + height];
			point3 = points[index + height + height + height];

			glm::vec3 newpoint = 0.5f * ((2.0f * point1) + (point2 - point0) * step + (2.0f * point0 - 5.0f * point1 + 4.0f * point2 - point3) *
				step * step + (3.0f * point1 - point0 - 3.0f * point2 + point3) * step * step * step);

			catmullRom.emplace_back(newpoint);
			index += height;
		}
	}
	return catmullRom;
}


//key callback function------------------------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) 
	{
	std::cout << key << std::endl;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	//Change Camera--------------------------------------------------------------------------------
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		zoom -= 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		zoom += 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		panX += 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		panX -= 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		tiltY += 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		tiltY -= 10.0f;
	}
	//change orientation--------------------------------------------------------------------------
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rotAngley -= 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		rotAngley += 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		rotAnglex += 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		rotAnglex -= 0.5f;
	}
	//Show Specific Step based on the tag------------------------------------------------------------------------------------
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) //show all points
	{
		tag = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) //show the skip points
	{
		tag = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) //show the skip points
	{
		tag = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) //show the skip points
	{
		getInputs = true;
	}
	//fix orientation-----------------------------------------------------------------------------------
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		rotAnglex = 0.0f; rotAngley = 0.0f;
		panX = 0.0f; tiltY = 0.0f; zoom = 1.0f;
	}
}


//cursor function isn't working well--------------------------------------------------------------------------------------------
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (optionRight == true) //right button on mouse is pressed -> pan
	{
		float xoffset = static_cast<float>(xpos - oldPositionx);
		oldPositionx = xpos;
		xoffset *= 0.5;
		panX = xoffset;
	}
	else if (optionMid == true) //middle button on mouse is pressed -> tilt
	{
		float yoffset = static_cast<float>(oldPositiony - ypos);
		oldPositiony = ypos;
		yoffset *= 0.005;
		tiltY = yoffset;

		//constraints on the y axis
		if (tiltY > 89.0f) { tiltY = 89.0f; }
		if (tiltY < -89.0f) { tiltY = -89.0f; }
	}
}

//mouse callback----------------------------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	//right button is pressed---------------------------------------------------------------------
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) { optionRight = true; }
	else { optionRight = false; oldPositiony = 0; oldPositionx = 0;}
	
	//left butto is pressed------------------------------------------------------------------------
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) { optionLeft = true; }
	else { optionLeft = false; oldPositiony = 0; oldPositionx = 0;}

	//middle button is pressed---------------------------------------------------------------------
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) { optionMid = true; }
	else { optionMid = false; oldPositiony = 0; oldPositionx = 0;}
}


//callback function for whenever the window size changes--------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);
}

//function to restart the game by repositionning all objects--------------------------------------------------------------------------
void restartGame()
{
	
}