#include "ObjParser.h"

bool loadObjFile(const char* file_path, std::vector<glm::vec4> &geometric_vertex,
	std::vector<glm::vec3> &texture_coords,
	std::vector<glm::vec3> &vertex_normals) {

	geometric_vertex.clear();
	texture_coords.clear();
	vertex_normals.clear();

	std::ifstream ifs(file_path);
	if (!ifs) {
		std::cerr << "Can't open file " << file_path << " !" << std::endl;
		return false;
	}

	std::string line;
	while (!ifs.eof()) {
		line.clear();
		std::string type;
		ifs >> type;

		// comment line
		if (type == "#")
			continue;

		// vertex points
		else if (type== "v") {
			std::string line_char;
			std::getline(ifs, line_char);
			glm::vec4 vertex;
			vertex.w = 1.0f; // Default value
			sscanf(line_char.c_str(), "%f %f %f %f\n", &vertex.x, &vertex.y, &vertex.z, &vertex.w);
			geometric_vertex.push_back(vertex);
		}
	}

	return true;
}