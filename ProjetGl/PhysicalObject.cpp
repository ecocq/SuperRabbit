// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include <vector>

#include "PhysicalObject.h"
#include "ObjParser.h"

PhysicalObject::PhysicalObject(const char* path, GLFWwindow* Objwindow)
{
	ObjPath = path;
	window = Objwindow;
}

int PhysicalObject::initialize() {

	//"obj/Rabbit.obj"
	if (!loadObjFile(ObjPath, geometric_vertex, texture_coords, vertex_normals)) {
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

	ModelMatrix = glm::mat4(1.0);
	translated = glm::vec3(0);
	
	return 0;
}

int PhysicalObject::execute() {
	
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
		for (int i = 0; i < geometric_vertex.size(); i++) {
			geometric_vertex[i] = geometric_vertex[i] * ModelMatrix;
		}
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

/* Init transforms - put null if not needed */
void PhysicalObject::initTransforms(glm::vec3 translate, glm::vec3 rotate) {
	glEnableVertexAttribArray(0);
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
		geometric_vertex[i] = geometric_vertex[i] * ModelMatrix;
	}
	glDrawArrays(GL_TRIANGLES, 0, geometric_vertex.size());

	glDisableVertexAttribArray(0);

	ModelMatrix = glm::mat4(1.0);
}

/* This function will be a default control transforms function
We should use inheritance to redefine according to objects behaviors */
void PhysicalObject::applyTransformsFromControls() {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		applyRotation(5, 0, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			applyRotation(0, 2, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		applyTranslation(glm::vec3(-0.2,0,0));
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0, -0.2, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		applyTranslation(glm::vec3(0.2, 0, 0));
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


/* --- Apply Transformations --- */

void PhysicalObject::applyTranslation(glm::vec3 trans) {
	translated = translated + trans;
	std::cout << translated.x << "," << translated.y << "," << translated.z << std::endl;
	ModelMatrix = ModelMatrix * translation(trans);
}

void PhysicalObject::applyRotation(float angle_x, float angle_y, float angle_z) {
	/*
	std::cout << "Avant trans : " << geometric_vertex[2][2] << std::endl;
	ModelMatrix = translation(-translated);
	for (int i = 0; i < geometric_vertex.size(); i++) {
		geometric_vertex[i] = translation(-translated) * geometric_vertex[i];
	}
	std::cout << "Apres trans " << geometric_vertex[2][2] << std::endl;*/
	if (angle_x != 0) {
		ModelMatrix = ModelMatrix * rotation_x(angle_x);
	}
	if (angle_y != 0) {
		ModelMatrix = ModelMatrix * rotation_y(angle_y);
	}
	if (angle_z != 0) {
		ModelMatrix = ModelMatrix * rotation_z(angle_z);
	}/*
	for (int i = 0; i < geometric_vertex.size(); i++) {
		geometric_vertex[i] = geometric_vertex[i] * ModelMatrix;
	}
	std::cout << "Apres Modele" << geometric_vertex[2][2] << std::endl;
	for (int i = 0; i < geometric_vertex.size(); i++) {
		geometric_vertex[i] = translation(-translated) * geometric_vertex[i];
	}
	std::cout << "Après trans" << geometric_vertex[2][2] << std::endl;*/
}

void PhysicalObject::applyRotationAroundAxis(float angle_d, glm::vec3 vect) {
	ModelMatrix = ModelMatrix * rotation_around_axis(angle_d, vect);
}

void PhysicalObject::applyScale(glm::vec3 vector) {
	ModelMatrix = ModelMatrix * scale(vector);
}

void PhysicalObject::applyScaleAlongAxis(float k, glm::vec3 axis) {
	ModelMatrix = ModelMatrix * scale_along_axis(k, axis);
}

void PhysicalObject::applyOrthographicProjection(glm::vec3 axis) {
	ModelMatrix = ModelMatrix * orthographic_projection(axis);
}

void PhysicalObject::applyReflection(glm::vec3 axis) {
	ModelMatrix = ModelMatrix * reflection(axis);
}
void PhysicalObject::applyShearOrNot(bool unshear) {
	ModelMatrix = ModelMatrix * shear(unshear);
}

void PhysicalObject::applyShearingXY(float s, float t) {
	ModelMatrix = ModelMatrix * shearing_xy(s, t);
}

void PhysicalObject::applyShearingXZ(float s, float t) {
	ModelMatrix = ModelMatrix * shearing_xz(s, t);
}

void PhysicalObject::applyShearingYZ(float s, float t) {
	ModelMatrix = ModelMatrix * shearing_yz(s, t);
}