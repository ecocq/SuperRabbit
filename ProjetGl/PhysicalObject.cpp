// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include <common/text2D.hpp>
#include "PhysicalObject.h"
#include "ObjParser.h"
#include "transformation_mat.h"


PhysicalObject::PhysicalObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID, glm::mat4 initialTrans)
{
	ObjPath = path;
	m_color = objcolor;
	fragmentShader = fragShader;
	window = Objwindow;
	position = glm::vec3(0, 0, 0);
	ModelMatrix = initialTrans;	
	CompleteModelMatrix = glm::mat4(1.0);
	this->programID = programID;
}

PhysicalObject::PhysicalObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID, glm::vec3 initialPos) : PhysicalObject(path, objcolor, fragShader, Objwindow, programID, translation(initialPos))
{
	position = initialPos;
}

void PhysicalObject::colliderTrans()
{
}

PhysicalObject::PhysicalObject(std::vector<glm::vec4> _geometric_vertex, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID) : PhysicalObject("NONE", objcolor, fragShader, Objwindow, programID)
{
	geometric_vertex = _geometric_vertex;
}

void PhysicalObject::fix_vertex(glm::mat4 MVP) {

	m_OBB.transform(CompleteModelMatrix);

	colliderTrans();

	GLint MVPHandle = glGetUniformLocation(programID, "MVP");
	GLint ViewID = glGetUniformLocation(programID, "model");
	CompleteModelMatrix = CompleteModelMatrix * ModelMatrix;
	glm::mat4 MVPMatrix = MVP * CompleteModelMatrix;
	glUniformMatrix4fv(MVPHandle, 1, GL_FALSE, &MVPMatrix[0][0]);
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &CompleteModelMatrix[0][0]);
}

int PhysicalObject::initialize(glm::mat4 MVP) {
	extremum _extremum;

	if (ObjPath != "NONE" && !loadObjFile(ObjPath, geometric_vertex, texture_coords, vertex_normals, _extremum)) {
		fprintf(stderr, "Failed to load obj\n");
		getchar();
		return -1;
	}
	else if (ObjPath == "NONE") {

	}

	m_OBB = OBB(glm::vec3(0, 0, 0), _extremum);

	fix_vertex(MVP);


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

int PhysicalObject::execute(glm::mat4 MVP) {
	glUniform3fv(fragmentShader, 1, &m_color[0]);

	// Compute the model matrix from keyboard and mouse input
	applyTransformsFromControls();

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
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
	fix_vertex(MVP);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, geometric_vertex.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	m_OBB.execute();

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


/* This function will be a default control transforms function
We should use inheritance to redefine according to objects behaviors */
void PhysicalObject::applyTransformsFromControls() {

}

void PhysicalObject::printIfReached(glm::vec3 direction, char* text) {
	if (CompleteModelMatrix[3][0] > direction.x && CompleteModelMatrix[3][1] > direction.y && CompleteModelMatrix[3][2] > direction.z) {
		printText2D(text, 170, 270, 50);
	}
}

/* --- Apply Transformations --- */

void PhysicalObject::applyTranslation(glm::vec3 trans) {
	translated = translated + trans;

	ModelMatrix = ModelMatrix * translation(trans);
}

void PhysicalObject::animateTrans(glm::vec3 direction, double duration) {
	if(translated.x < direction.x) {
		if(duration > 5.0f){
			applyTranslation((direction - position) * 0.01);
		}

		printText2D("Let's find", 160, 300, 50);
		printText2D("the carrot!", 140, 250, 50);
	}
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
	ModelMatrix = ModelMatrix * rotationMatrix;
}

void PhysicalObject::applyRotationAroundAxis(float angle_d, glm::vec3 vect) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * rotation_around_axis(angle_d, vect);
}

void PhysicalObject::applyScale(glm::vec3 vector) {
	float pos_x = (CompleteModelMatrix * glm::vec4(0, 0, 0, 1)).x;
	if (pos_x >= 5.0f && pos_x <= 11.5f) {
		if (vector.x < 1 && scale_factor < 0.13f) {
			return;
		}
		ModelMatrix = scale(vector);
		scale_old = scale_factor;
		scale_factor *= vector.x;
	} 
}

void PhysicalObject::applyScaleAlongAxis(float k, glm::vec3 axis) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * scale_along_axis(k, axis);
}

void PhysicalObject::applyOrthographicProjection(glm::vec3 axis) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * orthographic_projection(axis);
}

void PhysicalObject::applyReflection(glm::vec3 axis) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * reflection(axis);
}
void PhysicalObject::applyShearOrNot(bool unshear) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * shear(unshear);
}

void PhysicalObject::applyShearingXY(float s, float t) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * shearing_xy(s, t);
}

void PhysicalObject::applyShearingXZ(float s, float t) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * shearing_xz(s, t);
}

void PhysicalObject::applyShearingYZ(float s, float t) {
	glm::vec3 pos = position + translated;
	ModelMatrix = ModelMatrix * shearing_yz(s, t);
}