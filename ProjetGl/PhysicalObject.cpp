// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include <vector>

#include "PhysicalObject.h"
#include "ObjParser.h"

PhysicalObject::PhysicalObject(const char* objName)
{
	ObjName = objName;
}

int PhysicalObject::initialize() {

	//"obj/Rabbit.obj"
	if (!loadObjFile(ObjName, geometric_vertex, texture_coords, vertex_normals)) {
		fprintf(stderr, "Failed to load obj\n");
		getchar();
		return -1;
	}

	textures_coords_valid = (texture_coords.size() > 0 ? true : false);
	normals_valid = (vertex_normals.size() > 0 ? true : false);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, geometric_vertex.size() * sizeof(glm::vec4), &geometric_vertex[0], GL_STATIC_DRAW);

	if (textures_coords_valid) {
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, texture_coords.size() * sizeof(glm::vec3), &texture_coords[0], GL_STATIC_DRAW);
	}

	if (normals_valid) {
		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_normals.size() * sizeof(glm::vec3), &vertex_normals[0], GL_STATIC_DRAW);
	}
	
	return 0;
}

int PhysicalObject::execute(glm::mat4 ModelMatrix = glm::mat4(1.0)) {

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	if (ModelMatrix != glm::mat4(1.0)) {
		glBufferData(GL_ARRAY_BUFFER, geometric_vertex.size() * sizeof(glm::vec4), &geometric_vertex[0], GL_STATIC_DRAW);
	}
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
	for (int i = 0; i < geometric_vertex.size(); i++) {
		geometric_vertex[i] = geometric_vertex[i] * ModelMatrix;
	}

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, geometric_vertex.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	return 0;
}


PhysicalObject::~PhysicalObject()
{
	glDeleteBuffers(1, &vertexbuffer);
	if (textures_coords_valid) {
		glDeleteBuffers(1, &uvbuffer);
	}
}
