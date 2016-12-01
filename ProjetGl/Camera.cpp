#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"


Camera::Camera()
{
	position = glm::vec3(0, 0 , 0);
	speed = 0.1f;
}

void Camera::execute(GLFWwindow *window)
{
	glm::vec3 cartesian_dir = glm::vec3(1, 0, 0);
	glm::vec3 cartesian_dir_right = glm::vec3(0, 0, 1);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += cartesian_dir * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= cartesian_dir * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += cartesian_dir_right * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= cartesian_dir_right * speed;
	}

	// refaire lookAt
	CamViewMatrice = glm::lookAt(
		position,           // Camera is here
		position + cartesian_dir, // and looks here : at the same position, plus "direction"
		glm::vec3(0,1,0)                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// refaire perspective
						   // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

}

Camera::~Camera()
{

}
