#pragma once
#include <vector>

class PhysicalObject
{
protected:
	GLFWwindow* window;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint fragmentShader;

	std::vector<glm::vec4> geometric_vertex;
	std::vector<glm::vec3> texture_coords;
	std::vector<glm::vec3> vertex_normals;
	glm::vec3 m_color;

	glm::vec3 position;

	glm::mat4 ModelMatrix;
	glm::vec3 translated;

	bool textures_coords_valid;
	bool normals_valid;

	const char* ObjPath;

public:
	PhysicalObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, glm::mat4 initialPos = glm::mat4(1.0));
	PhysicalObject(std::vector<glm::vec4> geometric_vertex, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow);
	PhysicalObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, glm::vec3 initialPos);

	int initialize();
	int execute();
	void fix_vertex();
	void initTransforms(glm::vec3 translate, glm::vec3 rotate);
	virtual void applyTransformsFromControls();
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

	~PhysicalObject();
};

