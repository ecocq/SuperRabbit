// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "PhysicalObject.h"

#include "MovableObject.h"
static double wheel;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	wheel = yoffset;
}

MovableObject::MovableObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID) : PhysicalObject(path, objcolor, fragShader, Objwindow, programID) {
	glfwSetScrollCallback(window, scroll_callback);
}
MovableObject::MovableObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID, glm::vec3 initialPos) : PhysicalObject(path, objcolor, fragShader, Objwindow, programID, initialPos) { 
	glfwSetScrollCallback(window, scroll_callback);
}
void MovableObject::fix_vertex(glm::mat4 MVP) {
	m_OBB.transform(CompleteModelMatrix * ModelMatrix);

	// Ignore first object
	for (unsigned int i = 1; i < m_objects.size(); i++) {
		if (m_OBB.collides(m_objects[i]->m_OBB)) {
			ModelMatrix = glm::mat4(1.0);
			scale_factor = scale_old;
		}
	}
	float pos_x = (CompleteModelMatrix * glm::vec4(0, 0, 0, 1)).x;
	if (scale_factor != 1.0f && (pos_x < 5.0f || pos_x > 11.5f)) {
		ModelMatrix = scale(glm::vec3(1.0 / scale_factor, 1.0 / scale_factor, 1.0 / scale_factor));
		scale_factor = 1.0f;
		scale_old = 1.0f;
	}

	GLint MVPHandle = glGetUniformLocation(programID, "MVP");
	GLint ViewID = glGetUniformLocation(programID, "model");
	CompleteModelMatrix = CompleteModelMatrix * ModelMatrix;
	glm::mat4 MVPMatrix = MVP * CompleteModelMatrix;
	glUniformMatrix4fv(MVPHandle, 1, GL_FALSE, &MVPMatrix[0][0]);
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &CompleteModelMatrix[0][0]);
}

void MovableObject::setObjects(std::vector<PhysicalObject*> _objects) {
	m_objects = _objects;
}


void MovableObject::applyTransformsFromControls() {
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		applyTranslation(glm::vec3(-1, 0, 0)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		applyTranslation(glm::vec3(1, 0, 0)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, 0, -1)* speed);
	}else if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, 0, 1)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, 1, 0)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, -1, 0)* speed);
	}else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		applyRotation(0, angularSpeed, 0);
	}else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		applyRotation(0, -angularSpeed, 0);
	}else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		applyRotation(0, 0, -angularSpeed);
	}else if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
		applyRotation(0, 0, angularSpeed);
	}else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		applyRotation(angularSpeed, 0, 0);
	}else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		applyRotation(-angularSpeed, 0, 0);
	}

	if (wheel != 0) {
		if (wheel > 0) {
			this->applyScale(glm::vec3(1.25, 1.25, 1.25));
		}
		else if (wheel < 0) {
			this->applyScale(glm::vec3(0.8, 0.8, 0.8));
		}
		wheel = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		applyScale(glm::vec3(1.25, 1.25, 1.25));
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		applyScale(glm::vec3(0.8, 0.8, 0.8));
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		applyShearingYZ(shearSpeed, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		applyShearingYZ(-shearSpeed, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		applyReflection(glm::vec3(0, 1, 0));
	}

	//Not useful in the game
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		applyOrthographicProjection(glm::vec3(0, 1, 0));
	}
	
}
