#pragma once
class PhysicalObject
{
private:
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;

	std::vector<glm::vec4> geometric_vertex;
	std::vector<glm::vec3> texture_coords;
	std::vector<glm::vec3> vertex_normals;

	bool textures_coords_valid;
	bool normals_valid;

	const char* ObjName;

public:
	PhysicalObject(const char* objName);

	int initialize();
	int execute(glm::mat4 ModelMatrix);

	~PhysicalObject();
};

