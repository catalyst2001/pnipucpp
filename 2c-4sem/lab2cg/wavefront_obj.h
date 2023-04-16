#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "dbg.h"

struct obj_group {
	char name[64];
	int textureid;
	glm::vec3 position;
	glm::vec3 rotation;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	obj_group() {
		name[0] = 0;
		textureid = 0;
	}
};

/* SUPPORT ONLY TRIANGLES */
bool load_wavefront_obj(std::vector<obj_group *> &dst_groups, const char *p_filename);

void free_wavefront_obj(const std::vector<obj_group *> &dst_groups);