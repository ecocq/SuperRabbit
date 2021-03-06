// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
static glm::mat4 MVP;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/text2D.hpp>
#include "Camera.h"
#include "ObjParser.h"
#include "PhysicalObject.h"
#include "MovableObject.h"
#include "transformation_mat.h"
#include "Wall.h"



int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Super Rabbit", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("shader/TransformVertexShader.vertexshader", "shader/Color.fragmentshader");
	

	// Get a handle for our "MVP" and "View"  uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewID = glGetUniformLocation(programID, "View_matrix");


	// Get a handle for our "fragmentColor" uniform
	GLuint fragColor = glGetUniformLocation(programID, "m_fragColor");

	//Init objects
	std::vector<PhysicalObject*> objects;
	MovableObject* m_rabbit = new MovableObject("obj/Rabbit.obj", glm::vec3(1.0f, 1.0f, 1.0f), fragColor, window, programID, glm::vec3(2, -2.9, 0));
	PhysicalObject* carrot = new PhysicalObject("obj/carrot.obj", glm::vec3(1.0f, 0.6f, 0.03f), fragColor, window, programID, translation(glm::vec3(1.5, -2, 1.5)) * rotation_x(90));
	objects.push_back(m_rabbit);
	objects.push_back(carrot);

	// Walls and obstacles
	glm::vec3 wallColor(0.5f, 0.5f, 0.5f);
	objects.push_back(new Wall(glm::vec2(40, 5), glm::vec3(-3, -3, -1), glm::vec3(0, 0, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(40, 5), glm::vec3(-3, -3, -1), glm::vec3(-90, 0, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(40, 5), glm::vec3(-3, -3, 4), glm::vec3(0, 0, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(40, 5), glm::vec3(-3, 2, -1), glm::vec3(-90, 0, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(5, 5), glm::vec3(-3, -3, -1), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(5, 5), glm::vec3(37, -3, -1), glm::vec3(0, 90, 0), glm::vec3(1, 0.5f, 0.5f), fragColor, window, programID));

	// Obstacle 1
	wallColor = glm::vec3(0.5f, 1, 0.5f);
	objects.push_back(new Wall(glm::vec2(2, 5), glm::vec3(5, -3, -1), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(2, 5), glm::vec3(5, -3, 2), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(1, 3), glm::vec3(5, -3, 1), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	
	// Obstacle Scale
	wallColor = glm::vec3(0.5f, 0.5f, 1);
	objects.push_back(new Wall(glm::vec2(1.4f, 5.0f), glm::vec3(10, -3, -1), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(3.4f, 5.0f), glm::vec3(10, -3, 0.6f), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(0.2f, 4.8f), glm::vec3(10.0f, -2.8f, 0.4f), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	
	// Obstacle Rotation
	wallColor = glm::vec3(0.5f, 1, 0.5f);
	objects.push_back(new Wall(glm::vec2(5, 1), glm::vec3(15, 1, -1), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(5, 3), glm::vec3(15, -3, -1), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(1, 1), glm::vec3(15, 0, -1), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(2, 1), glm::vec3(15, 0, 2), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	
	// Obstacle Shear
	wallColor = glm::vec3(0.5f, 0.5f, 1);
	objects.push_back(new Wall(glm::vec2(1, 5), glm::vec3(20, -3, -1), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(4, 3.3f), glm::vec3(20, -1.3f, 0), glm::vec3(0, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(3, 3), glm::vec3(20, -3, 0), glm::vec3(45, 90, 0), wallColor, fragColor, window, programID));
	objects.push_back(new Wall(glm::vec2(3, 4), glm::vec3(20, -3, 1.2f), glm::vec3(-45, 90, 0), wallColor, fragColor, window, programID));

	m_rabbit->setObjects(objects);

	initText2D("Holstein.DDS");

	//Init camera
	Camera* cam = new Camera();
	cam->execute(window);
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 MVP;

	cam->execute(window);
	ProjectionMatrix = cam->getProjection();
	ViewMatrix = cam->getCamView();
	MVP = ProjectionMatrix * ViewMatrix * glm::mat4(1.0);

	for (uint i = 0; i < objects.size(); i++)
	{
		objects[i]->initialize(MVP);
	}

	std::clock_t start = std::clock();

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		//Cam
		cam->execute(window);
		ProjectionMatrix = cam->getProjection();
		ViewMatrix = cam->getCamView();
		MVP = ProjectionMatrix * ViewMatrix * glm::mat4(1.0);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ViewID, 1, GL_FALSE, &ViewMatrix[0][0]);

		for (uint i = 0; i < objects.size(); i++)
		{
			objects[i]->execute(MVP);
		}

		printText2D("IJKL UO move - PM up/down - YH bend over/back", 45, 40, 15);
		printText2D(",; rotate - wheel scale - 34 shear - 5 reflexion", 30, 20, 15);

		m_rabbit->printIfReached(glm::vec3(22.0f, -3.0f, -1.0f), "You win!");
		carrot->animateTrans(glm::vec3(30.0f, 0.0f, 0.0f), (std::clock() - start) / (double)CLOCKS_PER_SEC);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	
	for (uint i = 0; i < objects.size(); i++)
	{
		delete(objects[i]);
	}
	objects.clear();

	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
