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
#include "transformation_mat.h"

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	std::vector<glm::vec4> geometric_vertex;
	std::vector<glm::vec3> texture_coords;
	std::vector<glm::vec3> vertex_normals;

	if (!loadObjFile("obj/Rabbit.obj", geometric_vertex, texture_coords, vertex_normals)) {
		fprintf(stderr, "Failed to load obj\n");
		getchar();
		return -1;
	}

	std::cout << geometric_vertex.size() << std::endl;
	std::cout << texture_coords.size() << std::endl;
	std::cout << vertex_normals.size() << std::endl;


	bool textures_coords_valid = (texture_coords.size() > 0 ? true : false );
	bool normals_valid = (vertex_normals.size() > 0 ? true : false);

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
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Load the texture
	GLuint Texture = loadDDS("obj/wolf-obj.mtl");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, geometric_vertex.size() * sizeof(glm::vec4), &geometric_vertex[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	if (textures_coords_valid) {
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, texture_coords.size() * sizeof(glm::vec3), &texture_coords[0], GL_STATIC_DRAW);
	}

	GLuint normalbuffer;
	if (normals_valid) {
		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_normals.size() * sizeof(glm::vec3), &vertex_normals[0], GL_STATIC_DRAW);
	}

	//Init camera
	Camera* cam = new Camera();
	cam->execute(window);
	glm::mat4 ProjectionMatrix = cam->getProjection();
	glm::mat4 ViewMatrix = cam->getCamView();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;


	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		//Cam
		cam->execute(window);
		//TODO see how to replace this code to work well (cam + transformations):
		/*ProjectionMatrix = cam->getProjection();
		ViewMatrix = cam->getCamView();
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;*/

		// Compute the MVP matrix from keyboard and mouse input
		//Apply transformations -- to put in another function after testing ;)
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			applyRotation(MVP, 5, 0,0);
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			applyRotation(MVP, 0, 2, 0);
		}


		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		if (textures_coords_valid) {
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
			glVertexAttribPointer(
				1,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
		}

		if (normals_valid) {
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
			glVertexAttribPointer(
				2,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
		}


		//Apply transformations on all points
		for each(vec4 point in geometric_vertex) {
			point = point * MVP;
		}

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, geometric_vertex.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	if (textures_coords_valid) {
		glDeleteBuffers(1, &uvbuffer);
	}
	
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
