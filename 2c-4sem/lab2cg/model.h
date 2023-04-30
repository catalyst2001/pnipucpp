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

#define MESH_MAX_CHILDS 10

struct mesh_s {
	bool b_visible;
	char name[64];
	GLuint vbo;
	unsigned int num_verts;
	glm::vec3 min;
	glm::vec3 max;
	//glm::mat4x4 transform;
	glm::vec3 pos_of_parent;
	glm::vec3 position;
	glm::vec3 rotation;

	int parent_idx;
	int num_childs;
	int childs_idxs[MESH_MAX_CHILDS];
};

/* MODEL CACHE FILE FORMAT */
#define MDLCACHE_NOTHING(x)

enum MDLCACHE_CHUNKS : int {
	MDLCACHE_CHUNK_MESHES = 0,
	MDLCACHE_CHUNK_MATERIALS,
	MDLCACHE_NUM_CHUNKS
};
MDLCACHE_NOTHING(sizeof(MDLCACHE_CHUNKS));

struct mdlcache_chunk {
	int chunk_size;
	int data_offset;
};
MDLCACHE_NOTHING(sizeof(mdlcache_chunk));

#define MDLCACHE_SIGNATURE (MAKEFOURCC('M', 'D', 'L', 'C'))

struct mdlcache_hdr {
	int signature;
	int hashsum;
	int vertex_size;
	mdlcache_chunk chunks[MDLCACHE_NUM_CHUNKS];
};
MDLCACHE_NOTHING(sizeof(mdlcache_hdr));

struct mdlcache_mesh {
	int name_offset;
	int num_vertices;
	int vertices_offset;
	glm::vec3 centroid;
};
MDLCACHE_NOTHING(sizeof(mdlcache_mesh));

struct memfile_s {
	size_t size;
	unsigned char *p_data;
};

bool memfile_open(memfile_s *p_dst, const char *p_file);
void memfile_close(memfile_s *p_src);

class model
{
	friend class model_cache;

	glm::vec3 min; //COMPUTE MIN/MAX
	glm::vec3 max;

	std::vector<mesh_s> meshes;

protected:
	bool file_exists(const char *p_path);

	/* MODEL CACHE HELPER CLASS */
	bool load_cache(const char *p_filename);
	bool build_cache(const char *p_objfilename, const char *p_cachefilename);

public:
	model();
	~model();

	bool    load_model(const char *p_filename);
	void    free_model();

	size_t  get_meshes_count();
	bool    find_mesh_by_name(size_t *p_dst_index, const char *p_meshname);
	mesh_s *get_mesh_by_index(size_t _index);
	void    draw_model();
};
