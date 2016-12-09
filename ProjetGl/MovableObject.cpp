// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include "PhysicalObject.h"

#include "MovableObject.h"

MovableObject::MovableObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow) : PhysicalObject(path, objcolor, fragShader, Objwindow) { }

void MovableObject::applyTransformsFromControls() {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		applyRotation(5, 0, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		applyRotation(0, 2, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, 0, 0.2));
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, -0.2, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, 0, -0.2));
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, 0.2, 0));
	}

	//Apply when click and move mouse?
	//applyRotationAroundAxis

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		applyScale(glm::vec3(1.2, 1.2, 1.2));
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		applyScale(glm::vec3(0.8, 0.8, 0.8));
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		applyReflection(glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		applyShearingYZ(1.2, -0.5);
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		applyShearingYZ(-1.2, 0.5);
	}

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		applyShearOrNot(true);
	}

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		applyShearOrNot(false);
	}
}
