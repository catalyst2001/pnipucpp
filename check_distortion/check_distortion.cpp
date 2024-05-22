#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 verts[] = {
  /* bottom */
  glm::vec3(-1, -1, -1),
  glm::vec3(-1, -1, 1),
  glm::vec3(1, -1, 1),
  glm::vec3(1, -1, -1),

  /* top */
  glm::vec3(-1, 1, -1),
  glm::vec3(-1, 1, 1),
  glm::vec3(1, 1, 1),
  glm::vec3(1, 1, -1),

  /* roof */
  glm::vec3(-1, 1.5f, 0),
  glm::vec3(1, 1.5f, 0)
};

const size_t verts_count = sizeof(verts) / sizeof(verts[0]);

glm::vec3 projected[verts_count];

void print_mat4x4(const glm::mat4x4 &mat)
{
  printf("\n");
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      printf("%.3f ", mat[i][j]);
    }
    putc('\n', stdout);
  }
  printf("\n");
}

glm::mat4x4 trimetric_project_matrix(float p, float q, float r)
{
  glm::mat4x4 projection = glm::mat4x4(
    glm::vec4(1.f, 0.f, 0.f, 0.f),
    glm::vec4(0.f, 1.f, 0.f, 0.f),
    glm::vec4(0.f, 0.f, 0.f, r),
    glm::vec4(0.f, 0.f, 0.f, 1.f)
  );

  glm::mat4x4 rx = glm::rotate(glm::mat4x4(1.f), glm::radians(p), glm::vec3(1.f, 0.f, 0.f));
  glm::mat4x4 ry = glm::rotate(glm::mat4x4(1.f), glm::radians(q/* + 90.f*/), glm::vec3(0.f, 1.f, 0.f));
  glm::mat4x4 complex_mat = projection * ry * rx;
  print_mat4x4(complex_mat);
  return complex_mat;
}

void test()
{
  glm::mat4x4 projection = trimetric_project_matrix(0.f, 0.f, 1.f);
  

  glm::vec4 orts[] = {
    glm::vec4(1.f, 0.f, 0.f, 1.f), //x
    glm::vec4(0.f, 1.f, 0.f, 1.f), //y
    glm::vec4(0.f, 0.f, 1.f, 1.f)  //z
  };

  const size_t num_verts = sizeof(orts) / sizeof(orts[0]);
  glm::vec4 projected_orts[num_verts];
  printf("--------- orts -------\n");
  for (size_t i = 0; i < num_verts; i++) {
    glm::vec4 &coord = orts[i];
    printf("%.3f %.3f %.3f %.3f\n", coord.x, coord.y, coord.z, coord.w);
  }
  printf("----------------------\n");

  printf("--------- protected -------\n");
  for (size_t i = 0; i < num_verts; i++) {
    glm::vec4 &coord = projected_orts[i] = projection * orts[i];
    printf("%.3f %.3f %.3f %.3f\n", coord.x, coord.y, coord.z, coord.w);
  }
  printf("----------------------\n");

  float pxlen = glm::length(glm::vec2(projected_orts[0]));
  float pylen = glm::length(glm::vec2(projected_orts[1]));
  float pzlen = glm::length(glm::vec2(projected_orts[2]));
  printf(
    "xd: %.3f\n"
    "yd: %.3f\n"
    "zd: %.3f\n",
    pxlen / 1,
    pylen / 1,
    pzlen / 1
  );
}

int main()
{
  const float p = -45.f;
  const float q = 45.f;
  const float r = 0.5f;
  glm::mat4x4 complex_mat = trimetric_project_matrix(p, q, r);
#if 0
  /* print source verts */
  printf("----- source verts -----\n");
  for (size_t i = 0; i < verts_count; i++) {
    glm::vec3 &coord = verts[i];
    printf("coord %.3f %.3f %.3f\n", coord.x, coord.y, coord.z);
  }
  printf("------------------------\n");

  /* project */
  static glm::vec3 projected[verts_count];
  printf("----- protected verts -----\n");
  for (size_t i = 0; i < verts_count; i++) {
    glm::vec4 gcoord = complex_mat * glm::vec4(verts[i], 1.f);
    printf("coord: %.3f %.3f %.3f %.3f\n", gcoord.x, gcoord.y, gcoord.z, gcoord.w);
    projected[i] = gcoord;
  }
  printf("------------------------\n");

  float srcl = glm::length(glm::vec2(verts[0]));
  printf("src length: %.3f\n", srcl);

  float protectedl = glm::length(glm::vec2(projected[0]));
  printf("protected length: %.3f\n", protectedl);

  if (protectedl < 0.000001f)
    protectedl = 1.f;

  float coeff = protectedl / srcl;
  printf(
    "srcl = %f\n"
    "srcd = %f\n"
    "coeff = %f\n"
    "coeff_mat p = %f\n"
    "coeff_mat q = %f\n"
    "coeff_mat r = %f\n",
    srcl,
    protectedl,
    coeff,
    complex_mat[0][3],
    complex_mat[1][3],
    complex_mat[2][3]
  );
#endif

  //glm::vec4 coord1(1.f, 0.f, 0.f, 1.f);
  //glm::vec4 projected = complex_mat * coord1;

  //float src_len = glm::length(glm::vec3(coord1));
  //float projected_len = glm::length(glm::vec2(projected));
  //float dist1 = projected_len / src_len;
  //printf(
  //  "srcLen: %.3f\n"
  //  "projectedLen: %.3f\n"
  //  "distortion1: %.3f\n",
  //  src_len, projected_len, dist1
  //);
  test();
  return 0;
}