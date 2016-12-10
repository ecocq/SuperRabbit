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
#include "ObjParser.h"

class OBB
{
public:
	OBB() {};
	OBB(glm::vec3 center, extremum _extremum);
	~OBB();

	void transform(glm::mat4 _matrix);
	bool collides(const OBB &other);

	void execute();


private:
	void initializePosition();

	extremum m_extremum;
	glm::vec3 m_center;

	GLuint vertexbuffer;

	std::vector<glm::vec4> geometric_vertex;
	std::vector<glm::vec4> vertex_normals;
};
