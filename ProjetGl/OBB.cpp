#include "OBB.h"


OBB::OBB(glm::vec3 center, extremum _extremum) :
	m_center(center),
	m_extremum(_extremum)
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
	glm::vec3 point = m_center;
	/* Set bounding box corners */
	initial_geometric_vertex.push_back(glm::vec4(m_extremum.xmax, m_extremum.ymax, m_extremum.zmax, 1));
	initial_geometric_vertex.push_back(glm::vec4(m_extremum.xmax, m_extremum.ymax, m_extremum.zmin, 1));
	initial_geometric_vertex.push_back(glm::vec4(m_extremum.xmax, m_extremum.ymin, m_extremum.zmax, 1));
	initial_geometric_vertex.push_back(glm::vec4(m_extremum.xmax, m_extremum.ymin, m_extremum.zmin, 1));
	initial_geometric_vertex.push_back(glm::vec4(m_extremum.xmin, m_extremum.ymax, m_extremum.zmax, 1));
	initial_geometric_vertex.push_back(glm::vec4(m_extremum.xmin, m_extremum.ymax, m_extremum.zmin, 1));
	initial_geometric_vertex.push_back(glm::vec4(m_extremum.xmin, m_extremum.ymin, m_extremum.zmax, 1));
	initial_geometric_vertex.push_back(glm::vec4(m_extremum.xmin, m_extremum.ymin, m_extremum.zmin, 1));
	geometric_vertex = initial_geometric_vertex;
	/* Set bounding box normals */
	initial_vertex_normals.push_back(glm::vec4(0, 0, 1, 1));
	initial_vertex_normals.push_back(glm::vec4(0, 0, -1, 1));
	initial_vertex_normals.push_back(glm::vec4(0, 1, 0, 1));
	initial_vertex_normals.push_back(glm::vec4(0, -1, 0, 1));
	initial_vertex_normals.push_back(glm::vec4(1, 0, 0, 1));
	initial_vertex_normals.push_back(glm::vec4(-1, 0, 0, 1));
	vertex_normals = initial_vertex_normals;
}

void OBB::transform(glm::mat4 _matrix) {
	geometric_vertex_old = geometric_vertex;
	vertex_normals_old = vertex_normals;

	/* Transform box coordinates */
	for (int i = 0; i < geometric_vertex.size(); i++) {
		geometric_vertex[i] = _matrix * initial_geometric_vertex[i];
	}
	/* Avoid translating normals */
	_matrix[3][0] = _matrix[3][1] = _matrix[3][2] = 0;

	/* Transform box normals*/
	for (int i = 0; i < vertex_normals.size(); i++) {
		vertex_normals[i] = _matrix * initial_vertex_normals[i];
	}
}

/* Draw bounding box */
void OBB::execute() {
	// Uncomment to draw OBB lines
	/*
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


	glDrawArrays(GL_LINES, 0, geometric_vertex.size());

	glDisableVertexAttribArray(0);*/
}

void SATtest(const glm::vec4 axis, const std::vector<glm::vec4> vertex, float& min, float& max)
{
	min = (float)HUGE, max = (float)-HUGE;
	for (int i = 0; i < vertex.size(); i++)
	{
		float dot = glm::dot(glm::vec3(vertex[i]), glm::vec3(axis));
		if (dot < min)  min = dot;
		if (dot > max)  max = dot;
	}
}

bool isBetweenOrdered(float val, float lowerBound, float upperBound) {
	return lowerBound <= val && val <= upperBound;
}

bool overlaps(float min1, float max1, float min2, float max2)
{
	return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);
}

/* Is bounding box colliding with another */
bool OBB::collides(const OBB &other) {
	for (int i = 0; i < vertex_normals.size(); i++) {
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(vertex_normals[i], geometric_vertex, shape1Min, shape1Max);
		SATtest(vertex_normals[i], other.geometric_vertex, shape2Min, shape2Max);
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return false; 
		}
	}

	for (int i = 0; i < other.vertex_normals.size(); i++) {
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(other.vertex_normals[i], other.geometric_vertex, shape1Min, shape1Max);
		SATtest(other.vertex_normals[i], geometric_vertex, shape2Min, shape2Max);
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return false; 
		}
	}
	return true;
}
