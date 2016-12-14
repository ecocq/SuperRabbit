#include "ObjParser.h"

#include <sstream>

int count_substr(std::string s, std::string substr) {
	int count = 0;
	size_t nPos = s.find(substr, 0); // fist occurrence
	while (nPos != std::string::npos)
	{
		count++;
		nPos = s.find(substr, nPos + 1);
	}

	return count;
}

void initExtremum(extremum &_extremum) {
	_extremum.xmax = (float) -HUGE;
	_extremum.xmin = (float) HUGE;
	_extremum.ymax = (float) -HUGE;
	_extremum.ymin = (float) HUGE;
	_extremum.zmax = (float) -HUGE;
	_extremum.zmin = (float) HUGE;
}

void updateExtremum(extremum &_extremum, glm::vec4 vertex) {
	if (vertex.x > _extremum.xmax)
		_extremum.xmax = vertex.x;
	if (vertex.x < _extremum.xmin)
		_extremum.xmin = vertex.x;

	if (vertex.y > _extremum.ymax)
		_extremum.ymax = vertex.y;
	if (vertex.y < _extremum.ymin)
		_extremum.ymin = vertex.y;

	if (vertex.z > _extremum.zmax)
		_extremum.zmax = vertex.z;
	if (vertex.z < _extremum.zmin)
		_extremum.zmin = vertex.z;
}

bool loadObjFile(const char* file_path, std::vector<glm::vec4> &geometric_vertex,
	std::vector<glm::vec3> &texture_coords,
	std::vector<glm::vec3> &vertex_normals, extremum &_extremum) {

	geometric_vertex.clear();
	texture_coords.clear();
	vertex_normals.clear();

	initExtremum(_extremum);

	std::vector<glm::vec4> temp_vertex;
	std::vector<glm::vec3> temp_texture_coords;
	std::vector<glm::vec3> temp_normals;

	std::vector<face> temp_faces; // x will be the vertex, y the coords and z the normals

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
			updateExtremum(_extremum, vertex);
		}
		else if (type == "vt") {
			glm::vec3 coord;
			coord.z = 0.0f; // Default value
			sscanf(line_char.c_str(), "%f %f %f\n", &coord.x, &coord.y, &coord.z);
			temp_texture_coords.push_back(coord);
		}
		else if (type == "vn") {
			glm::vec3 normal;
			sscanf(line_char.c_str(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (type == "f") {
			face m_face;
			
			std::istringstream iss(line_char);
			std::string s;
			int nbVertices = 0;
			// Separating groups
			while (std::getline(iss, s, ' ')) {
				if (s.empty()) {
					continue;
				}
				
				int n = count_substr(s, "//");
				
				// Vertex//Normal
				if (n == 1) {
					int vertex, normal;
					sscanf(s.c_str(), "%d//%d", &vertex, &normal);
					m_face.vertex.push_back(vertex);
					m_face.normals.push_back(normal);
				}
				else {
					n = count_substr(s, "/");
		
					// Vertex Only
					if (n == 0) {
						int vertex;
						sscanf(s.c_str(), "%d", &vertex);
						m_face.vertex.push_back(vertex);
					}
					// Vertex/texture
					else if (n == 1) {
						int vertex, texture;
						sscanf(s.c_str(), "%d/%d", &vertex, &texture);
						m_face.vertex.push_back(vertex);
						m_face.text_coords.push_back(texture);
					}
					// Vertex/texture/normal
					else if (n == 2) {
						int vertex, texture, normal;
						sscanf(s.c_str(), "%d/%d/%d", &vertex, &texture, &normal);
						m_face.vertex.push_back(vertex);
						m_face.text_coords.push_back(texture);
						m_face.normals.push_back(normal);
						
					}

					if (nbVertices == 3) {
						// The face has 4 vertices, therefore we need to split it into 2 triangles
						int index1 = (int)m_face.vertex.size() - 4;
						int index2 = (int)m_face.vertex.size() - 2;
						m_face.vertex.push_back(m_face.vertex.at(index1));
						m_face.text_coords.push_back(m_face.text_coords.at(index1));
						m_face.normals.push_back(m_face.normals.at(index1));
						m_face.vertex.push_back(m_face.vertex.at(index2));
						m_face.text_coords.push_back(m_face.text_coords.at(index2));
						m_face.normals.push_back(m_face.normals.at(index2));
						break;
					}
					nbVertices++;
				}
			}

			for each(int vertex in m_face.vertex) {
				geometric_vertex.push_back(temp_vertex[vertex - 1]);
			}
			for each(int uv in m_face.text_coords) {
				texture_coords.push_back(temp_texture_coords[uv - 1]);
			}
			for each(int normal in m_face.normals) {
				vertex_normals.push_back(temp_normals[normal - 1]);
			}
			temp_faces.push_back(m_face);	
		}
	}
	return true;
}