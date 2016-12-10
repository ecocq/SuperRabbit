#pragma once

#ifndef __gl_h_ 

#include "glm\glm.hpp"
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

#endif

#include <vector>
#include <iostream>
#include "glm/ext.hpp"

class OBB
{
public:
	OBB() {};
	OBB(glm::mat4 matrix, glm::vec3 extends);
	~OBB();

	bool collides(OBB &other);
	void execute();
	void setMatrix(glm::mat4 _matrix);

private:
	void GetInvRot(glm::vec3 *pvRot);
	glm::vec3 GetCenterPoint();
	void initializePosition();

	glm::vec3 m_extent;
	glm::mat4 m_matrix;

	GLuint vertexbuffer;

	std::vector<glm::vec4> geometric_vertex;
};
