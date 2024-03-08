#include <stdio.h>
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

  void to_screen(glm::vec2 &dst_screen_coord, glm::vec3 model_coord) {
    float hsw = viewport_size.cx / 2.f;
    float hsh = viewport_size.cy / 2.f;
    glm::vec4 gcoord = t.projection * t.model * glm::vec4(model_coord, 1.f);
    if (gcoord.w > 0.f) {
      gcoord.x /= gcoord.w;
      gcoord.y /= gcoord.w;
      gcoord.z /= gcoord.w;
    }
    gcoord.x *= hsw;
    gcoord.y *= hsh;
    gcoord.x += hsw;
    gcoord.y += hsh;
    dst_screen_coord.x = gcoord.x;
    dst_screen_coord.y = gcoord.y;
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
    //FillRect(h_memdc, &rect, (HBRUSH)GetStockObject(GRAY_BRUSH));
    printf("l(%d) t(%d) r(%d) b(%d)\n", rect.left, rect.top, rect.right, rect.bottom);
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
    FillRect(h_memdc, &rect, brush);
  }

  void draw_line(const glm::vec2 &pt1, const glm::vec2 &pt2)
  {
    MoveToEx(h_memdc, (int)pt1.x, (int)pt1.y, NULL);
    LineTo(h_memdc, (int)pt2.x, (int)pt2.y);
  }

  void draw_triangles_indexed(const glm::vec3 *p_verts, const unsigned int *p_indices, size_t num_indices)
  {
    glm::vec2 vert[2];
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
    glm::vec2 vert[2];
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
    glm::vec2 p1, p2;
    for (size_t j = 0; j < 3; j++) {
      to_screen(p1, p_verts[j]);
      to_screen(p2, p_verts[(j + 1) % 3]);
      draw_line(p1, p2);
    }
  }

  void draw_lines_indexed(const glm::vec3 *p_verts, const unsigned int *p_indices, size_t num_indices)
  {
    glm::vec2 vert[2];
    size_t num_lines = num_indices / 2;
    for (size_t i = 0; i < num_lines; i++) {
      for (size_t j = 0; j < 2; j++) {
        to_screen(vert[0], p_verts[p_indices[i + j + 0]]);
        to_screen(vert[1], p_verts[p_indices[i + j + 1]]);
        draw_line(vert[0], vert[1]);
      }
    }
  }
};

HWND        h_wnd;
HWND        h_viewport;
transform_s transform;
HBRUSH      h_background;
HBRUSH      h_background2;

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

  h_wnd = CreateWindowExA(0, MAIN_WINDOW_CLASS, "Lab3 - Stereopair controls", WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,
    xpos + viewport_width, ypos, control_width, viewport_height, HWND_DESKTOP, (HMENU)NULL, NULL, NULL);
  if (!h_wnd) {
    printf("failed to create window. LastError=%d\n", GetLastError());
    return 1;
  }

  ShowWindow(h_wnd, SW_SHOW);
  UpdateWindow(h_wnd); 
  ShowWindow(h_viewport, SW_SHOW);
  UpdateWindow(h_viewport);

  h_background = CreateSolidBrush(RGB(255, 0, 0));
  h_background2 = CreateSolidBrush(RGB(0, 0, 255));

  MSG msg;
  while (GetMessageA(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }
  return 0;
}

void draw_scene(HDC hdc, const RECT &rect)
{
  RECT left_eye, right_eye;
  left_eye.left = 0;
  left_eye.top = 0;
  left_eye.right = rect.right >> 1;
  left_eye.bottom = rect.bottom;

  right_eye.left = left_eye.right;
  right_eye.top = 0;
  right_eye.right = rect.right;
  right_eye.bottom = rect.bottom;

  draw_viewport left_eye_drawer(transform, left_eye);
  left_eye_drawer.begin_frame(hdc);
  left_eye_drawer.clear(h_background);

  left_eye_drawer.end_frame(hdc);

  draw_viewport right_eye_drawer(transform, right_eye);
  right_eye_drawer.begin_frame(hdc);
  left_eye_drawer.clear(h_background2);

  right_eye_drawer.end_frame(hdc);
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
