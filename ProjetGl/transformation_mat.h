#ifndef TRANS_MATH_H
#define TRANS_MATH_H

glm::vec4 translation(glm::vec3 trans, glm::vec4 pos);

glm::vec4 rotation_x(float angle, glm::vec4 pos);
glm::vec4 rotation_y(float angle, glm::vec4 pos);
glm::vec4 rotation_z(float angle, glm::vec4 pos);

#endif