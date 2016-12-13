// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "PhysicalObject.h"

#include "MovableObject.h"
static int wheel;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	wheel = yoffset;
}

MovableObject::MovableObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID) : PhysicalObject(path, objcolor, fragShader, Objwindow, programID) {
	glfwSetScrollCallback(window, scroll_callback);
}

void MovableObject::fix_vertex(glm::mat4 MVP) {
	m_OBB.transform(CompleteModelMatrix * ModelMatrix);

	// Ignore first object
	for (int i = 1; i < m_objects.size(); i++) {
		if (m_OBB.collides(m_objects[i]->m_OBB)) {
			m_OBB.restore();
			translated = translated_old;
			ModelMatrix = glm::mat4(1.0);
			return;
		}
	}

	GLint MVPHandle = glGetUniformLocation(programID, "MVP");
	CompleteModelMatrix = CompleteModelMatrix * ModelMatrix;
	glm::mat4 MVPMatrix = MVP * CompleteModelMatrix;
	glUniformMatrix4fv(MVPHandle, 1, GL_FALSE, &MVPMatrix[0][0]);
}

void MovableObject::setObjects(std::vector<PhysicalObject*> _objects) {
	m_objects = _objects;
}


void MovableObject::applyTransformsFromControls() {
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		applyTranslation(glm::vec3(-1, 0, 0)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		applyTranslation(glm::vec3(1, 0, 0)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, 0, -1)* speed);
	}else if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, 0, 1)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, 1, 0)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, -1, 0)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		applyRotation(0, 0.1, 0);
	}else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		applyRotation(0, -0.1, 0);
	}else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		applyRotation(0, 0, -0.1);
	}else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		applyRotation(0, 0, 0.1);
	}else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		applyRotation(0.1, 0, 0);
	}else if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
		applyRotation(-0.1, 0, 0);
	}

	if (wheel != 0) {
		if (wheel > 0 && scale > -20) {
			scale--;
			applyScale(glm::vec3(1.2, 1.2, 1.2));
		}
		else if (wheel < 0 && scale < 20) {
			scale++;
			this->applyScale(glm::vec3(0.8, 0.8, 0.8));
		}
		wheel = 0;
	}

	//Apply when click and move mouse?
	//applyRotationAroundAxis

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		applyScale(glm::vec3(1.1, 1.1, 1.1));
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		applyScale(glm::vec3(0.9, 0.9, 0.9));
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
