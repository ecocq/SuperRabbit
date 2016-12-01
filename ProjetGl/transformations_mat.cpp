#include <stdio.h>
#include <stdlib.h>

#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transformation_mat.h"

#define PI 3.14159265

glm::mat4 translation(glm::vec3 trans) {
	float trans_[16] = {
		1, 0, 0, trans.x,
		0, 1, 0, trans.y,
		0, 0, 1, trans.z,
		0, 0, 0, 1
	};
	glm::mat4 trans_mat = glm::make_mat4(trans_);
	return trans_mat;
}

glm::mat4 rotation_x(float angle_d) {
	float angle = angle_d * PI / 180.0f;
	float rot_[16] = {
		1, 0, 0, 0,
		0, cos(angle), sin(angle), 0,
		0, -sin(angle), cos(angle), 0,
		0, 0, 0, 1
	};
	glm::mat4 rot_mat = glm::make_mat4(rot_);
	return rot_mat;
}

glm::mat4 rotation_y(float angle_d) {
	float angle = angle_d * PI / 180.0;
	float rot_[16] = {
		cos(angle), 0, -sin(angle), 0,
		0, 1, 0, 0,
		sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
	};
	glm::mat4 rot_mat = glm::make_mat4(rot_);
	return rot_mat;
}

glm::mat4 rotation_z(float angle_d) {
	float angle = angle_d * PI / 180.0;
	float rot_[16] = {
		cos(angle), -sin(angle), 0, 0,
		sin(angle), cos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glm::mat4 rot_mat = glm::make_mat4(rot_);
	return rot_mat;
}
