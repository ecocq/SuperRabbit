#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include <glm/glm.hpp>

struct faces {
	int vertex;
	int text_coords;
	int normal;
};

bool loadObjFile(const char* file_path, std::vector<glm::vec3> &geometric_vertex,
										std::vector<glm::vec2> &texture_coords,
										std::vector<glm::vec3> &vertex_normals);