#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include <glm/glm.hpp>

struct face {
	std::vector<int> vertex;
	std::vector<int> text_coords;
	std::vector<int> normals;
};

struct extremum {
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	float zmin;
	float zmax;
};

bool loadObjFile(const char* file_path, std::vector<glm::vec4> &geometric_vertex,
										std::vector<glm::vec3> &texture_coords,
										std::vector<glm::vec3> &vertex_normals, extremum &_extremum);