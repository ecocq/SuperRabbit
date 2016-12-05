#include "transformation_mat.h"

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

glm::mat4 rotation_around_axis(float angle_d, glm::vec3 vect) {
	float nx = vect.x;
	float ny = vect.y;
	float nz = vect.z;
	float angle = angle_d * PI / 180.0;
	float c = cos(angle);
	float s = sin(angle);

	float rot_[16] = {
		nx*nx*(1 - c) + c, nx*ny*(1 - c) + nz*s, nx*nz*(1 - c) - ny*s, 0,
		nx*ny*(1 - c) - nz*s, ny*ny*(1 - c) + c, ny*nz*(1 - c) + nx*s, 0,
		nx*nz*(1 - c) + ny*s, ny*nz*(1 - c) - nx*s, nz*nz*(1 - c) + c, 0,
		0, 0, 0, 1
	};
	glm::mat4 rot_mat = glm::make_mat4(rot_);
	return rot_mat;
}

glm::mat4 scale(glm::vec3 vector) {
	float vector_[16] = {
		vector.x, 0, 0, 0,
		0, vector.y, 0, 0,
		0, 0, vector.z, 0,
		0, 0, 0, 1
	};
	glm::mat4 vector_mat = glm::make_mat4(vector_);
	return vector_mat;
}

glm::mat4 scale_along_axis(float k, glm::vec3 axis) {
	float nx = axis.x;
	float ny = axis.y;
	float nz = axis.z;
	float vector_[16] = {
		1 + (k - 1)*nx*nx, (k - 1)*nx*ny, (k - 1)*nx*nz, 0,
		(k - 1)*nx*ny, 1 + (k - 1)*ny*ny, (k - 1)*ny*nz, 0,
		(k - 1)*nx*nz, (k - 1)*ny*nz, 1 + (k - 1)*nz*nz, 0,
		0, 0, 0, 1
	};
	glm::mat4 vector_mat = glm::make_mat4(vector_);
	return vector_mat;
}

glm::mat4 orthographic_projection(glm::vec3 axis) {
	return scale_along_axis(0, axis);
}

glm::mat4 reflection(glm::vec3 axis) {
	return scale_along_axis(-1, axis);
}

glm::mat4 shearing_xy(float s, float t) {
	float vector_[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		s, t, 1, 0,
		0, 0, 0, 1
	};
	glm::mat4 vector_mat = glm::make_mat4(vector_);
	return vector_mat;
}

glm::mat4 shearing_xz(float s, float t) {
	float vector_[16] = {
		1, 0, 0, 0,
		s, 1, t, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glm::mat4 vector_mat = glm::make_mat4(vector_);
	return vector_mat;
}

glm::mat4 shearing_yz(float s, float t) {
	float vector_[16] = {
		1, s, t, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glm::mat4 vector_mat = glm::make_mat4(vector_);
	return vector_mat;
}

glm::mat4 shear(bool unshear) {
	float vector_[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		unshear ? -1 : 1, unshear ? -1 : 1, 1, 0,
		0, 0, 0, 1
	};
	glm::mat4 vector_mat = glm::make_mat4(vector_);
	return vector_mat;
}

/* --------------------- */

void applyRotation(glm::mat4 &matrice, float angle_x, float angle_y, float angle_z) {
	if (angle_x != 0) {
		matrice = matrice * rotation_x(angle_x);
	}
	if (angle_y != 0) {
		matrice = matrice * rotation_y(angle_y);
	}
	if (angle_z != 0) {
		matrice = matrice * rotation_z(angle_z);
	}
}

void applyRotationAroundAxis(glm::mat4 &matrice, float angle_d, glm::vec3 vect) {
	matrice = matrice * rotation_around_axis(angle_d, vect);
}

void applyScale(glm::mat4 &matrice, glm::vec3 vector) {
	matrice = matrice * scale(vector);
}

void applyScaleAlongAxis(glm::mat4 &matrice, float k, glm::vec3 axis) {
	matrice = matrice * scale_along_axis(k, axis);
}

void applyOrthographicProjection(glm::mat4 &matrice, glm::vec3 axis) {
	matrice = matrice * orthographic_projection(axis);
}

void applyReflection(glm::mat4 &matrice, glm::vec3 axis) {
	matrice = matrice * reflection(axis);
}
void applyShearOrNot(glm::mat4 &matrice, bool unshear) {
	matrice = matrice * shear(unshear);
}

void applyShearingXY(glm::mat4 &matrice, float s, float t) {
	matrice = matrice * shearing_xy(s, t);
}

void applyShearingXZ(glm::mat4 &matrice, float s, float t) {
	matrice = matrice * shearing_xz(s, t);
}

void applyShearingYZ(glm::mat4 &matrice, float s, float t) {
	matrice = matrice * shearing_yz(s, t);
}