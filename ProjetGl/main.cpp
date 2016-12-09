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

#include <common/shader.hpp>
#include <common/texture.hpp>
#include "Camera.h"
#include "ObjParser.h"
#include "PhysicalObject.h"
#include "MovableObject.h"

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
	window = glfwCreateWindow(1024, 768, "Tutorial 07 - Model Loading", NULL, NULL);
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

	// Load the texture
	//---------------------------------------- TODO REMOVE ????
	GLuint Texture = loadDDS("obj/wolf-obj.mtl");

	// Get a handle for our "myTextureSampler" uniform
	// GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Get a handle for our "fragmentColor" uniform
	GLuint fragColor = glGetUniformLocation(programID, "m_fragColor");

	//Init objects
	std::vector<PhysicalObject*> objects;
	PhysicalObject *obj_rabbit = new MovableObject("obj/Rabbit.obj", glm::vec3(1.0f, 0.0f, 0.0f), fragColor, window);
	PhysicalObject *wall = new PhysicalObject("obj/Rabbit.obj", glm::vec3(1.0f, 0.0f, 0.0f), fragColor, window);
	objects.push_back(obj_rabbit);
	objects.push_back(wall);

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->initialize();
	}

	//Init camera
	Camera* cam = new Camera();
	cam->execute(window);
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 MVP;


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

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		// glUniform1i(TextureID, 0);

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->execute();
		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	
	for (int i = 0; i < objects.size(); i++)
	{
		delete(objects[i]);
	}
	objects.clear();

	glDeleteProgram(programID);
	// glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
