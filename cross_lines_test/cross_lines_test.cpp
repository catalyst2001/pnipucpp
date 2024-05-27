#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int lines_intersection(glm::vec2 &dst_pt,
  const glm::vec2 &l1b, const glm::vec2 &l1e,
  const glm::vec2 &l2b, const glm::vec2 &l2e) {

  // compute 1-st equation odds
  float A1 = l1e.y - l1b.y;
  float B1 = l1b.x - l1e.x;
  float C1 = A1 * l1b.x + B1 * l1b.y;

  // compute 2-st equation odds
  float A2 = l2e.y - l2b.y;
  float B2 = l2b.x - l2e.x;
  float C2 = A2 * l2b.x + B2 * l2b.y;

  // system determinant
  float det = A1 * B2 - A2 * B1;

  // are the lines parallel?
  if (fabsf(det) < FLT_EPSILON)
    return false;

  // calculating the coordinates of the intersection point
  dst_pt.x = (B2 * C1 - B1 * C2) / det;
  dst_pt.y = (A1 * C2 - A2 * C1) / det;
  return true;
}

int main()
{
  glm::vec2 intersection;
  glm::vec2 start(0.f, 0.f);
  glm::vec2 end1(1.f, 10.f);
  glm::vec2 end2(10.f, 1.f);
  if (lines_intersection(intersection, start, end1, start, end2)) {
    printf("lines intersection in point (%f %f)\n", intersection.x, intersection.y);
  }
  return 0;
}