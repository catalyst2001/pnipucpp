#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

HINSTANCE g_instance;
HWND h_wnd;

glm::vec2 angles = glm::vec2(/*-96.f, 89.f*/);
POINT curr_cursor, last_cursor;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
RECT rect, oldrect;
glm::mat4x4 view;
glm::mat4x4 projection;
float dist = -10.f;


glm::mat4x4 mat_identity = glm::mat4x4(1.f);

HPEN pen_red;
HPEN pen_blue;

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
      fprintf(fp, "%.2f & %.2f & %.2f & %.2f %s ", p_vert->x, p_vert->y, p_vert->z, 1.f, (i + 1 < count) ? "\\\\" : "");
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

logmessages logger(true, "matrices.txt");


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
	h_wnd = CreateWindowExA(0, wcex.lpszClassName, "lab3", WS_OVERLAPPEDWINDOW, xpos, ypos, width, height, NULL, NULL, hInstance, NULL);
	if (!h_wnd) {
		MessageBoxA(HWND_DESKTOP, "Couldn't create window", "Critical error", MB_OK | MB_ICONERROR);
		return 2;
	}

	GetClientRect(h_wnd, &rect);
	projection = glm::perspective(45.f, rect.right / (float)rect.bottom, 0.0001f, 1000.f);

	view = glm::translate(mat_identity, glm::vec3(0, 0, dist));
	view = glm::rotate(view, 45.f, glm::vec3(1.f, 1.f, 0.f));

	pen_red = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	pen_blue = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));

	//rotate_model();

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

glm::vec3 perspective_trimetric_project2(glm::vec3 in, glm::vec3 &begin, float _theta, float _phi, float _rho) {
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
	//idx0 = line2b.x + (-b(x))*n
	//idline1b.x = line2b.y +(-b(y))*n

	dst.x = line2b.x + (line2e.x - line2b.x) * n;
	dst.y = line2b.y + (line2e.y - line2b.y) * n;
	return true;
}

void text_print(HDC hdc, int x, int y, const char *p_format, ...)
{
	va_list argptr;
	char buffer[1024];
	va_start(argptr, p_format);
	vsprintf_s(buffer, sizeof(buffer), p_format, argptr);
	va_end(argptr);

	TextOutA(hdc, x, y, buffer, strlen(buffer));
}

void paint_scene(HDC hdc, HWND hwnd, RECT &rect)
{
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 begin(0.f, 0.f, 0.f);

  /* draw scene */
  transform_s transform;
  transform.projection = glm::perspective(45.f, rect.right / (float)rect.bottom, 0.1f, 1000.f);
  transform.model = view;
  draw_viewport viewport(transform, rect);
  viewport.begin_frame(hdc);
  viewport.clear((HBRUSH)(COLOR_WINDOW + 1));
  

  viewport.draw_lines_indexed(verts, indices, sizeof(indices) / sizeof(indices[0]));


	/* Z */
	//glm::vec2 endpos;
	//int vanishing_points = 0;
	//glm::vec2 line1b = glm::vec2(projected_verts[0][0].x, projected_verts[0][0].y);
	//glm::vec2 line1e = glm::vec2(projected_verts[0][1].x, projected_verts[0][1].y);
	//glm::vec2 line2b = glm::vec2(projected_verts[1][0].x, projected_verts[1][0].y);
	//glm::vec2 line2e = glm::vec2(projected_verts[1][1].x, projected_verts[1][1].y);
	//if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
	//	draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
	//	draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
	//	text_print(hdc, endpos.x, endpos.y, "Z (%d %d)", (int)endpos.x, (int)endpos.y);
	//	vanishing_points++;
	//}


	///* Y */
	//line1b = glm::vec2(projected_verts[16][0].x, projected_verts[16][0].y);
	//line1e = glm::vec2(projected_verts[16][1].x, projected_verts[16][1].y);
	//line2b = glm::vec2(projected_verts[17][0].x, projected_verts[17][0].y);
	//line2e = glm::vec2(projected_verts[17][1].x, projected_verts[17][1].y);
	//if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
	//	draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
	//	draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
	//	text_print(hdc, endpos.x, endpos.y, "Y (%d %d)", (int)endpos.x, (int)endpos.y);
	//	vanishing_points++;
	//}

	///* X */
	//line1b = glm::vec2(projected_verts[2][0].x, projected_verts[2][0].y);
	//line1e = glm::vec2(projected_verts[2][1].x, projected_verts[2][1].y);
	//line2b = glm::vec2(projected_verts[3][0].x, projected_verts[3][0].y);
	//line2e = glm::vec2(projected_verts[3][1].x, projected_verts[3][1].y);
	//if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
	//	draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
	//	draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
	//	text_print(hdc, endpos.x, endpos.y, "X (%d %d)", (int)endpos.x, (int)endpos.y);
	//	vanishing_points++;
	//}


	///* crysha left */
	//line1b = glm::vec2(projected_verts[28][0].x, projected_verts[28][0].y);
	//line1e = glm::vec2(projected_verts[28][1].x, projected_verts[28][1].y);
	//line2b = glm::vec2(projected_verts[29][0].x, projected_verts[29][0].y);
	//line2e = glm::vec2(projected_verts[29][1].x, projected_verts[29][1].y);
	//if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
	//	draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
	//	draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
	//	text_print(hdc, endpos.x, endpos.y, "(%d %d)", (int)endpos.x, (int)endpos.y);
	//	vanishing_points++;
	//}

	///* crysha left */
	//line1b = glm::vec2(projected_verts[32][0].x, projected_verts[32][0].y);
	//line1e = glm::vec2(projected_verts[32][1].x, projected_verts[32][1].y);
	//line2b = glm::vec2(projected_verts[33][0].x, projected_verts[33][0].y);
	//line2e = glm::vec2(projected_verts[33][1].x, projected_verts[33][1].y);
	//if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
	//	draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
	//	draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
	//	text_print(hdc, endpos.x, endpos.y, "(%d %d)", (int)endpos.x, (int)endpos.y);
	//	vanishing_points++;
	//}

	///* scene distortion factor */

	//glm::vec3 distortion_factor;
	//distortion_factor.x = glm::length(glm::vec2(projected_verts[2][1].x, projected_verts[2][1].y) - glm::vec2(projected_verts[2][0].x, projected_verts[2][0].y)) / glm::length(glm::vec2(house[2][1].x, house[2][1].y) - glm::vec2(house[2][0].x, house[2][0].y));
	//distortion_factor.y = glm::length(glm::vec2(projected_verts[16][1].x, projected_verts[16][1].y) - glm::vec2(projected_verts[16][0].x, projected_verts[16][0].y)) / glm::length(glm::vec2(house[16][1].x, house[16][1].y) - glm::vec2(house[16][0].x, house[16][0].y));
	//distortion_factor.z = glm::length(glm::vec2(projected_verts[0][1].x, projected_verts[0][1].y) - glm::vec2(projected_verts[0][0].x, projected_verts[0][0].y)) / (glm::length(glm::vec2(house[0][1].x, house[0][1].y) - glm::vec2(house[0][0].x, house[0][0].y)) + 1.f);

	//text_print(hdc, 10, 10, "vanishing points count %d", vanishing_points);
	//text_print(hdc, 10, 30, "distortion  x: %f y: %f z: %f", distortion_factor.x, distortion_factor.y, distortion_factor.z);
	//text_print(hdc, 10, 50, "scene distortion: %f", sqrtf(powf(distortion_factor.x, 2.f) + powf(distortion_factor.y, 2.f) + powf(distortion_factor.z, 2.f)));
	//
	//text_print(hdc, 10, 80, "theta %f", angles.y);
	//text_print(hdc, 10, 100, "phi %f", angles.x);
	//text_print(hdc, 10, 120, "z %f", dist);

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
		//InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_MOUSEWHEEL: {
		SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta < 0)
			dist -= 0.5f;
		else dist += 0.5f;

		//view = glm::translate(mat_identity, glm::vec3(0, 0, dist));
		//view = glm::rotate(view, glm::radians(angles.x), glm::vec3(1, 0, 0));
		//view = glm::rotate(view, glm::radians(angles.y), glm::vec3(0, 1, 0));
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_PAINT: {
    printf("REPAINT\n");
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
			printf("MOUSEMOVE\n");

			curr_cursor.x = LOWORD(lParam);
			curr_cursor.y = HIWORD(lParam);

			POINT diffpt;
			diffpt.y = curr_cursor.x - last_cursor.x;
			diffpt.x = curr_cursor.y - last_cursor.y;
			if (diffpt.x || diffpt.y) {
				angles.x += diffpt.x * -0.1f;
				angles.y += diffpt.y * 0.1f;
				printf("angle %f %f\n", angles.x, angles.y);			
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