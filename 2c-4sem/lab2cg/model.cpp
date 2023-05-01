#include "model.h"
#include "dbg.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../cgcommon/tiny_obj_loader.h"

void compute_centroid(vec3 &dst_vec, const vector<vec3> &vertices)
{
	float num_vertices;
	dst_vec = glm::vec3(0.f, 0.f, 0.f);
	for (size_t i = 0; i < vertices.size(); i++)
		dst_vec += vertices[i];

	num_vertices = (float)vertices.size();
	dst_vec /= num_vertices;
}

void move_to_coords_system_start(vec3 &center_position, vector<vec3> &vertices, const vec3 begin_of_coords_system)
{
	glm::vec3 centroid;
	compute_centroid(centroid, vertices);
	center_position = centroid;
	for (size_t i = 0; i < vertices.size(); i++)
		vertices[i] -= (centroid + begin_of_coords_system);
}

bool model::file_exists(const char *p_path)
{
	FILE *fp = NULL;
	fopen_s(&fp, p_path, "r");
	if (!fp)
		return false;

	fclose(fp);
	return true;
}

#pragma region CACHE CODE
bool model::load_cache(const char *p_filename)
{
	memfile_s memmdl;
	if (!memfile_open(&memmdl, p_filename))
		return false;

	mdlcache_hdr *p_header = (mdlcache_hdr *)memmdl.p_data;
	if (p_header->signature != MDLCACHE_SIGNATURE) {
		printf("Model cache file signature mistmatch!\n");
		memfile_close(&memmdl);
		return false;
	}

	if (p_header->vertex_size != sizeof(vertex_s)) {
		printf("Vertex size mistmatch!\n");
		memfile_close(&memmdl);
		return false;
	}

	int num_meshes;
	mdlcache_chunk *p_meshes_hdrs = (mdlcache_chunk *)&p_header->chunks[MDLCACHE_CHUNK_MESHES];
	if ((p_meshes_hdrs->chunk_size % sizeof(mdlcache_mesh))) {
		printf("Chunk size not multiple mdlcache_mesh size!\n");
		memfile_close(&memmdl);
		return false;
	}

	/* LOAD MESHES */
	mdlcache_mesh *p_mesh = (mdlcache_mesh *)&memmdl.p_data[p_meshes_hdrs->data_offset];
	num_meshes = p_meshes_hdrs->chunk_size / sizeof(mdlcache_mesh);
	for (int i = 0; i < num_meshes; i++, p_mesh++) {
		mesh_s mesh;
		strcpy_s(mesh.name, sizeof(mesh.name), (char *)&memmdl.p_data[p_mesh->name_offset]);
		mesh.position = p_mesh->centroid;
		mesh.rotation = glm::vec3(0.f, 0.f, 0.f);

		vertex_s *p_verts = (vertex_s *)&memmdl.p_data[p_mesh->vertices_offset];
		glGenBuffers(1, &mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, p_mesh->num_vertices * sizeof(vertex_s), p_verts, GL_STATIC_DRAW);
		assert(glGetError() == GL_NO_ERROR);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	}
	memfile_close(&memmdl);
	return false;
}

bool model::build_cache(const char *p_objfilename, const char *p_cachefilename)
{
	tinyobj::attrib_t attribs;
	std::string warnings, errors;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	FILE *fp = NULL;
	fopen_s(&fp, p_cachefilename, "wb");
	if (!fp) {
		printf("Failed to create file '%s'\n", p_cachefilename);
		return false;
	}

	if (!tinyobj::LoadObj(&attribs, &shapes, &materials, &warnings, &errors, p_objfilename, NULL, true)) {
		printf("model::build_cache(): WARNINGS: \n%s\n", warnings.c_str());
		fclose(fp);
		return false;
	}

	fpos_t offset = sizeof(mdlcache_hdr) + sizeof(mdlcache_mesh) * shapes.size();
	fsetpos(fp, &offset);

	char name[128];
	for (size_t ishape = 0; ishape < shapes.size(); ishape++) {
		tinyobj::shape_t *p_shape = &shapes[ishape];		
		strcpy_s(name, sizeof(name), p_shape->name.c_str());

		std::vector<glm::vec3> group_verts;
		std::vector<glm::vec3> normals;
		std::vector<vertex_s> prepared_verts;
		tinyobj::mesh_t *p_mesh = &p_shape->mesh;
		for (size_t idx = 0; idx < p_mesh->indices.size() / 3; idx++) {
			tinyobj::index_t idx0 = p_shape->mesh.indices[3 * idx + 0];
			tinyobj::index_t idx1 = p_shape->mesh.indices[3 * idx + 1];
			tinyobj::index_t idx2 = p_shape->mesh.indices[3 * idx + 2];

			// Get the three vertex indexes and coordinates
			int vi[3], ni[3]; // indexes
			float v[3][3]; // coords
			float vn[3][3]; // normals
			for (int k = 0; k < 3; k++) {
				vi[0] = idx0.vertex_index;
				vi[1] = idx1.vertex_index;
				vi[2] = idx2.vertex_index;
				assert(vi[0] >= 0);
				assert(vi[1] >= 0);
				assert(vi[2] >= 0);

				v[0][k] = attribs.vertices[3 * vi[0] + k];
				v[1][k] = attribs.vertices[3 * vi[1] + k];
				v[2][k] = attribs.vertices[3 * vi[2] + k];
			}

			group_verts.push_back(glm::vec3(v[0][0], v[0][1], v[0][2]));
			group_verts.push_back(glm::vec3(v[1][0], v[1][1], v[1][2]));
			group_verts.push_back(glm::vec3(v[2][0], v[2][1], v[2][2]));
		}

		//glm::vec3 old_position;
		//move_to_coords_system_start(old_position, out_verts, begin_coords_system);
		//mesh.transform = glm::translate(world_matrix, old_position);

		for (size_t j = 0; j < group_verts.size(); j++) {
			vertex_s vert;
			vert.vertex = group_verts[j];
			vert.normal = /*(out_normals.size()) ? out_normals[j] : */vec3(0.f, 0.f, 0.f);
			prepared_verts.push_back(vert);
		}
	}

	return false;
}
#pragma endregion

model::model()
{
}

model::~model()
{
}

bool model::load_model(const char *p_filename)
{
	/* LOAD MODEL */
	printf("Loading model...\n");
	glm::vec3 begin_coords_system = glm::vec3(0.f, 0.f, 0.f);
	tinyobj::attrib_t attribs;
	std::string warnings, errors;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	if (!tinyobj::LoadObj(&attribs, &shapes, &materials, &warnings, &errors, p_filename, NULL, true)) {
		DBG("Failed to load OBJ model. Error: %s", errors.c_str());
		return false;
	}

	for (size_t ishape = 0; ishape < shapes.size(); ishape++) {
		tinyobj::shape_t *p_shape = &shapes[ishape];
		mesh_s *p_mesh_object = new mesh_s();
		if (!p_mesh_object) {
			DBG("Memory allocation failed");
			return false;
		}

		strcpy_s(p_mesh_object->name, sizeof(p_mesh_object->name), p_shape->name.c_str());
		DBG("Preparing group ( %zd / %zd ): %s", ishape+1, shapes.size(), p_mesh_object->name);

		std::vector<glm::vec3> group_verts;
		std::vector<glm::vec3> normals;
		std::vector<vertex_s> prepared_verts;
		tinyobj::mesh_t *p_mesh = &p_shape->mesh;
		for (size_t idx = 0; idx < p_mesh->indices.size() / 3; idx++) {
			tinyobj::index_t idx0 = p_shape->mesh.indices[3 * idx + 0];
			tinyobj::index_t idx1 = p_shape->mesh.indices[3 * idx + 1];
			tinyobj::index_t idx2 = p_shape->mesh.indices[3 * idx + 2];

			// Get the three vertex indexes and coordinates
			int vi[3], ni[3]; // indexes
			float v[3][3], vn[3][3]; // coords
			for (int k = 0; k < 3; k++) {

				/* VERTICES */
				vi[0] = idx0.vertex_index;
				vi[1] = idx1.vertex_index;
				vi[2] = idx2.vertex_index;
				assert(vi[0] >= 0);
				assert(vi[1] >= 0);
				assert(vi[2] >= 0);

				v[0][k] = attribs.vertices[3 * vi[0] + k];
				v[1][k] = attribs.vertices[3 * vi[1] + k];
				v[2][k] = attribs.vertices[3 * vi[2] + k];

				/* NORMALS */
				ni[0] = idx0.normal_index;
				ni[1] = idx1.normal_index;
				ni[2] = idx2.normal_index;
				assert(ni[0] >= 0);
				assert(ni[1] >= 0);
				assert(ni[2] >= 0);
				vn[0][k] = attribs.normals[3 * ni[0] + k];
				vn[1][k] = attribs.normals[3 * ni[1] + k];
				vn[2][k] = attribs.normals[3 * ni[2] + k];
			}

			group_verts.push_back(glm::vec3(v[0][0], v[0][1], v[0][2]));
			group_verts.push_back(glm::vec3(v[1][0], v[1][1], v[1][2]));
			group_verts.push_back(glm::vec3(v[2][0], v[2][1], v[2][2]));

			normals.push_back(glm::vec3(vn[0][0], vn[0][1], vn[0][2]));
			normals.push_back(glm::vec3(vn[1][0], vn[1][1], vn[1][2]));
			normals.push_back(glm::vec3(vn[2][0], vn[2][1], vn[2][2]));
		}

		p_mesh_object->rotation = glm::vec3(0.f, 0.f, 0.f);
		move_to_coords_system_start(p_mesh_object->position, group_verts, begin_coords_system);

		for (size_t j = 0; j < group_verts.size(); j++) {
			vertex_s vert;
			vert.vertex = group_verts[j];
			vert.normal = (normals.size()) ? normals[j] : vec3(0.f, 0.f, 0.f);
			prepared_verts.push_back(vert);
		}

		// create buffers
		glGenBuffers(1, &p_mesh_object->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, p_mesh_object->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_s) * prepared_verts.size(), (char *)prepared_verts.data(), GL_STATIC_DRAW);
		assert(glGetError() == GL_NO_ERROR);
		p_mesh_object->num_verts = (unsigned int)prepared_verts.size();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		meshes.push_back(p_mesh_object);
	}
	return true;
}

void model::free_model()
{
	for (size_t i = 0; i < meshes.size(); i++) {
		glDeleteBuffers(1, &meshes[i]->vbo);
		delete meshes[i];
	}
}

std::vector<mesh_s *> &model::get_meshes()
{
	return meshes;
}

size_t model::get_meshes_count()
{
	return meshes.size();
}

bool model::find_mesh_by_name(size_t *p_dst_index, const char *p_meshname)
{
	for (size_t i = 0; i < meshes.size(); i++) {
		if (!strcmp(meshes[i]->name, p_meshname)) {
			*p_dst_index = i;
			return true;
		}
	}
	return false;
}

mesh_s *model::get_mesh_by_index(size_t _index)
{
	if (_index >= meshes.size())
		return NULL;

	return meshes[_index];
}

void model::draw_mesh(mesh_s *p_mesh)
{
	glPushMatrix();
	glTranslatef(p_mesh->pos_of_parent_curr.x, p_mesh->pos_of_parent_curr.y, p_mesh->pos_of_parent_curr.z);
	glRotatef(p_mesh->rotation.x, 1.f, 0.f, 0.f);
	glRotatef(p_mesh->rotation.y, 0.f, 1.f, 0.f);
	glRotatef(p_mesh->rotation.z, 0.f, 0.f, 1.f);

	glBindBuffer(GL_ARRAY_BUFFER, p_mesh->vbo);
	glVertexPointer(3, GL_FLOAT, sizeof(vertex_s), (void *)offsetof(vertex_s, vertex));
	glNormalPointer(GL_FLOAT, sizeof(vertex_s), (void *)offsetof(vertex_s, normal));
	glDrawArrays(GL_TRIANGLES, 0, p_mesh->num_verts);	
	assert(glGetError() == GL_NO_ERROR);

	for (int i = 0; i < p_mesh->num_childs; i++)
		draw_mesh(meshes[p_mesh->childs_idxs[i]]);

	glPopMatrix();
}

void model::draw_model()
{
	draw_mesh(meshes[0]);
}

bool memfile_open(memfile_s *p_dst, const char *p_file)
{
	assert(p_dst && p_file);

	FILE *fp = NULL;
	fopen_s(&fp, p_file, "rb");
	if (!fp)
		return false;

	fpos_t file_offset;
	fseek(fp, 0, SEEK_END);
	fgetpos(fp, &file_offset);
	if (!file_offset) {
		fclose(fp);
		return false;
	}

	p_dst->size = (size_t)file_offset;
	p_dst->p_data = (unsigned char *)calloc(p_dst->size, 1);
	assert(p_dst->p_data);
	if (!p_dst->p_data) {
		fclose(fp);
		return false;
	}

	if (fread(p_dst->p_data, 1, p_dst->size, fp) != p_dst->size) {
		free(p_dst->p_data);
		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;
}

void memfile_close(memfile_s *p_src)
{
	if (p_src->p_data) {
		free(p_src->p_data);
		p_src->p_data = NULL;
		p_src->size = 0;
	}
}