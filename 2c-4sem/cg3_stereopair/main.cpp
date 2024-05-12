/**
* Deryabin K.
* 
* Changelog:
*  Edited: 12.04.2024 (OK)
* 
* 
*
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <Windows.h>

/* math lib */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAIN_WINDOW_CLASS "main_window"
#define DRAW_VIEWPORT_CLASS "draw_viewport"

LRESULT CALLBACK draw_viewport_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK main_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

bool register_classes() {
  WNDCLASSA wca;
  memset(&wca, 0, sizeof(wca));
  wca.style = CS_HREDRAW | CS_VREDRAW;
  wca.hCursor = LoadCursor(NULL, IDC_ARROW);
  wca.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
  wca.hInstance = GetModuleHandleW(0);
  wca.lpszClassName = DRAW_VIEWPORT_CLASS;
  wca.lpfnWndProc = draw_viewport_wndproc;
  if (!RegisterClassA(&wca)) {
    printf("failed to register 'draw_viewport' class. LastError()=%d\n", GetLastError());
    return false;
  }

  wca.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
  wca.lpszClassName = MAIN_WINDOW_CLASS;
  wca.lpfnWndProc = main_wndproc;
  if (!RegisterClassA(&wca)) {
    printf("failed to register 'main_window' class. LastError()=%d\n", GetLastError());
    return false;
  }
  return true;
}

class logmessages
{
  FILE *fp;
  char buf[1024];

public:
  logmessages(bool b_use_logging, const char *p_filename) : fp(nullptr) {
    if (b_use_logging) {
      fp = fopen(p_filename, "wt");
      log_put_line("~~~~~~~~~~~~~~ log opened ~~~~~~~~~~~~~~");
    }
  }

  ~logmessages() {
    if (fp) {
      log_put_line("~~~~~~~~~~~~~~ log closed ~~~~~~~~~~~~~~");
      fflush(fp);
      fclose(fp);
      fp = nullptr;
    }
  }

  inline void log_put_line(const char *p_text) {
    if (fp) {
      fprintf(fp, "%s\n", p_text);
    }
  }

  inline void log_put(const char *p_string) {
    if (fp) {
      fputs(p_string, fp);
    }
  }

  inline bool is_enabled() { return fp != nullptr; }

  void latex_print_mat4x4_with_equal_varname(const char *p_math_var_name, const glm::mat4x4 &mat) {
    if (!is_enabled())
      return;

    snprintf(buf, sizeof(buf),
      "%s = \\begin{bmatrix}"
      "%.2f & %.2f & %.2f & %.2f \\\\ "
      "%.2f & %.2f & %.2f & %.2f \\\\ "
      "%.2f & %.2f & %.2f & %.2f \\\\ "
      "%.2f & %.2f & %.2f & %.2f "
      "\\end{bmatrix}\\\\",
      p_math_var_name,
      mat[0][0], mat[0][1], mat[0][2], mat[0][3],
      mat[1][0], mat[1][1], mat[1][2], mat[1][3],
      mat[2][0], mat[2][1], mat[2][2], mat[2][3],
      mat[3][0], mat[3][1], mat[3][2], mat[3][3]
    );
    log_put_line(buf);
  }

  void latex_print_vertices(const glm::vec3 *p_src, size_t count) {
    if (!is_enabled())
      return;

    /* begin latex matrix header */
    fputs("\\begin{bmatrix}", fp);
    for (size_t i = 0; i < count; i++) {
      const glm::vec3 *p_vert = &p_src[i];

      /* x y z w ('\\' or '') */
      fprintf(fp, "%.2f & %.2f & %.2f & %.2f %s ", p_vert->x, p_vert->y, p_vert->z, 1.f, (i+1 < count) ? "\\\\" : "");
    }
    fputs("\\end{bmatrix}\n", fp);
  }
};

struct transform_s
{
  glm::mat4x4 world; /* complex matrix */
  glm::mat4x4 model; /* model transforms */
  glm::mat4x4 view; /* view transforms */
  glm::mat4x4 projection; /* projection */

  glm::vec3 origin;
  glm::vec3 angles;
  glm::vec3 viewpoint;

  transform_s() {
    world = glm::mat4x4(1.f);
    model = glm::mat4x4(1.f);
    view = glm::mat4x4(1.f);
    projection = glm::mat4x4(1.f);
  }
  ~transform_s() {}
};

class draw_viewport
{
private:
  const transform_s &t;
  const RECT  &rect;
  HBITMAP    h_bitmap;
  HDC        h_memdc;
  SIZE       viewport_size;

  void to_screen(glm::vec3 &dst_screen_coord, glm::vec3 model_coord) {
    float hsw = viewport_size.cx / 2.f;
    float hsh = viewport_size.cy / 2.f;
    glm::vec4 gcoord = t.projection * t.model * glm::vec4(model_coord, 1.f);
    if (gcoord.w > 0.f) {
      gcoord.x /= gcoord.w;
      gcoord.y /= gcoord.w;
      gcoord.z /= gcoord.w;
    }
    gcoord.x *= hsw;
    gcoord.y *= -hsh;
    gcoord.x += hsw;
    gcoord.y += hsh;
    dst_screen_coord.x = gcoord.x;
    dst_screen_coord.y = gcoord.y;
    dst_screen_coord.z = gcoord.z;
  }

  void test_rects_draw() {
    RECT rct;
    rct.left = 0;
    rct.top = 0;
    rct.right = rct.left + 100;
    rct.bottom = rct.top + 100;
    FillRect(h_memdc, &rct, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

    rct.left = 100;
    rct.top = 100;
    rct.right = rct.left + 100;
    rct.bottom = rct.top + 100;
    FillRect(h_memdc, &rct, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
  }

public:
  /*
   _transform - transformations
   r - drawing rect in screen coords
  */
  draw_viewport(const transform_s &_transform, RECT &r) : t(_transform), rect(r) {
    h_bitmap = NULL;
    h_memdc = NULL;
  }
  ~draw_viewport() {}

  void begin_frame(HDC hdc) {
    viewport_size.cx = labs(rect.right - rect.left);
    viewport_size.cy = labs(rect.bottom - rect.top);
    h_memdc = CreateCompatibleDC(hdc);
    h_bitmap = CreateCompatibleBitmap(hdc, viewport_size.cx, viewport_size.cy);
    SelectObject(h_memdc, h_bitmap);
    printf("begin_frame  l(%d) t(%d) r(%d) b(%d)\n", rect.left, rect.top, rect.right, rect.bottom);
  }

  void end_frame(HDC hdc) {
    if (!BitBlt(hdc, rect.left, rect.top, viewport_size.cx, viewport_size.cy, h_memdc, 0, 0, SRCCOPY))
      printf("BitBlt Failed\n");

    if (h_memdc) {
      DeleteDC(h_memdc);
      h_memdc = NULL;
    }

    if (h_bitmap) {
      DeleteObject(h_bitmap);
      h_bitmap = NULL;
    }
  }

  void clear(HBRUSH brush) {
    RECT norm_rect;
    norm_rect.left = 0;
    norm_rect.top = 0;
    norm_rect.right = viewport_size.cx;
    norm_rect.bottom = viewport_size.cy;
    FillRect(h_memdc, &norm_rect, brush);
  }

  void draw_line(const glm::vec3 &pt1, const glm::vec3 &pt2)
  {
    MoveToEx(h_memdc, (int)pt1.x, (int)pt1.y, NULL);
    LineTo(h_memdc, (int)pt2.x, (int)pt2.y);
  }

  void draw_triangles_indexed(const glm::vec3 *p_verts, const unsigned int *p_indices, size_t num_indices)
  {
    glm::vec3 vert[2];
    size_t num_tris = num_indices / 3;
    for (size_t i = 0; i < num_tris; i++) {
      for (size_t j = 0; j < 3; j++) {
        to_screen(vert[0], p_verts[p_indices[i + j + 0]]);
        to_screen(vert[1], p_verts[p_indices[i + (j + 1) % 3]]);
        draw_line(vert[0], vert[1]);
      }
    }
  }

  void draw_triangles(const glm::vec3 *p_verts, size_t num_verts)
  {
    glm::vec3 vert[2];
    size_t num_tris = num_verts / 3;
    for (size_t i = 0; i < num_tris; i++) {
      for (size_t j = 0; j < 3; j++) {
        to_screen(vert[0], p_verts[i + j + 0]);
        to_screen(vert[1], p_verts[i + (j + 1) % 3]);
        draw_line(vert[0], vert[1]);
      }
    }
  }

  void draw_tri(const glm::vec3 *p_verts)
  {
    glm::vec3 p1, p2;
    for (size_t j = 0; j < 3; j++) {
      to_screen(p1, p_verts[j]);
      to_screen(p2, p_verts[(j + 1) % 3]);
      draw_line(p1, p2);
    }
  }

  void project_vertices_to_screen(glm::vec3 *p_dst, const glm::vec3 *p_src, size_t count)
  {
    for (size_t i = 0; i < count; i++)
      to_screen(p_dst[i], p_src[i]);
  }

  void draw_lines_indexed(const glm::vec3 *p_verts, const unsigned int *p_indices, size_t num_indices)
  {
    glm::vec3 p1, p2;
    for (size_t i = 0; i < num_indices - 1; i += 2) {
      to_screen(p1, p_verts[p_indices[i]]);
      to_screen(p2, p_verts[p_indices[i + 1]]);
      draw_line(p1, p2);
    }
  }
};

HWND        h_wnd;
HWND        h_viewport;
transform_s transform;
float       eyes_distance;
HBRUSH      h_background;
HBRUSH      h_background2;
logmessages logger(true, "matrices.txt");

int main()
{
  printf("registering window classes\n");
  if (!register_classes()) {
    printf("failed to register classes");
    return 1;
  }

  int control_width = 200;
  int viewport_width = 800;
  int viewport_height = 600;
  int full_width = viewport_width + control_width;
  int swidth = GetSystemMetrics(SM_CXSCREEN);
  int sheight = GetSystemMetrics(SM_CYSCREEN);
  int xpos = ((swidth - full_width) >> 1);
  int ypos = ((sheight - viewport_height) >> 1);
  h_viewport = CreateWindowExA(0, DRAW_VIEWPORT_CLASS, "Lab3 - Stereopair",
    WS_OVERLAPPEDWINDOW,
    xpos, ypos, viewport_width, viewport_height, h_wnd, (HMENU)NULL, NULL, NULL);
  if (!h_viewport) {
    printf("failed to create viewport window. LastError=%d\n", GetLastError());
    return 1;
  }

  //h_wnd = CreateWindowExA(WS_EX_TOPMOST, MAIN_WINDOW_CLASS, "Lab3 - Stereopair controls", WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,
  //  xpos + viewport_width, ypos, control_width, viewport_height, HWND_DESKTOP, (HMENU)NULL, NULL, NULL);
  //if (!h_wnd) {
  //  printf("failed to create window. LastError=%d\n", GetLastError());
  //  return 1;
  //}

  h_background = CreateSolidBrush(RGB(255, 0, 0));
  h_background2 = CreateSolidBrush(RGB(0, 0, 255));
  //ShowWindow(h_wnd, SW_SHOW);
  //UpdateWindow(h_wnd); 
  ShowWindow(h_viewport, SW_SHOW);
  UpdateWindow(h_viewport);

  MSG msg;
  while (GetMessageA(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }
  return 0;
}

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

glm::vec3 proj_verts[sizeof(verts) / sizeof(verts[0])]; /* for project vertices */

unsigned int indices[] = {
  /* bottom */
  0, 1,
  1, 2,
  2, 3,
  3, 0,

  /* top */
  4, 5,
  5, 6,
  6, 7,
  7, 4,

  /* vertical */
  0, 4,
  1, 5,
  2, 6,
  3, 7,

  /* roof */
  4, 8,
  8, 5,
  7, 9,
  9, 6,
  8, 9
};

unsigned int parallels[] = {
  /* Z */
  0, 1,
  3, 2,

  /* X */
  0, 3,
  1, 2,

  /* Y */
  0, 4,
  3, 7,

  /* roof -Z */
  5, 8,
  6, 9,

  /* roof +Z */
  4, 8,
  7, 9
};

float F = 10.f;
float d = 0.1f;
float z = 0.f;
float eyesd = 5.f;

// z = 0;
// F/20 = d/2
// -F/20 = -d/2
float solve_equation(float z, float d)
{
  // d = z + (F / 10)
  // F = 10d + z
  return 10.f * d + z;
}

void mat4x4_print(const glm::mat4x4 &mat)
{
  printf("------------------\n");
  for (size_t i = 0; i < 4; i++) {
    printf("%.2f %.2f %.2f %.2f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
  }
  printf("------------------\n");
}

void draw_scene(HDC hdc, RECT &rect)
{
  RECT left_eye, right_eye;
  float F = solve_equation(z, d);
  //printf("%f a=%f\n", F, atanf(d / F));

  glm::mat4x4 translate_matrix;
  glm::mat4x4 rotate_x_mat;
  glm::mat4x4 rotate_y_mat;
  glm::mat4x4 complex_mat;

  /* compute transformations */
  if (rect.bottom == 0)
    rect.bottom = 1;

  //float aspect_ratio = (float)(rect.right >> 1) / (float)rect.bottom;

  logger.log_put_line("*** BEGIN eye 1 ***");

  transform.projection = glm::mat4x4(
    glm::vec4(1.f, 0.f, 0.f, 0.f),
    glm::vec4(0.f, 1.f, 0.f, 0.f),
    glm::vec4(0.f, 0.f, 0.f, -1.f/F),
    glm::vec4(eyesd /20.f, 0.f, 0.f, 1.f)
  );
  logger.latex_print_mat4x4_with_equal_varname("P", transform.projection);

  translate_matrix = glm::translate(glm::mat4x4(1.f), glm::vec3(0.f, 0.f, -2.f));
  logger.latex_print_mat4x4_with_equal_varname("T", translate_matrix);

  rotate_x_mat = glm::rotate(glm::mat4x4(1.f), glm::radians(transform.angles.x), glm::vec3(1.f, 0.f, 0.f));
  logger.latex_print_mat4x4_with_equal_varname("Rx", rotate_x_mat);

  rotate_y_mat = glm::rotate(glm::mat4x4(1.f), glm::radians(transform.angles.y), glm::vec3(0.f, 1.f, 0.f));
  logger.latex_print_mat4x4_with_equal_varname("Ry", rotate_y_mat);

  complex_mat = translate_matrix * rotate_x_mat * rotate_y_mat;
  logger.latex_print_mat4x4_with_equal_varname("CM = Rx * Ry * T * P", complex_mat);
  transform.model = complex_mat;

  /* left eye */
  left_eye.left = 0;
  left_eye.top = 0;
  left_eye.right = rect.right >> 1;
  left_eye.bottom = rect.bottom;
  draw_viewport left(transform, left_eye);
  left.begin_frame(hdc);

  /* print src verts for LEFT EYE */
  logger.latex_print_vertices(verts, sizeof(verts) / sizeof(verts[0]));
  logger.log_put(" * CM = "); // in latex: [src verts mat] * CM = 

  /* project verts for LEFT EYE */
  left.project_vertices_to_screen(proj_verts, verts, sizeof(verts) / sizeof(verts[0]));
  
  /* print projected vertices for LEFT EYE */
  logger.latex_print_vertices(proj_verts, sizeof(proj_verts) / sizeof(proj_verts[0]));

  left.clear((HBRUSH)(COLOR_WINDOW + 1));
  left.draw_lines_indexed(verts, indices, sizeof(indices) / sizeof(indices[0]));
  left.end_frame(hdc);
  logger.log_put_line("*** END eye 1 ***\n\n");


  // ------------------------------------------------------------------------------------------------------------------
  logger.log_put_line("*** BEGIN eye 2 ***");
  transform.projection = glm::mat4x4(
    glm::vec4(1.f, 0.f, 0.f, 0.f),
    glm::vec4(0.f, 1.f, 0.f, 0.f),
    glm::vec4(0.f, 0.f, 0.f, -1.f / F),
    glm::vec4(-eyesd /20.f, 0.f, 0.f, 1.f)
  );
  logger.latex_print_mat4x4_with_equal_varname("P", transform.projection);

  translate_matrix = glm::translate(glm::mat4x4(1.f), glm::vec3(0.f, 0.f, -2.f));
  logger.latex_print_mat4x4_with_equal_varname("T", translate_matrix);

  rotate_x_mat = glm::rotate(glm::mat4x4(1.f), glm::radians(transform.angles.x), glm::vec3(1.f, 0.f, 0.f));
  logger.latex_print_mat4x4_with_equal_varname("Rx", rotate_x_mat);

  rotate_y_mat = glm::rotate(glm::mat4x4(1.f), glm::radians(transform.angles.y), glm::vec3(0.f, 1.f, 0.f));
  logger.latex_print_mat4x4_with_equal_varname("Ry", rotate_y_mat);

  complex_mat = translate_matrix * rotate_x_mat * rotate_y_mat;
  logger.latex_print_mat4x4_with_equal_varname("CM = Rx * Ry * T * P", complex_mat);
  transform.model = complex_mat;

  /* right eye */
  right_eye.left = left_eye.right;
  right_eye.top = 0;
  right_eye.right = rect.right;
  right_eye.bottom = rect.bottom;
  draw_viewport right(transform, right_eye);
  right.begin_frame(hdc);

  /* print src verts for RIGHT EYE */
  logger.latex_print_vertices(verts, sizeof(verts) / sizeof(verts[0]));
  logger.log_put(" * CM = "); // in latex: [src verts mat] * CM = 

  /* project verts for RIGHT EYE */
  left.project_vertices_to_screen(proj_verts, verts, sizeof(verts) / sizeof(verts[0]));

  /* print projected vertices for RIGHT EYE */
  logger.latex_print_vertices(proj_verts, sizeof(proj_verts) / sizeof(proj_verts[0]));

  right.clear((HBRUSH)(COLOR_WINDOW + 1));
  right.draw_lines_indexed(verts, indices, sizeof(indices) / sizeof(indices[0]));
  right.end_frame(hdc);
  logger.log_put_line("*** END eye 2 ***\n\n");
}

const float move_scale = 1.f;
const float rot_scale = 1.5f;

void handle_keys(int key)
{
  switch (key)
  {
  case 'W':
    transform.origin.z += move_scale;
    break;

  case 'A':
    transform.origin.x -= move_scale;
    break;

  case 'S':
    transform.origin.z -= move_scale;
    break;

  case 'D':
    transform.origin.x += move_scale;
    break;

  case VK_DOWN:
    transform.angles.x += rot_scale;
    break;

  case VK_UP:
    transform.angles.x -= rot_scale;
    break;

  case VK_LEFT:
    transform.angles.y -= rot_scale;
    break;

  case VK_RIGHT:
    transform.angles.y += rot_scale;
    break;

  case VK_HOME:
    d += 0.001f;
    break;

  case VK_END:
    d -= 0.001f;
    break;

  case VK_PRIOR:
    eyesd += 0.1f;
    break;

  case VK_NEXT:
    eyesd -= 0.1f;
    break;
  }
}

LRESULT CALLBACK draw_viewport_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  static RECT rect;
  switch (msg)
  {
  case WM_CLOSE:
    PostQuitMessage(0);
    break;

  case WM_PAINT: {
    PAINTSTRUCT ps;
    GetClientRect(hwnd, &rect);
    HDC hdc = BeginPaint(hwnd, &ps);
    draw_scene(hdc, rect);
    EndPaint(hwnd, &ps);
    break;
  }

  case WM_KEYDOWN:
    handle_keys(wparam);
    InvalidateRect(hwnd, NULL, TRUE);
    break;

  default:
    return DefWindowProcA(hwnd, msg, wparam, lparam);
  }
  return 0;
}

LRESULT CALLBACK main_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  default:
    return DefWindowProcA(hwnd, msg, wparam, lparam);
  }
  return 0;
}
