#include "model.h"

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
	glm::mat4x4 world_matrix = glm::translate(glm::mat4x4(1.f), glm::vec3(0.f, 0.f, 0.f));
	tinyobj::attrib_t attribs;
	std::string warnings, errors;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	if (!tinyobj::LoadObj(&attribs, &shapes, &materials, &warnings, &errors, p_filename, NULL, true)) {
		printf("Failed to load OBJ model. Error: %s\n", warnings.c_str());
		return false;
	}

	for (size_t ishape = 0; ishape < shapes.size(); ishape++) {
		tinyobj::shape_t *p_shape = &shapes[ishape];

		mesh_s mesh;
		strcpy_s(mesh.name, sizeof(mesh.name), p_shape->name.c_str());
		printf("Preparing group ( %zd / %zd ): %s\n", ishape+1, shapes.size(), mesh.name);

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

		// create buffers
		glGenBuffers(1, &mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_s) * prepared_verts.size(), (char *)prepared_verts.data(), GL_STATIC_DRAW);
		mesh.num_verts = (unsigned int)prepared_verts.size();
		meshes.push_back(mesh);
	}
	return true;
}

void model::free_model()
{
	for (size_t i = 0; i < meshes.size(); i++)
		glDeleteBuffers(1, &meshes[i].vbo);
}

bool model::find_mesh_by_name(size_t *p_dst_index, const char *p_meshname)
{
	for (size_t i = 0; i < meshes.size(); i++) {
		if (!strcmp(meshes[i].name, p_meshname)) {
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

	return &meshes[_index];
}

void model::draw_model()
{
	glVertexPointer(3, GL_FLOAT, sizeof(vertex_s), (void *)offsetof(vertex_s, vertex));
	glNormalPointer(GL_FLOAT, sizeof(vertex_s), (void *)offsetof(vertex_s, normal));
	for (size_t i = 0; i < meshes.size(); i++) {
		mesh_s *p_currmesh = &meshes[i];
		//glPushMatrix();
		//glMultMatrixf(glm::value_ptr(p_currmesh->transform));
		glBindBuffer(GL_ARRAY_BUFFER, p_currmesh->vbo);
		glDrawArrays(GL_TRIANGLES, 0, p_currmesh->num_verts);
		//glPopMatrix();
	}
}
