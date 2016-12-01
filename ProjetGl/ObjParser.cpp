#include "ObjParser.h"

bool loadObjFile(const char* file_path, std::vector<glm::vec4> &geometric_vertex,
	std::vector<glm::vec3> &texture_coords,
	std::vector<glm::vec3> &vertex_normals) {

	geometric_vertex.clear();
	texture_coords.clear();
	vertex_normals.clear();

	std::vector<glm::vec4> temp_vertex;
	std::vector<glm::vec3> temp_texture_coords;
	std::vector<glm::vec3> temp_normals;

	std::vector<faces> temp_faces; // x will be the vertex, y the coords and z the normals

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

		std::string line_char;
		std::getline(ifs, line_char);

		// vertex points
		if (type== "v") {
			glm::vec4 vertex;
			vertex.w = 1.0f; // Default value
			int n = sscanf(line_char.c_str(), "%f %f %f %f\n", &vertex.x, &vertex.y, &vertex.z, &vertex.w);
			temp_vertex.push_back(vertex);
		}
		else if (type == "vt") {
			glm::vec3 coord;
			coord.z = 0.0f; // Default value
			sscanf(line_char.c_str(), "%f %f\n", &coord.x, &coord.y, &coord.z);
			temp_texture_coords.push_back(coord);
		}
		else if (type == "vn") {
			glm::vec3 normal;
			sscanf(line_char.c_str(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (type == "f") {
			faces face_tab[3];
			size_t n = std::count(line_char.begin(), line_char.end(), '/');
			if (n == 6) {
				int i = sscanf(line_char.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d\n", &face_tab[0].vertex, &face_tab[0].text_coords, &face_tab[0].normal, &face_tab[1].vertex, &face_tab[1].text_coords, &face_tab[1].normal, &face_tab[2].vertex, &face_tab[2].text_coords, &face_tab[2].normal);
				if (i != 9) {
					face_tab[0].text_coords = face_tab[1].text_coords = face_tab[2].text_coords = -1;
				}
			}
			else if (n == 0) {
				sscanf(line_char.c_str(), "%d %d %d\n", &face_tab[0].vertex, &face_tab[1].vertex, &face_tab[2].vertex);
				face_tab[0].text_coords = face_tab[1].text_coords = face_tab[2].text_coords = -1;
				face_tab[0].normal = face_tab[1].normal = face_tab[2].normal = -1;
			}
			else if (n == 3) {
				sscanf(line_char.c_str(), "%d/%d %d/%d %d/%d\n", &face_tab[0].vertex, &face_tab[0].text_coords, &face_tab[1].vertex, &face_tab[1].text_coords, &face_tab[2].vertex, &face_tab[2].text_coords);
				face_tab[0].normal = face_tab[1].normal = face_tab[2].normal = -1;
			}
			else {
				printf("error obj not supported");
				return false;
			}

			for each (faces face in face_tab)
			{
				temp_faces.push_back(face);
			}
		}
	}

	for each (faces face in temp_faces)
	{

		geometric_vertex.push_back(temp_vertex[face.vertex -1]);
		if (face.text_coords != -1) {
			texture_coords.push_back(temp_texture_coords[face.text_coords - 1]);
		}
		if (face.normal != -1) {
			vertex_normals.push_back(temp_normals[face.normal - 1]);
		}
	}

	return true;
}