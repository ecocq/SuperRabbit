#pragma once
#include "transformation_mat.h"

class PhysicalObject
{
private:
	GLFWwindow* window;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;

	std::vector<glm::vec4> geometric_vertex;
	std::vector<glm::vec3> texture_coords;
	std::vector<glm::vec3> vertex_normals;

	glm::mat4 ModelMatrix;

	bool textures_coords_valid;
	bool normals_valid;

	const char* ObjPath;

public:
	PhysicalObject(const char* path, GLFWwindow* Objwindow);

	int initialize();
	int execute();
	virtual void applyTransformsFromControls();

	~PhysicalObject();
};

