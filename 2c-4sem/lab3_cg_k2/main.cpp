#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#if 1
#define X_ANGLE (51.000f)
#define Y_ANGLE (56.000f)
#define R_VAL (-1.7f)
#else
#define X_ANGLE (0.1f)
#define Y_ANGLE (0.1f)
#define R_VAL (-1.20)
#endif

float     rot_x = X_ANGLE, rot_y = Y_ANGLE, r = R_VAL;
HPEN      pen_black;
HPEN      pen_red;
HPEN      pen_blue;
HPEN      dash_pen;
HINSTANCE g_instance;
HWND      h_wnd;
POINT     curr_cursor, last_cursor;
RECT      rect, oldrect;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

//p: 181.000153   q: 15.599987   r: -0.500000
//p: -21.600180   q : 40.899914   r : -0.500000


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

  inline void log_put_linef(const char *p_format, ...) {
    if (!is_enabled())
      return;

    va_list argptr;
    va_start(argptr, p_format);
    vsnprintf_s(buf, sizeof(buf), p_format, argptr);
    va_end(argptr);
    fprintf(fp, "%s\n", buf);
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
      "%.3f & %.3f & %.3f & %.3f \\\\ "
      "%.3f & %.3f & %.3f & %.3f \\\\ "
      "%.3f & %.3f & %.3f & %.3f \\\\ "
      "%.3f & %.3f & %.3f & %.3f "
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
      fprintf(fp, "%.3f & %.3f & %.3f & %.3f %s ", p_vert->x, p_vert->y, p_vert->z, 1.f, (i + 1 < count) ? "\\\\" : "");
    }
    fputs("\\end{bmatrix}\n", fp);
  }
};

class draw_viewport
{
private:
  const RECT        rect;
  const glm::mat4x4 world;
  HBITMAP           h_bitmap;
  HDC               h_memdc;
  SIZE              viewport_size;
  HBRUSH            h_old_brush;
  HPEN              h_old_pen;

  void to_screen(glm::vec3 &dst_screen_coord, glm::vec3 model_coord) {
    float hsw = viewport_size.cx / 2.f;
    float hsh = viewport_size.cy / 2.f;
    glm::vec4 gcoord = world * glm::vec4(model_coord, 1.f);
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
  draw_viewport(const glm::mat4x4 &worldmat, const RECT &r) : world(worldmat), rect(r) {
    h_bitmap = NULL;
    h_memdc = NULL;
  }
  ~draw_viewport() {}

  void to_screen_project(glm::vec3 &dst_screen_coord, const glm::mat4x4 &proj, glm::vec4 gomcoord) {
    float hsw = viewport_size.cx / 2.f;
    float hsh = viewport_size.cy / 2.f;
    glm::vec4 gcoord = proj * gomcoord;
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

  void push_brush(HBRUSH hbrush) {
    h_old_brush = (HBRUSH)SelectObject(h_memdc, hbrush);
  }

  void pop_brush() {
    SelectObject(h_memdc, h_old_brush);
  }

  void push_pen(HPEN hpen) {
    h_old_pen = (HPEN)SelectObject(h_memdc, hpen);
  }

  void pop_pen() {
    SelectObject(h_memdc, h_old_pen);
  }

  void begin_frame(HDC hdc) {
    viewport_size.cx = labs(rect.right - rect.left);
    viewport_size.cy = labs(rect.bottom - rect.top);
    h_memdc = CreateCompatibleDC(hdc);
    h_bitmap = CreateCompatibleBitmap(hdc, viewport_size.cx, viewport_size.cy);
    SelectObject(h_memdc, h_bitmap);
    SetBkMode(h_memdc, TRANSPARENT);
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

  static glm::vec3 gcoord_to_NDC(glm::vec4 p1)
  {
    if (p1.w > FLT_EPSILON) {
      p1.x /= p1.w;
      p1.y /= p1.w;
      p1.z /= p1.w;
    }
    return glm::vec3(p1.x, p1.y, p1.z);
  }

  void draw_line_2d(const glm::vec2 &pt1, const glm::vec2 &pt2)
  {
    MoveToEx(h_memdc, (int)pt1.x, (int)pt1.y, NULL);
    LineTo(h_memdc, (int)pt2.x, (int)pt2.y);
  }

  void draw_line_2d(const glm::vec3 &pt1, const glm::vec3 &pt2)
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
        draw_line_2d(vert[0], vert[1]);
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
        draw_line_2d(vert[0], vert[1]);
      }
    }
  }

  void draw_tri(const glm::vec3 *p_verts)
  {
    glm::vec3 p1, p2;
    for (size_t j = 0; j < 3; j++) {
      to_screen(p1, p_verts[j]);
      to_screen(p2, p_verts[(j + 1) % 3]);
      draw_line_2d(p1, p2);
    }
  }

  void project_vertices_to_screen(glm::vec3 *p_dst, const glm::vec3 *p_src, size_t count)
  {
    for (size_t i = 0; i < count; i++)
      to_screen(p_dst[i], p_src[i]);
  }

  void project_vertices(glm::vec3 *p_dst, const glm::vec3 *p_src, size_t count) {
    for (size_t i = 0; i < count; i++) {
      glm::vec4 gcoord = world * glm::vec4(p_src[i], 1.f);
      if (gcoord.w > 0.f) {
        gcoord.x /= gcoord.w;
        gcoord.y /= gcoord.w;
        gcoord.z /= gcoord.w;
      }
      p_dst[i] = gcoord;
    }
  }

  void draw_lines_indexed(const glm::vec3 *p_verts, const unsigned int *p_indices, size_t num_indices)
  {
    glm::vec3 p1, p2;
    for (size_t i = 0; i < num_indices - 1; i += 2) {
      to_screen(p1, p_verts[p_indices[i]]);
      to_screen(p2, p_verts[p_indices[i + 1]]);
      draw_line_2d(p1, p2);
    }
  }

  void text_print(int x, int y, const char *p_format, ...)
  {
    va_list argptr;
    char buffer[1024];
    va_start(argptr, p_format);
    vsprintf_s(buffer, sizeof(buffer), p_format, argptr);
    va_end(argptr);

    TextOutA(h_memdc, x, y, buffer, strlen(buffer));
  }
};

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

const size_t num_verts = sizeof(verts) / sizeof(verts[0]);
glm::vec4    gcoords[num_verts];
glm::vec3    projected[num_verts];
glm::vec3    projected_clip[num_verts];
logmessages  logger(true, "matrices.txt");
glm::vec3    origin;
glm::vec3    angles;
glm::vec3    viewpoint;
float        scale = 0.5f;

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

unsigned int parallels[][2] = {
  /* Z */
  { 0, 1 },
  { 3, 2 },
  { 7, 6 },
  { 4, 5 },

  /* Y */
  { 0, 4 },
  { 3, 7 },
  { 2, 6 },
  { 1, 5 },

  /* X */
  { 0, 3 },
  { 1, 2 },
  { 5, 6 },
  { 4, 7 },

  /* roof -Z */
  { 5, 8 },
  { 6, 9 },

  /* roof +Z */
  { 4, 8 },
  { 7, 9 }
};

inline glm::vec4 to_gomogen_coord(const glm::mat4x4 &SRc, const glm::vec3 &in)
{
  glm::vec4 coord = SRc * glm::vec4(in, 1.f);
  //if (coord.w > FLT_EPSILON) {
  //  coord.x /= coord.w;
  //  coord.y /= coord.w;
  //  coord.z /= coord.w;
  //}
  return coord;
}

int z_compare(void const *a, void const *b)
{
  glm::vec4 *p_src = (glm::vec4 *)a;
  glm::vec4 *p_to_compare = (glm::vec4 *)b;
  if (p_src->z < p_to_compare->z)
    return -1;

  return 1;
}

void print_coords(const char *p_label, glm::vec4 *p_src, size_t count)
{
  printf("--- %s:\n", p_label);
  for (size_t i = 0; i < count; i++) {
    glm::vec4 *p_cd = &p_src[i];
    printf("   %f %f %f %f\n", p_cd->x, p_cd->y, p_cd->z, p_cd->w);
  }
  printf("\n\n");
}

void draw_vanishing_points(draw_viewport &viewport)
{
  size_t i;
  glm::mat4x4 rx = glm::rotate(glm::mat4x4(1.f), glm::radians(rot_x), glm::vec3(1.f, 0.f, 0.f));
  glm::mat4x4 ry = glm::rotate(glm::mat4x4(1.f), glm::radians(rot_y + 90.f), glm::vec3(0.f, 1.f, 0.f));
  glm::mat4x4 SRc = ry * rx; //scale-rotatex-rotatey complex matrix

  glm::mat4x4 projection = glm::mat4x4(
    glm::vec4(scale, 0.f,   0.f,   0.f),
    glm::vec4(0.f,   scale, 0.f,   0.f),
    glm::vec4(0.f,   0.f,   scale, r  ),
    glm::vec4(0.f,   0.f,   0.f,   1.f)
  );

  /* transform all vertices to gomogen coords */
  for (size_t i = 0; i < (sizeof(gcoords) / sizeof(gcoords[0])); i++)
    gcoords[i] = to_gomogen_coord(SRc, verts[i]);

  /* sort Z */
  print_coords("without qsort Z", gcoords, num_verts);
  qsort(gcoords, num_verts, sizeof(glm::vec4), &z_compare);
  print_coords("with qsort Z", gcoords, num_verts);

  /* draw 4 points */
  for (size_t i = 0; i < 4; i++) {
    glm::vec4 *p_curgcoord = &gcoords[i];
    glm::vec3 c2d1;
    glm::vec3 c2d2;
    viewport.to_screen_project(c2d2, projection, gcoords[i]);
    viewport.to_screen_project(c2d1, projection, glm::vec4(p_curgcoord->x, p_curgcoord->y, p_curgcoord->z, 100.00001f)); //INFINITY w=0
    viewport.draw_line_2d(c2d1, c2d2);
  }
}

glm::vec3 perspective_trimetric_old(glm::vec3 in, glm::vec3 &begin, float _theta, float _phi, float _rho) {
  float rez;
  float deg2rad = atanf(1.f) / 45.f;

  float th = _theta * deg2rad;
  float ph = (_phi + 90) * deg2rad;
  float costh = cosf(th);
  float sinth = sinf(th);
  float cosph = cosf(ph);
  float sinph = sinf(ph);

  float v11 = -sinth;
  float v12 = -cosph * costh;
  float v13 = -sinph * costh;
  float v21 = costh;
  float v22 = -cosph * sinth;
  float v23 = -sinph * sinth;
  float v32 = sinph;
  float v33 = -cosph;
  float v43 = _rho;

  // 
  // -sinth, -cosph * costh, 0               0
  // 0       1               -sinph * sinth  0
  // 0       sinph           -cosph          _rho
  // 0       0               0               1
  // 

  float xe = v11 * in.x + v21 * in.y;
  float ye = v12 * in.x + v22 * in.y + v32 * in.z;
  float ze = v13 * in.x + v23 * in.y + v33 * in.z + v43;

  float half_screen_width = rect.right / 2.f;
  float half_screen_height = rect.bottom / 2.f;
  float pX = 45 * _rho * xe / ze + half_screen_width - (2 * begin.y);
  float pY = 45 * _rho * ye / ze + half_screen_height - (2 * begin.z);
  return glm::vec3(pX, pY, 1.f);
}

glm::mat4x4 trimetric_project_matrix(float p, float q, float r)
{
  logger.log_put_line("==== trimetric_project_matrix ====");
  logger.log_put_linef("  angle p=%.3f", p);
  logger.log_put_linef("  angle q=%.3f", q);
  logger.log_put_linef("  r=%.3f", r);
  logger.log_put("\n");

  glm::mat4x4 projection = glm::mat4x4(
    glm::vec4(1.f,   0.f,   0.f,   0.f),
    glm::vec4(0.f,   1.f,   0.f,   0.f),
    glm::vec4(0.f,   0.f,   0.f,   -1.f / r),
    glm::vec4(0.f,   0.f,   0.f,   1.f)
  );
  logger.latex_print_mat4x4_with_equal_varname("P", projection);

  glm::mat4x4 rx = glm::rotate(glm::mat4x4(1.f), glm::radians(p), glm::vec3(1.f, 0.f, 0.f));
  logger.latex_print_mat4x4_with_equal_varname("Rx", rx);

  glm::mat4x4 ry = glm::rotate(glm::mat4x4(1.f), glm::radians(q + 90.f), glm::vec3(0.f, 1.f, 0.f));
  logger.latex_print_mat4x4_with_equal_varname("Ry", ry);

  glm::mat4x4 complex_mat = projection * ry * rx;

  logger.log_put_line("Tproj = P * Rx * Ry");
  logger.latex_print_mat4x4_with_equal_varname("Tproj", complex_mat);
  logger.log_put_line("==== trimetric_project_matrix end ====");
  return complex_mat;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("conout$", "w", stdout);
	printf("OK\n");

	WNDCLASSEXA wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = "WindowClass";
	if (!RegisterClassExA(&wcex)) {
		MessageBoxA(HWND_DESKTOP, "Couldn't create window class", "Critical error", MB_OK | MB_ICONERROR);
		return 1;
	}

	g_instance = hInstance;
	INT width = 800;
	INT height = 600;
	INT xpos = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
	INT ypos = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);
	h_wnd = CreateWindowExA(0, wcex.lpszClassName, "lab3 trimetric projection", WS_OVERLAPPEDWINDOW, xpos, ypos, width, height, NULL, NULL, hInstance, NULL);
	if (!h_wnd) {
		MessageBoxA(HWND_DESKTOP, "Couldn't create window", "Critical error", MB_OK | MB_ICONERROR);
		return 2;
	}

  pen_black = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	pen_red = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pen_blue = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
  dash_pen = CreatePen(PS_DASH, 1, RGB(200, 200, 200));

	ShowWindow(h_wnd, SW_SHOW);
	UpdateWindow(h_wnd);

	MSG msg;
	while (GetMessageA(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
  return 0;
}

void clear_color_buffer(HDC hdc)
{
	FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

void draw_line(HDC hdc, int x0, int y0, int x1, int y1)
{
	POINT pt;
	MoveToEx(hdc, x0, y0, &pt);
	LineTo(hdc, x1, y1);
	MoveToEx(hdc, pt.x, pt.y, &pt);
}

int lines_intersection(glm::vec2 &dst_pt,
  const glm::vec2 &l1b, const glm::vec2 &l1e,
  const glm::vec2 &l2b, const glm::vec2 &l2e)
{

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

int lines_intersection_debug(glm::vec2 &dst_pt,
  const glm::vec2 &l1b, const glm::vec2 &l1e,
  const glm::vec2 &l2b, const glm::vec2 &l2e, const char *p_tag)
{
  bool b_ret = false;
  logger.log_put_linef("=============== lines_intersection_debug(%s) BEGIN ===============", p_tag);
  logger.log_put_linef(
    "\nlines:\n"
    "x1, y1 - begin 1 line\n"
    "x2, y2 - end 1 line\n"
    "x3, y3 - begin 2 line\n"
    "x4, y4 - end 2 line\n"
    "x1 = %.3f\n"
    "y1 = %.3f\n"
    "x2 = %.3f\n"
    "y2 = %.3f\n"
    "x3 = %.3f\n"
    "y3 = %.3f\n"
    "x4 = %.3f\n"
    "y4 = %.3f\n",
    l1b.x, l1b.y,
    l1e.x, l1e.y,
    l2b.x, l2b.y,
    l2e.x, l2e.y
  );

  // compute 1-st equation odds
  float A1 = l1e.y - l1b.y;
  float B1 = l1b.x - l1e.x;
  float C1 = A1 * l1b.x + B1 * l1b.y;
  logger.log_put_line("\n//compute 1-st equation odds");
  logger.log_put_line(
    "A1 = y2 - y1\n"
    "B1 = x1 - x2\n"
    "C1 = A1 * x1 + B1 * y1\n"
  );
  logger.log_put_linef("A1 = %.3f - %.3f = %.3f", l1e.y, l1b.y, A1);
  logger.log_put_linef("B1 = %.3f - %.3f = %.3f", l1b.x, l1e.x, B1);
  logger.log_put_linef("C1 = %.3f * %.3f + %.3f * %.3f = %.3f", A1, l1b.x, B1, l1b.y, C1);

  // compute 2-st equation odds
  float A2 = l2e.y - l2b.y;
  float B2 = l2b.x - l2e.x;
  float C2 = A2 * l2b.x + B2 * l2b.y;
  logger.log_put_line("\n//compute 2-st equation odds");
  logger.log_put_line(
    "A2 = y4 - y3\n"
    "B2 = x3 - x4\n"
    "C2 = A2 * x3 + B2 * y3\n"
  );
  logger.log_put_linef("A2 = %.3f - %.3f = %.3f", l2e.y, l2b.y, A2);
  logger.log_put_linef("B2 = %.3f - %.3f = %.3f", l2b.x, l2e.x, B2);
  logger.log_put_linef("C2 = %.3f * %.3f + %.3f * %.3f = %.3f", A2, l2b.x, B2, l2b.y, C2);

  // system determinant
  float det = A1 * B2 - A2 * B1;
  logger.log_put_line("\n//system determinant");
  logger.log_put_linef("det = %.3f * %.3f - %.3f * %.3f = %.3f", A1, B2, A2, B1, det);

  // are the lines parallel?
  if (fabsf(det) > FLT_EPSILON) {
    logger.log_put_line("\ndet != 0");

    // calculating the coordinates of the intersection point
    dst_pt.x = (B2 * C1 - B1 * C2) / det;
    logger.log_put_linef(
      "x = (B2 * C1 - B1 * C2) / det\n"
      "x = \\frac{%.3f * %.3f - %.3f * %.3f}{%.3f} = %.3f", 
      B2, C1, B1, C2, det, dst_pt.x
      );

    dst_pt.y = (A1 * C2 - A2 * C1) / det;
    logger.log_put_linef(
      "y = (A1 * C2 - A2 * C1) / det\n"
      "y = \\frac{%.3f * %.3f - %.3f * %.3f}{%.3f} = %.3f",
      A1, C2, A2, C1, det, dst_pt.y
    );

    b_ret = true; // intersect exists
  }
  logger.log_put_linef("=============== lines_intersection_debug(%s) END ===============", p_tag);
  return b_ret;
}

#if defined (_DEBUG)
#define lines_intersection_tagged(d, l11, l12, l21, l22, tag)\
  lines_intersection_debug(d, l11, l12, l21, l22, tag)
#else
#define lines_intersection_tagged(d, l11, l12, l21, l22, tag)\
  lines_intersection(d, l11, l12, l21, l22)
#endif

template<typename type>
type min(const type &a, const type &b)
{
  if (a < b)
    return a;

  return b;
}

template<typename type>
type max(const type &a, const type &b)
{
  if (a > b)
    return a;

  return b;
}

bool is_positive(const glm::vec2 &v)
{
  return v.x > 0.f && v.y > 0.f;
}

glm::vec2 center_of_line(const glm::vec2 &line_from, const glm::vec2 &line_to)
{
  glm::vec2 dst;
  glm::vec2 pmin, pmax;
  pmin.x = min(line_from.x, line_to.x);
  pmin.y = min(line_from.y, line_to.y);
  pmax.x = max(line_from.x, line_to.x);
  pmax.y = max(line_from.y, line_to.y);
  float half_x = fabsf(pmax.x - pmin.x) / 2.f;
  float half_y = fabsf(pmax.y - pmin.y) / 2.f;
  dst.x = pmax.x - half_x;
  dst.y = pmax.y - half_y;
  return dst;
}

void paint_scene(HDC hdc, HWND hwnd, RECT &rect)
{
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 begin(0.f, 0.f, 0.f);

  logger.log_put_line("=============== paint_scene Begin ===============");

  /* draw scene */
  glm::mat4x4 scale_mat =  glm::scale(glm::mat4x4(1.f), glm::vec3(scale, scale, scale));
  logger.latex_print_mat4x4_with_equal_varname("Scl", scale_mat);

  glm::mat4x4 projection = trimetric_project_matrix(rot_x, rot_y, r);

  glm::mat4x4 complex_matrix = projection * scale_mat;
  logger.log_put_line("CM = Tproj * Scl");
  logger.latex_print_mat4x4_with_equal_varname("CM", complex_matrix);

  draw_viewport viewport(complex_matrix, rect);
  viewport.begin_frame(hdc);
  viewport.clear((HBRUSH)(COLOR_WINDOW + 1));
  viewport.project_vertices_to_screen(projected, verts, num_verts);
  viewport.project_vertices(projected_clip, verts, num_verts);

  logger.latex_print_vertices(verts, sizeof(verts) / sizeof(verts[0]));
  logger.log_put(" * CM = ");
  logger.latex_print_vertices(projected, sizeof(projected) / sizeof(projected[0]));

  //draw_vanishing_points(viewport);
  //if (fabsf(r) > 0.0001f) {
    viewport.push_pen(dash_pen);

    /* Z */
    glm::vec2 endpos;
    int vanishing_points = 0;
    glm::vec2 line1b = glm::vec2(projected[parallels[0][0]].x, projected[parallels[0][0]].y);
    glm::vec2 line1e = glm::vec2(projected[parallels[0][1]].x, projected[parallels[0][1]].y);
    glm::vec2 line2a = glm::vec2(projected[parallels[1][0]].x, projected[parallels[1][0]].y);
    glm::vec2 line2b = glm::vec2(projected[parallels[1][1]].x, projected[parallels[1][1]].y);

    float length = glm::length(line1b - line1e);
    glm::vec2 center = center_of_line(line1b, line1e);
    viewport.text_print(center.x, center.y, "len %.3f", length);
    viewport.text_print(line1b.x, line1b.y, "b %.3f %.3f", line1b.x, line1b.y);
    viewport.text_print(line1e.x, line1e.y, "e %.3f %.3f", line1e.x, line1e.y);
    length = glm::length(line2a - line2b);
    center = center_of_line(line2a, line2b);
    viewport.text_print(center.x, center.y, "len %.3f", length);
    viewport.text_print(line2a.x, line2a.y, "b %.3f %.3f", line2a.x, line2a.y);
    viewport.text_print(line2b.x, line2b.y, "e %.3f %.3f", line2b.x, line2b.y);
    
    if (lines_intersection_tagged(endpos, line1b, line1e, line2a, line2b, "Z")) {
      viewport.draw_line_2d(line1b, endpos);
      viewport.draw_line_2d(line2a, endpos);
      viewport.draw_line_2d(glm::vec2(projected[parallels[2][0]].x, projected[parallels[2][0]].y), endpos);
      viewport.draw_line_2d(glm::vec2(projected[parallels[3][0]].x, projected[parallels[3][0]].y), endpos);
      viewport.text_print(endpos.x, endpos.y, "Z ( %.2f %.2f )", endpos.x, endpos.y);
      vanishing_points++;
    }

    /* Y */
    line1b = glm::vec2(projected[parallels[4][0]].x, projected[parallels[4][0]].y);
    line1e = glm::vec2(projected[parallels[4][1]].x, projected[parallels[4][1]].y);
    line2a = glm::vec2(projected[parallels[5][0]].x, projected[parallels[5][0]].y);
    line2b = glm::vec2(projected[parallels[5][1]].x, projected[parallels[5][1]].y);
    if (lines_intersection_tagged(endpos, line1b, line1e, line2a, line2b, "Y")) {
      viewport.draw_line_2d(line1b, endpos);
      viewport.draw_line_2d(line2a, endpos);
      viewport.draw_line_2d(glm::vec2(projected[parallels[6][0]].x, projected[parallels[6][0]].y), endpos);
      viewport.draw_line_2d(glm::vec2(projected[parallels[7][0]].x, projected[parallels[7][0]].y), endpos);
      viewport.text_print(endpos.x, endpos.y, "Y ( %.2f %.2f )", endpos.x, endpos.y);
      vanishing_points++;
    }

    /* X */
    line1b = glm::vec2(projected[parallels[8][0]].x, projected[parallels[8][0]].y);
    line1e = glm::vec2(projected[parallels[8][1]].x, projected[parallels[8][1]].y);
    line2a = glm::vec2(projected[parallels[9][0]].x, projected[parallels[9][0]].y);
    line2b = glm::vec2(projected[parallels[9][1]].x, projected[parallels[9][1]].y);
    if (lines_intersection_tagged(endpos, line1b, line1e, line2a, line2b, "X")) {
      viewport.draw_line_2d(line1b, endpos);
      viewport.draw_line_2d(line2a, endpos);
      viewport.draw_line_2d(glm::vec2(projected[parallels[10][0]].x, projected[parallels[10][0]].y), endpos);
      viewport.draw_line_2d(glm::vec2(projected[parallels[11][0]].x, projected[parallels[11][0]].y), endpos);
      viewport.text_print(endpos.x, endpos.y, "X ( %.2f %.2f )", endpos.x, endpos.y);
      vanishing_points++;
    }

    /* crysha left */
    line1b = glm::vec2(projected[parallels[12][0]].x, projected[parallels[12][0]].y);
    line1e = glm::vec2(projected[parallels[12][1]].x, projected[parallels[12][1]].y);
    line2a = glm::vec2(projected[parallels[13][0]].x, projected[parallels[13][0]].y);
    line2b = glm::vec2(projected[parallels[13][1]].x, projected[parallels[13][1]].y);
    if (lines_intersection_tagged(endpos, line1b, line1e, line2a, line2b, "roof left")) {
      viewport.draw_line_2d(line1b, endpos);
      viewport.draw_line_2d(line2b, endpos);
      viewport.text_print(endpos.x, endpos.y, "( %.2f %.2f )", endpos.x, endpos.y);
      vanishing_points++;
    }

    /* crysha left */
    line1b = glm::vec2(projected[parallels[14][0]].x, projected[parallels[14][0]].y);
    line1e = glm::vec2(projected[parallels[14][1]].x, projected[parallels[14][1]].y);
    line2a = glm::vec2(projected[parallels[15][0]].x, projected[parallels[15][0]].y);
    line2b = glm::vec2(projected[parallels[15][1]].x, projected[parallels[15][1]].y);
    if (lines_intersection_tagged(endpos, line1b, line1e, line2a, line2b, "roof right")) {
      viewport.draw_line_2d(line1b, endpos);
      viewport.draw_line_2d(line2b, endpos);
      viewport.text_print(endpos.x, endpos.y, "( %.2f %.2f )", endpos.x, endpos.y);
      vanishing_points++;
    }
    viewport.pop_pen();

    /* scene distortion factor */
    glm::vec3 distortion_factor;

    /* transform with projection */
    //for (size_t i = 0; i < num_verts; i++)
    //  projected[i] = complex_matrix * glm::vec4(verts[i], 1.f);

    auto projected_line_length = [](int line_start_idx, int line_end_idx) -> float {
      return glm::length(
        glm::vec2(projected_clip[line_start_idx].x, projected_clip[line_start_idx].y) - glm::vec2(projected_clip[line_end_idx].x, projected_clip[line_end_idx].y)
      );
    };

    auto source_line_length = [](int line_start_idx, int line_end_idx) -> float {
      float line_length = glm::length(verts[line_start_idx] - verts[line_end_idx]);
      if (fabsf(line_length) < FLT_EPSILON) // check division by zero
        line_length = 1.f;

      return line_length;
    };

    /* distortion factor from parallel lines */
    //distortion_factor.x = projected_line_length(0, 3) / source_line_length(0, 3);
    //distortion_factor.y = projected_line_length(0, 4) / source_line_length(0, 4);
    //distortion_factor.z = projected_line_length(0, 1) / source_line_length(0, 1);
    //printf("distortion_factor:  %.3f %.3f %.3f\n", distortion_factor.x, distortion_factor.y, distortion_factor.z);

    /* distortion factor from matrix */
    distortion_factor.x = complex_matrix[0][3];
    distortion_factor.y = complex_matrix[1][3];
    distortion_factor.z = complex_matrix[2][3];
    //printf("distortion_factor_from_mat:  %.3f %.3f %.3f\n", distortion_factor_from_mat.x, distortion_factor_from_mat.y, distortion_factor_from_mat.z);


    float scene_distortion = sqrtf(distortion_factor.x*distortion_factor.x + distortion_factor.y*distortion_factor.y + distortion_factor.z*distortion_factor.z);

    viewport.text_print(10, 10, "vanishing points count %d", vanishing_points);
    viewport.text_print(10, 30, "distortion  x: %f y: %f z: %f", distortion_factor.x, distortion_factor.y, distortion_factor.z);
    viewport.text_print(10, 50, "scene distortion: %f", scene_distortion);
  //}

  viewport.push_pen(pen_black);
  viewport.draw_lines_indexed(verts, indices, sizeof(indices) / sizeof(indices[0]));
  viewport.pop_pen();

  viewport.text_print(10, 80, "angle x=%.2f°", rot_x);
  viewport.text_print(10, 100, "angle y=%.2f°", rot_y);
  viewport.text_print(10, 120, "r=%.2f", r);
  viewport.text_print(10, 140, "-1/r=%.2f", -1.f/r);

  logger.log_put_line("=============== paint_scene End ===============");
  viewport.end_frame(hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool buttons[1024];
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case 1:
				break;

            default:
                return DefWindowProcA(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_SIZE:
	  InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_MOUSEWHEEL: {
		SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

    if (!buttons[VK_CONTROL]) {
      if (zDelta < 0)
        r -= 0.01f;
      else r += 0.01f;
    }
    else
    {
      if (zDelta < 0)
        scale -= 0.01f;
      else scale += 0.01f;
    }
    printf("rot_x: %f   rot_y: %f   r: %f\n", rot_x, rot_y, r);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_PAINT: {
		PAINTSTRUCT ps;
		GetClientRect(h_wnd, &rect);
    if (!rect.bottom)
      rect.bottom = 1;

		HDC hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		paint_scene(hdc, hWnd, rect);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_MOUSEMOVE:
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			curr_cursor.x = LOWORD(lParam);
			curr_cursor.y = HIWORD(lParam);

			POINT diffpt;
			diffpt.y = curr_cursor.x - last_cursor.x;
			diffpt.x = curr_cursor.y - last_cursor.y;
			if (diffpt.x || diffpt.y) {
				rot_x += diffpt.x * -0.1f;
				rot_y += diffpt.y * 0.1f;
				printf("p: %f   q: %f   r: %f\n", rot_x, rot_y, r);			
			}
			last_cursor = curr_cursor;

			// setup matrix
			//view = glm::translate(mat_identity, glm::vec3(0, 0, dist));
			//view = glm::rotate(view, glm::radians(angles.x), glm::vec3(1, 0, 0));
			//view = glm::rotate(view, glm::radians(angles.y), glm::vec3(0, 1, 0));
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
		curr_cursor.x = LOWORD(lParam);
		curr_cursor.y = HIWORD(lParam);
		last_cursor = curr_cursor;
		break;

  case WM_DESTROY:
      PostQuitMessage(0);
      break;

	case WM_KEYDOWN:
		buttons[wParam] = true;
		break;

	case WM_KEYUP:
		buttons[wParam] = false;
		break;

	case WM_ERASEBKGND:
		return 1;

    default:
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    return 0;
}