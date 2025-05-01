#include <stdio.h>
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "objload.h"

HWND h_wnd;
bool b_alive = true;
HDC  h_dc;
HDC  h_memdc;

glm::mat4x4 projection_matrix(1.f);
glm::mat4x4 view_matrix(1.f);
glm::mat4x4 model_matrix(1.f);

LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_KEYDOWN: {
		switch (wparam) {
		case VK_ESCAPE:
			b_alive = false;
			break;
		case 'W':
			printf("move foward\n");
			view_matrix = glm::translate(view_matrix, glm::vec3(0.f, 0.f, -1.f));
			break;
		case 'S':
			printf("move backward\n");
			view_matrix = glm::translate(view_matrix, glm::vec3(0.f, 0.f, 1.f));
			break;
		case 'A':
			printf("move left\n");
			view_matrix = glm::translate(view_matrix, glm::vec3(-1.f, 0.f, 0.f));
			break;
		case 'D':
			printf("move right\n");
			view_matrix = glm::translate(view_matrix, glm::vec3(1.f, 0.f, 0.f));
			break;
		}
		break;
	}

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcA(hwnd, msg, wparam, lparam);
}

double get_time()
{
	LARGE_INTEGER li, freq;
	QueryPerformanceCounter(&li);
	QueryPerformanceFrequency(&freq);
	return (double)li.QuadPart / (double)freq.QuadPart;
}

double current_time = get_time();
double last_time=0.0;

const glm::vec3 cube_vertices[] = {
	glm::vec3(-1.f, -1.f, -1.f),
	glm::vec3(1.f, -1.f, -1.f),
	glm::vec3(1.f,  1.f, -1.f),
	glm::vec3(-1.f,  1.f, -1.f),
	glm::vec3(-1.f, -1.f,  1.f),
	glm::vec3(1.f, -1.f,  1.f),
	glm::vec3(1.f,  1.f,  1.f),
	glm::vec3(-1.f,  1.f,  1.f)
};

const int cube_indices[] = {
	0, 1, 2, 3, // back face
	4, 5, 6, 7, // front face
	0, 1, 5, 4, // bottom face
	2, 3, 7, 6, // top face
	0, 3, 7, 4, // left face
	1, 2, 6, 5  // right face
};

void draw_line_gdi(float x0, float y0, float x1, float y1, COLORREF color)
{
	HPEN h_pen = CreatePen(PS_SOLID, 1, color);
	HGDIOBJ oldpen = SelectObject(h_memdc, h_pen);
	MoveToEx(h_memdc, (int)x0, (int)y0, nullptr);
	LineTo(h_memdc, (int)x1, (int)y1);
	SelectObject(h_memdc, oldpen);
	DeleteObject(h_pen);
}

glm::vec3 transform_coord(const RECT &window_rect, const glm::vec3 &src)
{
	// M V P
	// P V M
	glm::mat4x4 mat_complex = projection_matrix * view_matrix * model_matrix;
	glm::vec4   gcoord(src, 1.f);
	glm::vec4   clip_coord = mat_complex * gcoord;
	if (clip_coord.w > FLT_EPSILON) {
		clip_coord.x /= clip_coord.w;
		clip_coord.y /= clip_coord.w;
		clip_coord.z /= clip_coord.w;
	}

	float hw = window_rect.right / 2.f;
	float hh = window_rect.bottom / 2.f;
	clip_coord.x *= hw;
	clip_coord.x += hw;
	clip_coord.y *= -hh;
	clip_coord.y += hh;
	return glm::vec3(clip_coord.x, clip_coord.y, clip_coord.z);
}

void draw_line_3d(const RECT &rect, glm::vec3 begin, glm::vec3 end, COLORREF color)
{
	begin = transform_coord(rect, begin);
	end = transform_coord(rect, end);
	draw_line_gdi(begin.x, begin.y, end.x, end.y, color);
}

void draw_lines_indexed(const RECT &rect, const glm::vec3 *pverts, const int *pindices, int count, COLORREF color)
{
	for (int i = 0; i < count; i += 2) {
		const glm::vec3& begin = pverts[pindices[i + 0]];
		const glm::vec3& end =   pverts[pindices[i + 1]];
		draw_line_3d(rect, begin, end, color);
	}
}

void draw_lines(const RECT& rect, const glm::vec3* pverts, int num_verts, COLORREF color)
{
	int vert = 0;
	for (int i = 0; i < num_verts; i++) {
		const glm::vec3& begin = pverts[vert];
		const glm::vec3& end = pverts[(vert + 1) % num_verts];
		draw_line_3d(rect, begin, end, color);
		vert++;
	}
}

int main()
{
	WNDCLASSA wc{};
	wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
	wc.lpszClassName = "WindowClass";
	wc.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
	wc.lpfnWndProc = wndproc;
	if (!RegisterClassA(&wc)) {
		MessageBoxA(nullptr, "Failed to register window class", "Error", MB_OK);
		return 1;
	}

	const int width = 800;
	const int height = 600;
	int mwidth = GetSystemMetrics(SM_CXSCREEN);
	int mheight = GetSystemMetrics(SM_CYSCREEN);
	int x = (mwidth - width) / 2;
	int y = (mheight - height) / 2;
	h_wnd = CreateWindowExA(0, "WindowClass", "Software Renderer", 
		WS_OVERLAPPEDWINDOW, x, y, width, height, HWND_DESKTOP, (HMENU)0, nullptr, nullptr);
	if (!h_wnd) {
		MessageBoxA(nullptr, "Failed to create window", "Error", MB_OK);
		return 1;
	}

	h_dc = GetDC(h_wnd);
	ShowWindow(h_wnd, SW_SHOW);

	/**
	* load model
	*/
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	if (!obj_load("boblamp.obj", vertices, uvs, normals)) {
		MessageBoxA(nullptr, "Failed to load model", "Error", MB_OK);
		return 1;
	}

	MSG msg;
	RECT rect;
	while (b_alive) {
		while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
			//printf("message: %d\n", msg.message);
			if (msg.message == WM_QUIT) {
				b_alive = false;
				break;
			}
			DispatchMessageA(&msg);
		}

		double dt = current_time - last_time;
		current_time = get_time();
		last_time = current_time;

		GetClientRect(h_wnd, &rect);
		if (!rect.bottom)
			rect.bottom = 1;

		HBITMAP h_bitmap = CreateCompatibleBitmap(h_dc, rect.right, rect.bottom);
		h_memdc = CreateCompatibleDC(h_dc);
		SelectObject(h_memdc, h_bitmap);

		projection_matrix = glm::perspective(glm::radians(45.f), (float)rect.right / (float)rect.bottom, 0.1f, 1000.f);

		float zx_scl = glm::sin(current_time);
		model_matrix = glm::scale(glm::mat4x4(1.f), glm::vec3(1.f, zx_scl, 1.f));
		model_matrix = glm::rotate(model_matrix, (float)current_time, glm::vec3(0.f, 1.f, 0.f));

		view_matrix = glm::translate(glm::mat4x4(1.f), glm::vec3(0.f, -20.f, -200.f));

		draw_lines(rect, vertices.data(), vertices.size(), RGB(255, 255, 255));

		BitBlt(h_dc, 0, 0, rect.right, rect.bottom, h_memdc, 0, 0, SRCCOPY);
		DeleteObject(h_bitmap);
		DeleteDC(h_memdc);
	}
	ReleaseDC(h_wnd, h_dc);
  return 0;
}