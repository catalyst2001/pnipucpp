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
	tinyobj::attrib_t attrib;
	std::string error;
	std::vector<tinyobj::shape_t> shapes;
	glm::vec3 begin_coords_system = glm::vec3(0.f, 0.f, 0.f);
	glm::mat4x4 world_matrix = glm::translate(glm::mat4x4(1.f), glm::vec3(0.f, 0.f, 0.f));
	if (tinyobj::LoadObj(&attrib, &shapes, NULL, &error, p_filename, NULL, true)) {
		for (auto &shape : shapes) {
			mesh_s mesh;
			strcpy_s(mesh.name, sizeof(mesh.name), shape.name.c_str());
			std::vector<vertex_s> prepared_verts;
			std::vector<glm::vec3> out_verts;
			std::vector<glm::vec3> out_normals;
			for (size_t j = 0; j < shape.mesh.indices.size() / 3; j++) {
				out_verts.push_back(vec3(
					attrib.vertices[shape.mesh.indices[3 * j + 0].vertex_index],
					attrib.vertices[shape.mesh.indices[3 * j + 1].vertex_index],
					attrib.vertices[shape.mesh.indices[3 * j + 2].vertex_index]
				));

				out_normals.push_back(vec3(
					attrib.normals[shape.mesh.indices[3 * j + 0].normal_index],
					attrib.normals[shape.mesh.indices[3 * j + 1].normal_index],
					attrib.normals[shape.mesh.indices[3 * j + 2].normal_index]
				));
			}

			glm::vec3 old_position;
			move_to_coords_system_start(old_position, out_verts, begin_coords_system);
			mesh.transform = glm::translate(world_matrix, old_position);

			for (size_t j = 0; j < out_verts.size(); j++) {
				vertex_s vert;
				vert.vertex = out_verts[j];
				vert.normal = out_normals[j];
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
	MessageBoxA(0, error.c_str(), "Loading model error", 0);
	return false;
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
	glVertexPointer(3, GL_FLOAT, sizeof(vertex_s), (char *)offsetof(vertex_s, vertex));
	glNormalPointer(GL_FLOAT, sizeof(vertex_s), (char *)offsetof(vertex_s, normal));
	for (size_t i = 0; i < meshes.size(); i++) {
		mesh_s *p_currmesh = &meshes[i];
		if (i == 2) {
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(p_currmesh->transform));
			glBindBuffer(GL_ARRAY_BUFFER, p_currmesh->vbo);
			glDrawArrays(GL_TRIANGLES, 0, p_currmesh->num_verts);
			glPopMatrix();
		}
	}
}
