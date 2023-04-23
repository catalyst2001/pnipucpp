#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gl_exts.h"

using std::vector;
using glm::vec3;

struct vertex_s {
	glm::vec3 vertex;
	glm::vec3 normal;
};

struct mesh_s {
	bool b_visible;
	char name[64];
	GLuint vbo;
	unsigned int num_verts;
	glm::vec3 min;
	glm::vec3 max;
	glm::mat4x4 transform;
};

class model
{
	glm::vec3 min;
	glm::vec3 max;

	std::vector<mesh_s> meshes;
public:
	model();
	~model();

	bool    load_model(const char *p_filename);
	void    free_model();

	bool    find_mesh_by_name(size_t *p_dst_index, const char *p_meshname);
	mesh_s *get_mesh_by_index(size_t _index);
	void    draw_model();
};
