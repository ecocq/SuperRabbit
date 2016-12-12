#pragma once
class MovableObject : public PhysicalObject
{
public:
	MovableObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow);

	void applyTransformsFromControls();
	void fix_vertex();

	void setObjects(std::vector<PhysicalObject*> _objects);

private:

	int scale = 0;
	std::vector<PhysicalObject*> m_objects;
};

