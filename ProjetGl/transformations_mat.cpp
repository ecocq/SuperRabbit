#include <stdio.h>
#include <stdlib.h>

#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transformation_mat.h"

glm::vec4 translation(glm::vec3 trans, glm::vec4 pos) {
	float trans_[16] = {
		1, 0, 0, trans.x,
		0, 1, 0, trans.y,
		0, 0, 1, trans.z,
		0, 0, 0, 1
	};
	glm::mat4 trans_mat = glm::make_mat4(trans_);
	return pos * trans_mat;
}
