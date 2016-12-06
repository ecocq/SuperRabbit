#pragma once
class PhysicalObject
{
private:
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint fragmentShader;

	std::vector<glm::vec4> geometric_vertex;
	std::vector<glm::vec3> texture_coords;
	std::vector<glm::vec3> vertex_normals;
	glm::vec3 m_color;

	bool textures_coords_valid;
	bool normals_valid;

	const char* ObjName;

public:
	PhysicalObject(const char* objName, glm::vec3 objcolor, GLuint fragShader);

	int initialize();
	int execute(glm::mat4 ModelMatrix = glm::mat4(1.0));
	glm::vec3 getColor();

	~PhysicalObject();
};

