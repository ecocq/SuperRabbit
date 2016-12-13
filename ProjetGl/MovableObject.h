#pragma once
class MovableObject : public PhysicalObject
{
public:
	MovableObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID);
	MovableObject(const char* path, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow, GLint programID, glm::vec3 trans);

	void applyTransformsFromControls();
	void fix_vertex(glm::mat4 MVP);
	void colliderTrans();

	void setObjects(std::vector<PhysicalObject*> _objects);

private:
	std::vector<PhysicalObject*> m_objects;
};

