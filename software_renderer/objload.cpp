#include "objload.h"

void gen_sphere(float radius, int slices, int rows, std::vector<glm::vec3>& vertices, std::vector<uint32_t>& indices) {
  // Очистка предыдущих данных
  vertices.clear();
  indices.clear();

  // Генерация вершин
  for (int i = 0; i <= rows; ++i) {
    float theta = i * glm::pi<float>() / rows; // Угол от полюса к полюсу
    float sinTheta = sinf(theta);
    float cosTheta = cosf(theta);

    for (int j = 0; j <= slices; ++j) {
      float phi = j * 2 * glm::pi<float>() / slices; // Угол вокруг сферы
      float sinPhi = sinf(phi);
      float cosPhi = cosf(phi);

      // Позиция вершины
      glm::vec3 position;
      position.x = radius * sinTheta * cosPhi;
      position.y = radius * cosTheta;
      position.z = radius * sinTheta * sinPhi;

      vertices.push_back({ position });
    }
  }

  // Генерация индексов
  for (int i = 0; i < rows; ++i) {
    int offset = i * (slices + 1);
    for (int j = 0; j < slices; ++j) {
      int current = offset + j;
      int next = offset + j + 1;
      int nextRow = offset + (slices + 1) + j;
      int nextRowNext = offset + (slices + 1) + j + 1;

      // Первый треугольник
      indices.push_back(current);
      indices.push_back(nextRow);
      indices.push_back(next);

      // Второй треугольник
      indices.push_back(next);
      indices.push_back(nextRow);
      indices.push_back(nextRowNext);
    }
  }
}

bool obj_load(const char* fileName,
  std::vector<glm::vec3>& out_vertices,
  std::vector<glm::vec2>& out_uvs,
  std::vector<glm::vec3>& out_normals)
{
  std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_uvs;
  std::vector<glm::vec3> temp_normals;

  FILE* file = fopen(fileName, "r");
  if (!file)
    return false;

  while (true)
  {
    char lineheader[128];
    if (fscanf(file, "%s", lineheader) == EOF) {
      break;
    }

    if (strcmp(lineheader, "v") == 0)
    {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      temp_vertices.push_back(vertex);
    }
    else if (strcmp(lineheader, "vt") == 0)
    {
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      temp_uvs.push_back(uv);
    }
    else if (strcmp(lineheader, "vn") == 0)
    {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    }
    else if (strcmp(lineheader, "f") == 0)
    {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9) {
        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
      }
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      uvIndices.push_back(uvIndex[0]);
      uvIndices.push_back(uvIndex[1]);
      uvIndices.push_back(uvIndex[2]);
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    }
  }

  for (int i = 0; i < vertexIndices.size(); i++) {
    unsigned int vertex_index = vertexIndices[i];
    glm::vec3 vertex = temp_vertices[vertex_index - 1];
    out_vertices.push_back(vertex);
  }

  for (int i = 0; i < uvIndices.size(); i++) {
    unsigned int uv_index = uvIndices[i];
    glm::vec2 uv = temp_uvs[uv_index - 1];
    out_uvs.push_back(uv);
  }

  for (int i = 0; i < normalIndices.size(); i++) {
    unsigned int normal_index = normalIndices[i];
    glm::vec3 normal = temp_normals[normal_index - 1];
    out_normals.push_back(normal);
  }
  return true;
}