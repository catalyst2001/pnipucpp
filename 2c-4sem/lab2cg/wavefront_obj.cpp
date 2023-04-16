#include "wavefront_obj.h"

#include <malloc.h>

void *operator new(size_t _size) { return calloc(1, _size); }
void operator delete(void *p_block) { free(p_block); }

bool obj_equalscmd(char *p_currline, const char *p_startcmd)
{
	char *ptr = strchr(p_currline, ' ');
	if (ptr) {
		*ptr = 0x0;
		return !strcmp(p_currline, p_startcmd);
	}
	return false;
}

void obj_try_set_firstcmd(char *p_dst, int maxlen, char *p_line)
{
	char *ptr = strchr(p_line, ' ');
	if (!p_dst[0] && ptr) {
		*ptr = 0x0;
		strcpy_s(p_dst, maxlen, p_line);
	}
}

enum OBJ_CMD {
	OBJ_CMD_GROUP = 0,
	OBJ_CMD_VERTEX,
	OBJ_CMD_VERTEX_NORMAL,
	OBJ_CMD_FACES
};

bool obj_save_data_to_group(std::vector<obj_group *> &dst_groups, char *p_currgroupname, int currgroup_maxlen,
	std::vector<glm::vec3> &temp_vertices, std::vector<glm::vec3> &temp_normals, std::vector<glm::vec2> &temp_uvs,
	std::vector<unsigned int> &vertex_indices, std::vector<unsigned int> &uv_indices, std::vector<unsigned int> &normal_indices)
{
	obj_group *p_meshgroup = new obj_group();
	if (!p_meshgroup) {
		DBG("Failed to allocate memory");
		return false;
	}

	strcpy_s(p_meshgroup->name, sizeof(p_meshgroup->name), p_currgroupname);

	for (size_t i = 0; i < vertex_indices.size(); i++)
		p_meshgroup->vertices.push_back(temp_vertices[vertex_indices[i] - 1]);

	for (size_t i = 0; i < uv_indices.size(); i++)
		p_meshgroup->uvs.push_back(temp_uvs[uv_indices[i] - 1]);

	for (size_t i = 0; i < normal_indices.size(); i++)
		p_meshgroup->normals.push_back(temp_normals[normal_indices[i] - 1]);

	DBG("Vertices count: %zd", p_meshgroup->vertices.size());
	DBG("UVs count: %zd", p_meshgroup->uvs.size());
	DBG("Normals count: %zd", p_meshgroup->normals.size());
	dst_groups.push_back(p_meshgroup);

	/* RESET TO DEFAULTS */
	strcpy_s(p_currgroupname, currgroup_maxlen, "new_group"); // reset value to default
	temp_vertices.clear();
	temp_uvs.clear();
	temp_normals.clear();
	return true;
}

/* SUPPORT ONLY TRIANGLES */
bool load_wavefront_obj(std::vector<obj_group *> &dst_groups, const char *p_filename)
{
	char line[256], group[64], firstcmd[64];
	firstcmd[0] = 0;
	union {
		glm::vec3 temp_vec3;
		glm::vec2 temp_vec2;
	};

	FILE *fp = NULL;
	fopen_s(&fp, p_filename, "rt");
	if (!fp) {
		DBG("Failed to open file '%s'", p_filename);
		return false;
	}

	obj_group *p_object = NULL;
	std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	while (!feof(fp) && !ferror(fp)) {
		fgets(line, sizeof(line), fp);
		if (line[0] == '\n' || line[0] == '#')
			continue;

		if (sscanf(line, "g %64s", group) == 1) {
			obj_try_set_firstcmd(firstcmd, sizeof(firstcmd), line);
		}
		else if (!strcmp(line, "v")) {
			sscanf(line, "v %f %f %f", &temp_vec3.x, &temp_vec3.y, &temp_vec3.z);
			temp_vertices.push_back(temp_vec3);
			obj_try_set_firstcmd(firstcmd, sizeof(firstcmd), line);
		}
		else if (!strcmp(line, "vt")) {
			sscanf(line, "vt %f %f", &temp_vec2.x, &temp_vec2.y);
			temp_uvs.push_back(temp_vec2);
			obj_try_set_firstcmd(firstcmd, sizeof(firstcmd), line);
		}
		else if (!strcmp(line, "vn")) {
			sscanf(line, "vn %f %f %f", &temp_vec3.x, &temp_vec3.y, &temp_vec3.z);
			temp_normals.push_back(temp_vec3);
			obj_try_set_firstcmd(firstcmd, sizeof(firstcmd), line);
		}
		else if (!strncmp(line, "f", 1)) {
			unsigned int vertidxs[3], uvidxs[3], normalidxs[3];
			int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertidxs[0], &uvidxs[0], &normalidxs[0], &vertidxs[1], &uvidxs[1], &normalidxs[1], &vertidxs[2], &uvidxs[2], &normalidxs[2]);
			if (matches != 9) {
				fclose(fp);
				return false;
			}
			vertex_indices.push_back(vertidxs[0]);
			vertex_indices.push_back(vertidxs[1]);
			vertex_indices.push_back(vertidxs[2]);
			uv_indices.push_back(uvidxs[0]);
			uv_indices.push_back(uvidxs[1]);
			uv_indices.push_back(uvidxs[2]);
			normal_indices.push_back(normalidxs[0]);
			normal_indices.push_back(normalidxs[1]);
			normal_indices.push_back(normalidxs[2]);
			obj_try_set_firstcmd(firstcmd, sizeof(firstcmd), line);
		}

		/* SAVE DATA TO GROUP */
		if (obj_equalscmd(line, firstcmd) && !obj_save_data_to_group(dst_groups, group, sizeof(group), temp_vertices, temp_normals, temp_uvs, vertex_indices, uv_indices, normal_indices)) {
			DBG("saving group failed");
			fclose(fp);
			return false;
		}
	}
	return true;
}

void free_wavefront_obj(const std::vector<obj_group*> &dst_groups)
{
	for (size_t i = 0; i < dst_groups.size(); i++) {
		if (dst_groups[i]) {
			delete dst_groups[i];
		}
	}
}
