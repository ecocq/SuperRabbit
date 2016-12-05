#ifndef TRANS_MATH_H
#define TRANS_MATH_H

#include <stdio.h>
#include <stdlib.h>

#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265

glm::mat4 translation(glm::vec3 trans);

glm::mat4 rotation_x(float angle_d);
glm::mat4 rotation_y(float angle_d);
glm::mat4 rotation_z(float angle_d);
glm::mat4 rotation_around_axis(float angle_d, glm::vec3 vect);

glm::mat4 scale(glm::vec3 vector);
glm::mat4 scale_along_axis(float k, glm::vec3 axis);

glm::mat4 orthographic_projection(glm::vec3 axis);
glm::mat4 reflection(glm::vec3 axis);

glm::mat4 shear(bool unshear);
glm::mat4 shearing_xy(float s, float t);
glm::mat4 shearing_xz(float s, float t);
glm::mat4 shearing_yz(float s, float t);

void applyRotation(glm::mat4 &matrice, float angle_x, float angle_y, float angle_z);
void applyRotationAroundAxis(glm::mat4 &matrice, float angle_d, glm::vec3 vect);
void applyScale(glm::mat4 &matrice, glm::vec3 vector);
void applyScaleAlongAxis(glm::mat4 &matrice, float k, glm::vec3 axis);
void applyOrthographicProjection(glm::mat4 &matrice, glm::vec3 axis);
void applyReflection(glm::mat4 &matrice, glm::vec3 axis);
void applyShearOrNot(glm::mat4 &matrice, bool unshear);
void applyShearingXY(glm::mat4 &matrice, float s, float t);
void applyShearingXZ(glm::mat4 &matrice, float s, float t);
void applyShearingYZ(glm::mat4 &matrice, float s, float t);




#endif