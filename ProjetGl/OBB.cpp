#include "OBB.h"


OBB::OBB(glm::mat4 matrix, glm::vec3 extent) :
	m_matrix(matrix),
	m_extent(extent)
{
	initializePosition();
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, geometric_vertex.size() * sizeof(glm::vec4), &geometric_vertex[0], GL_STATIC_DRAW);
}


OBB::~OBB()
{
}

void OBB::initializePosition() {

	glm::vec3 point(0.0f, 0.0f, 0.0f);
	//std::cout << glm::to_string(point) << std::endl;
	if (geometric_vertex.size() < 8) {
		point = GetCenterPoint();
		geometric_vertex.push_back(glm::vec4(point.x + m_extent.x, point.y + m_extent.y, point.z + m_extent.z, 1));
		geometric_vertex.push_back(glm::vec4(point.x + m_extent.x, point.y + m_extent.y, point.z - m_extent.z, 1));
		geometric_vertex.push_back(glm::vec4(point.x + m_extent.x, point.y - m_extent.y, point.z + m_extent.z, 1));
		geometric_vertex.push_back(glm::vec4(point.x + m_extent.x, point.y - m_extent.y, point.z - m_extent.z, 1));
		geometric_vertex.push_back(glm::vec4(point.x - m_extent.x, point.y + m_extent.y, point.z + m_extent.z, 1));
		geometric_vertex.push_back(glm::vec4(point.x - m_extent.x, point.y + m_extent.y, point.z - m_extent.z, 1));
		geometric_vertex.push_back(glm::vec4(point.x - m_extent.x, point.y - m_extent.y, point.z + m_extent.z, 1));
		geometric_vertex.push_back(glm::vec4(point.x - m_extent.x, point.y - m_extent.y, point.z - m_extent.z, 1));
	}
	else {
		geometric_vertex[0] = m_matrix*glm::vec4(point.x + m_extent.x, point.y + m_extent.y, point.z + m_extent.z, 1);
		geometric_vertex[1] = m_matrix*glm::vec4(point.x + m_extent.x, point.y + m_extent.y, point.z - m_extent.z, 1);
		geometric_vertex[2] = m_matrix*glm::vec4(point.x + m_extent.x, point.y - m_extent.y, point.z + m_extent.z, 1);
		geometric_vertex[3] = m_matrix*glm::vec4(point.x + m_extent.x, point.y - m_extent.y, point.z - m_extent.z, 1);
		geometric_vertex[4] = m_matrix*glm::vec4(point.x - m_extent.x, point.y + m_extent.y, point.z + m_extent.z, 1);
		geometric_vertex[5] = m_matrix*glm::vec4(point.x - m_extent.x, point.y + m_extent.y, point.z - m_extent.z, 1);
		geometric_vertex[6] = m_matrix*glm::vec4(point.x - m_extent.x, point.y - m_extent.y, point.z + m_extent.z, 1);
		geometric_vertex[7] = m_matrix*glm::vec4(point.x - m_extent.x, point.y - m_extent.y, point.z - m_extent.z, 1);
	}
}

void OBB::setMatrix(glm::mat4 _matrix) {
	m_matrix = m_matrix*_matrix;
	// std::cout << glm::to_string(m_matrix) << std::endl;
	initializePosition();
}

void OBB::execute() {


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, geometric_vertex.size() * sizeof(glm::vec4), &geometric_vertex[0], GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,                  // attribute
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_LINES, 0, geometric_vertex.size());

	glDisableVertexAttribArray(0);
}

glm::vec3 OBB::GetCenterPoint() {
	return glm::vec3(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
}

void OBB::GetInvRot(glm::vec3 *pvRot)
{
	pvRot[0] = glm::vec3(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2]);
	pvRot[1] = glm::vec3(m_matrix[1][0], m_matrix[1][1], m_matrix[1][2]);
	pvRot[2] = glm::vec3(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]);
}

bool OBB::collides(OBB &other)
{
	glm::vec3 SizeA = m_extent;
	glm::vec3 SizeB = other.m_extent;
	glm::vec3 RotA[3], RotB[3];

	GetInvRot(RotA);
	other.GetInvRot(RotB);

	float R[3][3];  // Rotation from B to A
	float AR[3][3]; // absolute values of R matrix, to use with box extents
	float ExtentA, ExtentB, Separation;
	int i, k;

	// Calculate B to A rotation matrix
	for (i = 0; i < 3; i++)
		for (k = 0; k < 3; k++)
		{
			R[i][k] = glm::dot(RotA[i], RotB[k]);
			AR[i][k] = fabs(R[i][k]);
		}

	// Vector separating the centers of Box B and of Box A	
	glm::vec3 vSepWS = other.GetCenterPoint() - GetCenterPoint();
	// Rotated into Box A's coordinates
	glm::vec3 vSepA(glm::dot(vSepWS, RotA[0]), glm::dot(vSepWS, RotA[1]), glm::dot(vSepWS, RotA[2]));

	// Test if any of A's basis vectors separate the box
	for (i = 0; i < 3; i++)
	{
		ExtentA = SizeA[i];
		ExtentB = glm::dot(SizeB, glm::vec3(AR[i][0], AR[i][1], AR[i][2]));
		Separation = fabs(vSepA[i]);

		if (Separation > ExtentA + ExtentB) return false;
	}

	// Test if any of B's basis vectors separate the box
	for (k = 0; k < 3; k++)
	{
		ExtentA = glm::dot(SizeA, glm::vec3(AR[0][k], AR[1][k], AR[2][k]));
		ExtentB = SizeB[k];
		Separation = fabs(glm::dot(vSepA, glm::vec3(R[0][k], R[1][k], R[2][k])));

		if (Separation > ExtentA + ExtentB) return false;
	}

	// Now test Cross Products of each basis vector combination ( A[i], B[k] )
	for (i = 0; i<3; i++)
		for (k = 0; k<3; k++)
		{
			int i1 = (i + 1) % 3, i2 = (i + 2) % 3;
			int k1 = (k + 1) % 3, k2 = (k + 2) % 3;
			ExtentA = SizeA[i1] * AR[i2][k] + SizeA[i2] * AR[i1][k];
			ExtentB = SizeB[k1] * AR[i][k2] + SizeB[k2] * AR[i][k1];
			Separation = fabs(vSepA[i2] * R[i1][k] - vSepA[i1] * R[i2][k]);
			if (Separation > ExtentA + ExtentB) return false;
		}

	// No separating axis found, the boxes overlap	
	return true;
}