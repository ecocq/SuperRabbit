#include "Wall.h"
#include "transformation_mat.h"


Wall::Wall(glm::vec2 size, glm::vec3 pos, glm::vec3 rot, glm::vec3 objcolor, GLuint fragShader, GLFWwindow* Objwindow) :
	PhysicalObject("obj/wall.obj", objcolor, fragShader, Objwindow, translation(pos) * rotation_x(rot.x) * rotation_y(rot.y) * rotation_z(rot.z) * scale(glm::vec3(size.x, size.y, 1)))
{
}


Wall::~Wall()
{
}
