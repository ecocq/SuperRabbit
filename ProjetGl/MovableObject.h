#pragma once
class MovableObject : public PhysicalObject
{
public:
	MovableObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID);

	void applyTransformsFromControls();
	void fix_vertex(glm::mat4 MVP);
	void colliderTrans();

	void setObjects(std::vector<PhysicalObject*> _objects);

private:

	int scale = 0;
	std::vector<PhysicalObject*> m_objects;
};

