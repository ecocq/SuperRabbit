#ifndef TRANS_MATH_H
#define TRANS_MATH_H

glm::mat4 translation(glm::vec3 trans);

glm::mat4 rotation_x(float angle_d);
glm::mat4 rotation_y(float angle_d);
glm::mat4 rotation_z(float angle_d);

#endif