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
	geometric_vertex.push_back(glm::vec4(m_extremum.xmax, m_extremum.ymax, m_extremum.zmax, 1));
	geometric_vertex.push_back(glm::vec4(m_extremum.xmax, m_extremum.ymax, m_extremum.zmin, 1));
	geometric_vertex.push_back(glm::vec4(m_extremum.xmax, m_extremum.ymin, m_extremum.zmax, 1));
	geometric_vertex.push_back(glm::vec4(m_extremum.xmax, m_extremum.ymin, m_extremum.zmin, 1));
	geometric_vertex.push_back(glm::vec4(m_extremum.xmin, m_extremum.ymax, m_extremum.zmax, 1));
	geometric_vertex.push_back(glm::vec4(m_extremum.xmin, m_extremum.ymax, m_extremum.zmin, 1));
	geometric_vertex.push_back(glm::vec4(m_extremum.xmin, m_extremum.ymin, m_extremum.zmax, 1));
	geometric_vertex.push_back(glm::vec4(m_extremum.xmin, m_extremum.ymin, m_extremum.zmin, 1));

	/* Set bounding box normals */
	vertex_normals.push_back(glm::vec4(0, 0, 1, 1));
	vertex_normals.push_back(glm::vec4(0, 0, -1, 1));
	vertex_normals.push_back(glm::vec4(0, 1, 0, 1));
	vertex_normals.push_back(glm::vec4(0, -1, 0, 1));
	vertex_normals.push_back(glm::vec4(1, 0, 0, 1));
	vertex_normals.push_back(glm::vec4(-1, 0, 0, 1));
}


void OBB::transform(glm::mat4 _matrix) {
	for (int i = 0; i < geometric_vertex.size(); i++) {
		geometric_vertex[i] = _matrix * geometric_vertex[i];
	}
	for (int i = 0; i < vertex_normals.size(); i++) {
		vertex_normals[i] = _matrix * vertex_normals[i];
	}
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

void SATtest(const glm::vec4 axis, const std::vector<glm::vec4> ptSet, float& minAlong, float& maxAlong)
{
	minAlong = HUGE, maxAlong = -HUGE;
	for (int i = 0; i < ptSet.size(); i++)
	{
		float dotVal = glm::dot(glm::vec3(ptSet[i]), glm::vec3(axis));
		if (dotVal < minAlong)  minAlong = dotVal;
		if (dotVal > maxAlong)  maxAlong = dotVal;
	}
}

bool isBetweenOrdered(float val, float lowerBound, float upperBound) {
	return lowerBound <= val && val <= upperBound;
}

bool overlaps(float min1, float max1, float min2, float max2)
{
	return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);
}


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
