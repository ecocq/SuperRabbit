#pragma once
class MovableObject : public PhysicalObject
{
public:
	MovableObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow);

	void applyTransformsFromControls();

};

