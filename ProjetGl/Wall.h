#pragma once
#include "PhysicalObject.h"
class Wall :
	public PhysicalObject
{
public:
	Wall(glm::vec2 size, glm::vec3 pos, glm::vec3 rot, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID);
	~Wall();
};

