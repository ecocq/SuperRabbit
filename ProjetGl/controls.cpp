#include "controls.h"

/* These controls must be changed to fit with our game later */

void applyTransformsFromControls(GLFWwindow* window, glm::mat4 &matrice)
{
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		applyRotation(matrice, 5, 0, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		applyRotation(matrice, 0, 2, 0);
	}

	//Apply when click and move mouse?
	//applyRotationAroundAxis

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		applyScale(matrice, glm::vec3(1.2,1.2,1.2));
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		applyScale(matrice, glm::vec3(0.8, 0.8, 0.8));
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		applyReflection(matrice, glm::vec3(0, 1, 0));
	}
	
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		applyShearingYZ(matrice, 1.2, -0.5);
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		applyShearingYZ(matrice, -1.2, 0.5);
	}

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		applyShearOrNot(matrice, true);
	}

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		applyShearOrNot(matrice, false);
	}
	


}
	