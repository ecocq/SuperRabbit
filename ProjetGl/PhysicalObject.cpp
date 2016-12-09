// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include "PhysicalObject.h"
#include "ObjParser.h"
#include "transformation_mat.h"


PhysicalObject::PhysicalObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow)
{
	ObjPath = path;
	m_color = objcolor;
	fragmentShader = fragShader;
	window = Objwindow;
	position = glm::vec3(0, 0, 0);
}

PhysicalObject::PhysicalObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, glm::vec3 initialPos) : PhysicalObject(path, objcolor, fragShader, Objwindow)
{
	position = initialPos;
}


PhysicalObject::PhysicalObject(std::vector<glm::vec4> _geometric_vertex, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow) : PhysicalObject("NONE", objcolor, fragShader, Objwindow)
{
	geometric_vertex = _geometric_vertex;
}

void PhysicalObject::fix_vertex() {
	for (int i = 0; i < geometric_vertex.size(); i++) {
		geometric_vertex[i] = ModelMatrix * geometric_vertex[i];
	}

}

int PhysicalObject::initialize() {

	if (ObjPath != "NONE" && !loadObjFile(ObjPath, geometric_vertex, texture_coords, vertex_normals)) {
		fprintf(stderr, "Failed to load obj\n");
		getchar();
		return -1;
	}
	else if (ObjPath == "NONE") {

	}
	if (position != glm::vec3(0, 0, 0)) {
		ModelMatrix = glm::mat4(1.0) * translation(position);
		fix_vertex();
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

	ModelMatrix = glm::mat4(1.0);
	translated = glm::vec3(0);

	return 0;
}

int PhysicalObject::execute() {
	glUniform3fv(fragmentShader, 1, &m_color[0]);

	// Compute the model matrix from keyboard and mouse input
	applyTransformsFromControls();

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
	if (ModelMatrix != glm::mat4(1.0)) {
		fix_vertex();
	}


	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, geometric_vertex.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	ModelMatrix = glm::mat4(1.0);

	return 0;
}


PhysicalObject::~PhysicalObject()
{
	glDeleteBuffers(1, &vertexbuffer);
	if (textures_coords_valid) {
		glDeleteBuffers(1, &uvbuffer);
	}
}

/* Init transforms  */
void PhysicalObject::initTransforms(glm::vec3 translate, glm::vec3 rotate) {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, geometric_vertex.size() * sizeof(glm::vec4), &geometric_vertex[0], GL_STATIC_DRAW);

	if (rotate != glm::vec3(0, 0, 0)) {
		if (rotate.x != 0) {
			ModelMatrix = ModelMatrix * rotation_x(rotate.x);
		}
		if (rotate.y != 0) {
			ModelMatrix = ModelMatrix * rotation_y(rotate.y);
		}
		if (rotate.z != 0) {
			ModelMatrix = ModelMatrix * rotation_z(rotate.z);
		}
	}
	if (translate != glm::vec3(0, 0, 0)) {
		applyTranslation(translate);
	}
	//Apply transformations on all points
	for (int i = 0; i < geometric_vertex.size(); i++) {
		geometric_vertex[i] = ModelMatrix * geometric_vertex[i];
	}
	glDrawArrays(GL_TRIANGLES, 0, geometric_vertex.size());

	glDisableVertexAttribArray(0);

	ModelMatrix = glm::mat4(1.0);
}

/* This function will be a default control transforms function
We should use inheritance to redefine according to objects behaviors */
void PhysicalObject::applyTransformsFromControls() {

}


/* --- Apply Transformations --- */

void PhysicalObject::applyTranslation(glm::vec3 trans) {
	translated = translated + trans;
	std::cout << translated.x << "," << translated.y << "," << translated.z << std::endl;
	ModelMatrix = ModelMatrix * translation(trans);
}

void PhysicalObject::applyRotation(float angle_x, float angle_y, float angle_z) {
	glm::mat4 rotationMatrix(1.0);
	if (angle_x != 0) {
		rotationMatrix = rotationMatrix * rotation_x(angle_x);
	}
	if (angle_y != 0) {
		rotationMatrix = rotationMatrix * rotation_y(angle_y);
	}
	if (angle_z != 0) {
		rotationMatrix = rotationMatrix * rotation_z(angle_z);
	}

	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * rotationMatrix * translation(-pos);
}

void PhysicalObject::applyRotationAroundAxis(float angle_d, glm::vec3 vect) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * rotation_around_axis(angle_d, vect) * translation(-pos);
}

void PhysicalObject::applyScale(glm::vec3 vector) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * scale(vector) * translation(-pos);
}

void PhysicalObject::applyScaleAlongAxis(float k, glm::vec3 axis) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * scale_along_axis(k, axis) * translation(-pos);
}

void PhysicalObject::applyOrthographicProjection(glm::vec3 axis) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * orthographic_projection(axis) * translation(-pos);
}

void PhysicalObject::applyReflection(glm::vec3 axis) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * reflection(axis) * translation(-pos);
}
void PhysicalObject::applyShearOrNot(bool unshear) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * shear(unshear) * translation(-pos);
}

void PhysicalObject::applyShearingXY(float s, float t) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * shearing_xy(s, t) * translation(-pos);
}

void PhysicalObject::applyShearingXZ(float s, float t) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * shearing_xz(s, t) * translation(-pos);
}

void PhysicalObject::applyShearingYZ(float s, float t) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * translation(pos) * shearing_yz(s, t) * translation(-pos);
}