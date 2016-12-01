#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include <glm/glm.hpp>

bool loadObjFile(const char* file_path, std::vector<glm::vec4> &geometric_vertex,
										std::vector<glm::vec3> &texture_coords,
										std::vector<glm::vec3> &vertex_normals);