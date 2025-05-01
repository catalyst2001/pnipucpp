#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <string>

void gen_sphere(float radius, int slices, int rows, std::vector<glm::vec3>& vertices, std::vector<uint32_t>& indices);

bool obj_load(const char* fileName,
  std::vector<glm::vec3>& out_vertices,
  std::vector<glm::vec2>& out_uvs,
  std::vector<glm::vec3>& out_normals);