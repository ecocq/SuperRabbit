#pragma once
#include <vector>
#include "OBB.h"

class PhysicalObject
{
protected:
	GLFWwindow* window;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint fragmentShader;
	GLint programID;

	std::vector<glm::vec4> geometric_vertex;
	std::vector<glm::vec3> texture_coords;
	std::vector<glm::vec3> vertex_normals;
	glm::vec3 m_color;

	glm::vec3 position;

	glm::mat4 ModelMatrix;
	glm::mat4 CompleteModelMatrix;
	glm::vec3 translated;
	glm::vec3 translated_old;

	bool textures_coords_valid;
	bool normals_valid;

	const char* ObjPath;
	float speed = 0.02f;

public:
	PhysicalObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID, glm::mat4 initialPos = glm::mat4(1.0));
	PhysicalObject(std::vector<glm::vec4> geometric_vertex, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID);
	PhysicalObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID, glm::vec3 initialPos);

	int initialize(glm::mat4 MVP);
	int execute(glm::mat4 MVP);
	void printIfReached(glm::vec3 direction, char* text);
	void animateTrans(glm::vec3 direction, double duration);
	virtual void fix_vertex(glm::mat4 MVP);
	virtual void applyTransformsFromControls();
	void colliderTrans();
	void applyTranslation(glm::vec3 trans);
	void applyRotation(float angle_x, float angle_y, float angle_z);
	void applyRotationAroundAxis(float angle_d, glm::vec3 vect);
	void applyScale(glm::vec3 vector);
	void applyScaleAlongAxis(float k, glm::vec3 axis);
	void applyOrthographicProjection(glm::vec3 axis);
	void applyReflection(glm::vec3 axis);
	void applyShearOrNot(bool unshear);
	void applyShearingXY(float s, float t);
	void applyShearingXZ(float s, float t);
	void applyShearingYZ(float s, float t);

	OBB m_OBB;

	~PhysicalObject();
};

