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
#include "objloader.hpp"  //include the object loader

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

float X = -0.35f; float Y = -0.35f; //initial position for pacman
float camX = 0.0f; float camY = 0.0f; float camZ = 0.0f; //initial position for the camera
int renderKey = 0; //render key for points, lines or triangles
int numberFood = rand() % 15 + 1; //random number of objects for pacman to get
glm::vec3 foodPositions[15]; //positions for the food
glm::vec3 ghostPositions[4]; //positions for the ghost
bool drawFood[15]; //check to draw food at a certain position
int ate = 0; //variable to monitor how many objects pacman ate

glm::vec3 camera_position; 
glm::vec3 triangle_scale;
glm::vec3 sphere_scale;
glm::mat4 projection_matrix;

// Constant vectors
const glm::vec3 center(0.0f, 0.0f, 0.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 eye(0.0f + camX, 0.0f + camY, 3.0f + camZ);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void restartGame(); //function to restart the game

//functions to get a random point in the grid
float getXPoint();
float getYPoint();

// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting Pacman Assignment Nicholas Gattuso" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Pacman", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //call function to readjust size
	
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback); //for keyboards
	glfwSetCursorPosCallback(window, cursor_position_callback); //for cursor
	glfwSetMouseButtonCallback(window, mouse_button_callback); //for mouse buttons
	glfwSetScrollCallback(window, scroll_callback); //for scroll

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//enable for depth testing
	//glEnable(GL_DEPTH_TEST);

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);

	// Build and compile our shader program
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
	// Fragment shader
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

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	loadOBJ("pacman.obj", vertices, normals, UVs); //read the vertices from the .obj file;

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	GLuint vertices_VBO, normals_VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertices_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	vector<glm::vec3> vertices2;
	vector<glm::vec3> normals2;
	vector<glm::vec2> UVs2;
	loadOBJ("sphere.obj", vertices2, normals2, UVs2); //read the vertices from the .obj file;

												   //loop to generate the random locations for the food positions and to initialize their drawing to true
	for (int i = 0; i <= numberFood - 1; i++)
	{
		foodPositions[i] = { getXPoint(), getYPoint(), 0.0f };
		drawFood[i] = true;
	}

	GLuint VAO_sphere, VBO_sphere;
	glGenVertexArrays(1, &VAO_sphere);
	glGenBuffers(1, &VBO_sphere);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	GLuint vertices_VBO_sphere, normals_VBO_sphere;

	glGenVertexArrays(1, &VAO_sphere);
	glGenBuffers(1, &vertices_VBO_sphere);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_sphere);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO_sphere);
	glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec3), &vertices2.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normals_VBO_sphere);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO_sphere);
	glBufferData(GL_ARRAY_BUFFER, normals2.size() * sizeof(glm::vec3), &normals2.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	vector<glm::vec3> vertices3;
	vector<glm::vec3> normals3;
	vector<glm::vec2> UVs3;
	loadOBJ("teapot.obj", vertices3, normals3, UVs3); //read the vertices from the .obj file;

	//loop to generate the random locations for the ghost positions
	for (int i = 0; i <= 3; i++)
	{
		ghostPositions[i] = { getXPoint(), getYPoint(), 0.0f };
	}

	GLuint VAO_ghost, VBO_ghost;
	glGenVertexArrays(1, &VAO_ghost);
	glGenBuffers(1, &VBO_ghost);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	GLuint vertices_VBO_ghost, normals_VBO_ghost;

	glGenVertexArrays(1, &VAO_ghost);
	glGenBuffers(1, &vertices_VBO_ghost);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_ghost);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO_ghost);
	glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(glm::vec3), &vertices3.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normals_VBO_ghost);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO_ghost);
	glBufferData(GL_ARRAY_BUFFER, normals3.size() * sizeof(glm::vec3), &normals3.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	float colours_cordinate[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	float cordinate_axis[] = {
		0.0f, 0.0f, 0.0f,	0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,	0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.5f,
	};

	unsigned int VBO2, VAO2, VBO_cordcolour;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2); //bind the Vertex Array object

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2); //bind the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(cordinate_axis), cordinate_axis, GL_STATIC_DRAW); //copy defined vertex data into buffer memory
																						   //linking vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //3 vertex of 3 positions each of 4 bytes
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &VBO_cordcolour);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cordcolour); //bind the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours_cordinate), colours_cordinate, GL_STATIC_DRAW); //copy defined vertex data into buffer memory

																								 //linking vertex attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //3 vertex of 3 positions each of 4 bytes
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBindVertexArray(0);

	float grid[] = {
		0.8f, 0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		-0.8f, -0.8f, 0.0f,
		-0.8f, -0.8f, 0.0f,
		-0.8f, 0.8f, 0.0f,
		-0.8f, 0.8f, 0.0f,
		0.8f, 0.8f, 0.0f,

		-0.7238f, 0.8f, 0.0f,
		-0.7238f, -0.8f, 0.0f, //1
		-0.6476f, 0.8f, 0.0f,
		-0.6476f, -0.8f, 0.0f, //2
		-0.5714f, 0.8f, 0.0f,
		-0.5714f, -0.8f, 0.0f, //3
		-0.4952, 0.8f, 0.0f,
		-0.4952f, -0.8f, 0.0f, //4 
		-0.419f, 0.8f, 0.0f,
		-0.419f, -0.8f, 0.0f, //5
		-0.3428f, 0.8f, 0.0f,
		-0.3428f, -0.8f, 0.0f, //6
		-0.2666f, 0.8f, 0.0f,
		-0.2666f, -0.8f, 0.0f, //7
		-0.1904f, 0.8f, 0.0f,
		-0.1904f, -0.8f, 0.0f, //8
		-0.1142f, 0.8f, 0.0f,
		-0.1142f, -0.8f, 0.0f, //9
		-0.038f, 0.8f, 0.0f,
		-0.038f, -0.8f, 0.0f, //10
		0.038f, 0.8f, 0.0f,
		0.038f, -0.8f, 0.0f, //11
		0.7238f, 0.8f, 0.0f,
		0.7238f, -0.8f, 0.0f, //12
		0.6476f, 0.8f, 0.0f,
		0.6476f, -0.8f, 0.0f, //13
		0.5714f, 0.8f, 0.0f,
		0.5714f, -0.8f, 0.0f, //14
		0.4952, 0.8f, 0.0f,
		0.4952f, -0.8f, 0.0f, //15
		0.419f, 0.8f, 0.0f,
		0.419f, -0.8f, 0.0f, //16
		0.3428f, 0.8f, 0.0f,
		0.3428f, -0.8f, 0.0f, //17
		0.2666f, 0.8f, 0.0f,
		0.2666f, -0.8f, 0.0f, //18
		0.1904f, 0.8f, 0.0f,
		0.1904f, -0.8f, 0.0f, //19
		0.1142f, 0.8f, 0.0f,
		0.1142f, -0.8f, 0.0f,//20

		 //vertical points
		 -0.8f, -0.7238f, 0.0f,
		 0.8f, -0.7238f, 0.0f, //1
		 -0.8f, -0.6476f, 0.0f,
		 0.8f, -0.6476f, 0.0f, //2
		 -0.8f, -0.5714f, 0.0f,
		 0.8f, -0.5714f, 0.0f, //3
		 -0.8f, -0.4952, 0.0f,
		 0.8f, -0.4952f, 0.0f, //4 
		 -0.8f, -0.419f, 0.0f,
		 0.8f, -0.419f, 0.0f, //5
		 -0.8f, -0.3428f, 0.0f,
		 0.8f, -0.3428f, 0.0f, //6
		 -0.8f, -0.2666f, 0.0f,
		 0.8f,-0.2666f, 0.0f, //7
		 -0.8f, -0.1904f, 0.0f,
		 0.8f, -0.1904f, 0.0f, //8
		 -0.8f, -0.1142f, 0.0f,
		 0.8f, -0.1142f, 0.0f, //9
		 -0.8f, -0.038f, 0.0f,
		 0.8f, -0.038f, 0.0f, //10
		 -0.8f, 0.038f, 0.0f,
		 0.8f, 0.038f, 0.0f, //11
		 -0.8f, 0.7238f, 0.0f,
		 0.8f, 0.7238f, 0.0f, //12
		 -0.8f, 0.6476f, 0.0f,
		 0.8f, 0.6476f, 0.0f, //13
		 -0.8f, 0.5714f, 0.0f,
		 0.8f, 0.5714f, 0.0f, //14
		 -0.8f, 0.4952f, 0.0f,
		 0.8f, 0.4952f, 0.0f, //15
		 -0.8f, 0.419f, 0.0f,
		 0.8f, 0.419f, 0.0f, //16
		 -0.8f, 0.3428f, 0.0f,
		 0.8f, 0.3428f, 0.0f, //17
		 -0.8f, 0.2666f, 0.0f,
		 0.8f, 0.2666f, 0.0f, //18
		 -0.8f, 0.1904f, 0.0f,
		 0.8f, 0.1904f, 0.0f, //19
		 -0.8f, 0.1142f, 0.0f,
		 0.8f, 0.1142f, 0.0f

	}; //0.07619


	unsigned int VBO3, VAO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);

	glBindVertexArray(VAO3); //bind the Vertex Array object

	glBindBuffer(GL_ARRAY_BUFFER, VBO3); //bind the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(grid), grid, GL_STATIC_DRAW); //copy defined vertex data into buffer memory

																	   //linking vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //3 vertex of 3 positions each of 4 bytes
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	triangle_scale = glm::vec3(.005f);
	sphere_scale = glm::vec3(0.05f);

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");
	GLuint object_type_loc = glGetUniformLocation(shaderProgram, "object_type");

	float markX; float markY; 
	float distanceToPacX, distanceToPacY, absDistanceX, absDistanceY; //variable to calculate the distance from ghost to pacman
	int movement, countMovements[4] = { 0,0,0,0 }; //variable to decide which direction should the ghost move to

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 view_matrix;
		eye = { 0.0f + camX, 0.0f + camY, 3.0f + camZ };
		view_matrix = glm::lookAt(eye, center, up);

		glm::mat4 model_matrix;
		
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		//draw the cordinate axis
		glUniform1i(object_type_loc, 0);
		model_matrix = glm::scale(model_matrix, glm::vec3(1.2f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glBindVertexArray(VAO2);
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);

		//draw the grid
		glUniform1i(object_type_loc, 3);
		glm::mat4 model_matrixGrid;
		model_matrixGrid = glm::scale(model_matrix, glm::vec3(1.5f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrixGrid));
		glBindVertexArray(VAO3);
		glDrawArrays(GL_LINES, 0, 100);
		glBindVertexArray(0);

		//draw pacman
		glUniform1i(object_type_loc, 2);
		glm::mat4 model_matrixPac;
		glm::mat4 identityPac = glm::mat4(1.0f);
		glm::vec3 pacPosition = { X, Y, 0.0f };
		glm::mat4 translatePac = glm::translate(model_matrixPac, pacPosition);
		glm::mat4 scalePac = glm::scale(model_matrixPac, glm::vec3(triangle_scale));
		model_matrixPac = translatePac * scalePac * identityPac;
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrixPac));
		glBindVertexArray(VAO);
		if (renderKey == 0) { glDrawArrays(GL_TRIANGLES, 0, vertices.size()); }
		else if (renderKey == 1) { glDrawArrays(GL_POINTS, 0, vertices.size()); }
		else if (renderKey == 2) { glDrawArrays(GL_LINES, 0, vertices.size()); }
		glBindVertexArray(0);

		//draw ghost
		for (int i = 0; i <= 3; i++)
		{
			if (countMovements[i] == 150) {
				distanceToPacX = ghostPositions[i].x - pacPosition.x;
				absDistanceX = abs(distanceToPacX);
				distanceToPacY = ghostPositions[i].y - pacPosition.y;
				absDistanceY = abs(distanceToPacY);
				if (absDistanceX <= 0.001f && absDistanceY <= 0.001f) { restartGame(); }
				else if (absDistanceX <= 0.001f)
				{
					if (distanceToPacY < 0 && ghostPositions[i].y != 1.47f) { ghostPositions[i].y += 0.140f; }
					else if (distanceToPacY > 0 && ghostPositions[i].y != -1.47f) { ghostPositions[i].y -= 0.140f; }
				}
				else if (absDistanceY <= 0.001f)
				{
					if (distanceToPacX < 0 && ghostPositions[i].x != 1.47f) { ghostPositions[i].x += 0.140f; }
					else if (distanceToPacX > 0 && ghostPositions[i].x != -1.47f) { ghostPositions[i].x -= 0.140f; }
				}
				else
				{
					movement = rand() % 2 + 1; //choice between 1 and 2

					if (movement == 1)
					{
						if (distanceToPacX < 0 && ghostPositions[i].x != 1.47f) { ghostPositions[i].x += 0.140f; }
						else if (distanceToPacX > 0 && ghostPositions[i].x != -1.47f){ ghostPositions[i].x -= 0.140f; }
					}
					else
					{
						if (distanceToPacY < 0 && ghostPositions[i].y != 1.47f) { ghostPositions[i].y += 0.140f; }
						else if (distanceToPacY > 0 && ghostPositions[i].y != -1.47f) { ghostPositions[i].y -= 0.140f; }
					}
				}
				countMovements[i] = 0;
			}
			++countMovements[i];

			float timeValue = glfwGetTime() * 2.0f;
			float firstValue = (sin(timeValue) / 2.0f) + 0.5f;
			int vertexColorLocation = glGetUniformLocation(shaderProgram, "uniformColor");
			glUniform4f(vertexColorLocation, 0.2f, firstValue, 0.5f, 1.0f);

			glUniform1i(object_type_loc, 5);
			glm::mat4 model_matrixGhost;
			glm::mat4 identityGhost = glm::mat4(1.0f);
			glm::mat4 translateGhost = glm::translate(model_matrixGhost, ghostPositions[i]);
			glm::mat4 scaleGhost = glm::scale(model_matrixGhost, glm::vec3(sphere_scale * 2.0f));
			model_matrixGhost = translateGhost * scaleGhost * identityGhost;
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrixGhost));
			glBindVertexArray(VAO_ghost);
			if (renderKey == 0) { glDrawArrays(GL_TRIANGLES, 0, vertices3.size()); }
			else if (renderKey == 1) { glDrawArrays(GL_POINTS, 0, vertices3.size()); }
			else if (renderKey == 2) { glDrawArrays(GL_LINES, 0, vertices3.size()); }
			glBindVertexArray(0);
		}

		//cout << "pacman position is " << pacPosition.x << pacPosition.y << endl;
		for (unsigned int i = 0; i <= numberFood - 1; i++)
		{
			if (drawFood[i] == true)
			{
				//cout << "position of object " << i << " is " << foodPositions[i].x << " and " << foodPositions[i].y << endl;
				markX = abs(foodPositions[i].x - pacPosition.x); markY = abs(foodPositions[i].y - pacPosition.y);
				if (markX <= 0.001f && markY <= 0.001f) { cout << "true" << endl; drawFood[i] = false; ate++; }
				else 
				{
					glUniform1i(object_type_loc, 4);
					glm::mat4 identityMatrix = glm::mat4(1.0f);
					glm::mat4 model_matrix2;
					glm::mat4 translatedM = glm::translate(model_matrix2, foodPositions[i]);
					glm::mat4 scaledM = glm::scale(model_matrix2, glm::vec3(sphere_scale));
					model_matrix2 = translatedM * scaledM * identityMatrix; //target = t*SCALE*R*m1 
					glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix2));
					glBindVertexArray(VAO_sphere);
					if (renderKey == 0) { glDrawArrays(GL_TRIANGLES, 0, vertices2.size()); }
					else if (renderKey == 1) { glDrawArrays(GL_POINTS, 0, vertices2.size()); }
					else if (renderKey == 2) { glDrawArrays(GL_LINES, 0, vertices2.size()); }
					glBindVertexArray(0);
				}
			}
			if (ate == numberFood) { restartGame(); }
		} 

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (X + 0.140f <= 1.47f) {
			X += 0.140f;}			//add rotation for orientation change
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (X - 0.140f >= -1.47f) {
			X -= 0.140f;}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (Y + 0.140f <= 1.47f) {
			Y += 0.140f;}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (Y - 0.140f >= -1.47f) {
			Y -= 0.140f;}
	}

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		triangle_scale += 0.001;
		sphere_scale += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		triangle_scale -= 0.001;
		sphere_scale -= 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camX += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camX -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camY += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camY -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		renderKey = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		renderKey = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		renderKey = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		X = getXPoint(); Y = getYPoint();
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		restartGame();
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

//callback function for whenever the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void restartGame()
{
	ate = 0;
	X = getXPoint(); Y = getYPoint();
	numberFood = rand() % 15 + 1;
	for (int i = 0; i <= numberFood - 1; i++)
	{
		foodPositions[i] = { getXPoint(), getYPoint(), 0.0f };
		drawFood[i] = true;
		if (i <= 3){ ghostPositions[i] = { getXPoint(), getYPoint(), 0.0f }; }
	}
}

float getYPoint()
{
	int ySection = rand() % 22 + 1;
	switch ( ySection)
	{
	case 1: return -1.47f; break;
	case 2: return -1.33f; break;
	case 3: return -1.19f; break;
	case 4: return -1.05f; break;
	case 5: return -0.91f; break;
	case 6: return -0.77f;	break;
	case 7: return -0.63f; break;
	case 8: return -0.49f; break;
	case 9: return -0.35f; break;
	case 10:return -0.21f; break;
	case 11:return -0.07f;	 break;
	case 12:return 0.07f;	 break;
	case 13:return 0.21f;	 break;
	case 14:return 0.35f;	 break;
	case 15:return 0.49f;	 break;
	case 16:return 0.63f;	 break;
	case 17:return 0.77f;	 break;
	case 18:return 0.91f;	 break;
	case 19:return 1.05f;	 break;
	case 20:return 1.19f;	 break;
	case 21:return 1.33f; break;
	case 22:return 1.47;	 break;
	default: return -0.07f; break;
	}
}

float getXPoint()
{
	int xSection = rand() % 22 + 1;
	switch (xSection)
	{
	case 1: return -1.47f; break;
	case 2: return -1.33f; break;
	case 3: return -1.19f; break;
	case 4: return -1.05f; break;
	case 5: return -0.91f; break;
	case 6: return -0.77f;	break;
	case 7: return -0.63f; break;
	case 8: return -0.49f; break;
	case 9: return -0.35f; break;
	case 10:return -0.21f; break;
	case 11:return -0.07f;	 break;
	case 12:return 0.07f;	 break;
	case 13:return 0.21f;	 break;
	case 14:return 0.35f;	 break;
	case 15:return 0.49f;	 break;
	case 16:return 0.63f;	 break;
	case 17:return 0.77f;	 break;
	case 18:return 0.91f;	 break;
	case 19:return 1.05f;	 break;
	case 20:return 1.19f;	 break;
	case 21:return 1.33f; break;
	case 22:return 1.47;	 break;
	default: return -0.07f; break;
	}
}
