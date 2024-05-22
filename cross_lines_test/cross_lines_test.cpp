#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct segment {
  glm::vec2 begin;
  glm::vec2 end;
};

bool cross_lines(glm::vec2 &dst, glm::vec2 &line1b, glm::vec2 &line1e, glm::vec2 &line2b, glm::vec2 &line2e) {
  float n;
  if (line1e.y - line1b.y != 0) {  // a(y)
    float q = (line1e.x - line1b.x) / (line1b.y - line1e.y);
    float sn = (line2b.x - line2e.x) + (line2b.y - line2e.y) * q;
    if (!sn)
      return false;  // c(x) + c(y)*q

    float fn = (line2b.x - line1b.x) + (line2b.y - line1b.y) * q;   // b(x) + b(y)*q
    n = fn / sn;
  }
  else {
    if (!(line2b.y - line2e.y))
      return false; // b(y)

    n = (line2b.y - line1b.y) / (line2b.y - line2e.y);   // c(y)/b(y)
  }
  dst.x = line2b.x + (line2e.x - line2b.x) * n;
  dst.y = line2b.y + (line2e.y - line2b.y) * n;
  return true;
}


bool cross_lines(const segment &seg1, const segment &seg2)
{

}

int main()
{



  return 0;
}