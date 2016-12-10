#include <glfw3.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "transformation_mat.h"
#include "Camera.h"


Camera::Camera()
{
	position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	speed = 0.1f;
	angle = 0.0f;
}

void Camera::execute(GLFWwindow *window)
{
	glm::vec3 cartesian_dir =  rotation_y(angle) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 cartesian_dir_right =  rotation_y(angle) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position = translation(cartesian_dir * speed) * position;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position = translation(-cartesian_dir * speed) * position;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position = translation(cartesian_dir_right * speed) * position;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position = translation(-cartesian_dir_right * speed) * position;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		angle += 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		angle -= 1.0f;
	}

	CamViewMatrice = glm::lookAt(glm::vec3(position), glm::vec3(position) + cartesian_dir, glm::vec3(0,1,0));

	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

}

Camera::~Camera()
{

}
