#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "../cgcommon/gl_viewport.h"
#include "wavefront_obj.h"
#include "dbg.h"

HINSTANCE g_instance;
HWND h_main_window;
HWND h_control_panel;
GLVIEWPORT gl_viewport;

typedef size_t GLsizeiptr;
typedef BOOL (WINAPI *wglSwapIntervalEXTPfn)(int interval);
typedef void (WINAPI *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (WINAPI *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (WINAPI *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void (WINAPI *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);

PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;

void compute_centroid(glm::vec3 &dst_vec, const obj_group &group)
{
	float num_vertices;
	dst_vec = glm::vec3(0.f, 0.f, 0.f);
	for (size_t i = 0; i < group.vertices.size(); i++)
		dst_vec += group.vertices[i];

	num_vertices = (float)group.vertices.size();
	dst_vec /= num_vertices;
}

void obj_group_world_veritces_to_local(obj_group &dst_group, const glm::vec3 begin_of_coords_system)
{
	glm::vec3 centroid;
	compute_centroid(centroid, dst_group);
	dst_group.position = centroid;
	centroid = begin_of_coords_system - centroid;
	for (size_t i = 0; i < dst_group.vertices.size(); i++)
		dst_group.vertices[i] -= centroid;
}

struct vertex {
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 uv;
};

enum BUFFERS {
	VBO = 0,
	IBO,
	NUM_BUFFERS
};

struct mesh {
	union {
		struct {
			GLuint vertex_buffer;
			GLuint index_buffer;
		};

		struct {
			GLuint buffers[2];
		};
	};
	glm::vec3 min;
	glm::vec3 max;
};

struct model {
	glm::vec3 min;
	glm::vec3 max;
	std::vector<mesh> mesh_groups;
};

bool load_gl_extensions()
{
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	return glGenBuffers && glDeleteBuffers && glBindBuffer && glBufferData;
}

void error_msg(const char *p_format, ...)
{
	va_list argptr;
	char buffer[1024];
	va_start(argptr, p_format);
	vsprintf_s(buffer, sizeof(buffer), p_format, argptr);
	va_end(argptr);

	MessageBoxA(HWND_DESKTOP, buffer, "CRITICAL ERROR", MB_OK | MB_ICONERROR);
	exit(1);
}

void center_of_screen(LPRECT p_dst_rect, int width, int height)
{
	p_dst_rect->right = width;
	p_dst_rect->bottom = height;
	p_dst_rect->left = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
	p_dst_rect->top = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);
}

void resize_ui(const LPRECT p_rect)
{
	RECT rect;
	SIZE clientrect_width;
	int controls_panel_width;

	clientrect_width.cx = p_rect->right - (MARGIN_PX * 2);
	clientrect_width.cy = p_rect->bottom - (MARGIN_PX * 2);
	controls_panel_width = CONTROL_PANEL_WIDTH;

	/* RECOMPUTE CONTROL WINDOW SIZE */
	rect.left = clientrect_width.cx - controls_panel_width + MARGIN_PX;
	rect.top = p_rect->top + MARGIN_PX;
	rect.right = CONTROL_PANEL_WIDTH;
	rect.bottom = clientrect_width.cy;
	MoveWindow(h_control_panel, rect.left, rect.top, rect.right, rect.bottom, TRUE);

	/* RECOMPUTE SCENE WINDOW SIZE */
	rect.left = p_rect->left + MARGIN_PX;
	rect.top = p_rect->top + MARGIN_PX;
	rect.right = clientrect_width.cx - controls_panel_width - MARGIN_PX;
	rect.bottom = clientrect_width.cy;
	MoveWindow(gl_viewport.h_viewport, rect.left, rect.top, rect.right, rect.bottom, FALSE);
}

LRESULT CALLBACK wnd_proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK scene_wnd_proc(HWND, UINT, WPARAM, LPARAM);

bool register_classes(HINSTANCE h_instance)
{
	/* REGISTER MAIN WINDOW CLASS */
	WNDCLASSEXA wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW|CS_VREDRAW;
	wcex.hInstance = h_instance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpfnWndProc = wnd_proc;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszClassName = WC_LAB2;
	if (!RegisterClassExA(&wcex))
		return false;

	/* REGISTER VIEWPORT WINDOW CLASS */
	wcex.lpfnWndProc = scene_wnd_proc;
	wcex.style = CS_OWNDC;
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszClassName = WC_LAB2_SCENE;
	if (!RegisterClassExA(&wcex))
		return false;

	return true;
}

model machine;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DBG_INIT();

	g_instance = hInstance;
	if (!register_classes(hInstance)) {
		error_msg("Failed to create window class!");
		return 1;
	}

	/* CREATE MAIN WINDOW */
	RECT rect;
	center_of_screen(&rect, 800, 600);
	h_main_window = CreateWindowExA(0, WC_LAB2, "Lab2", WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right, rect.bottom, NULL, NULL, hInstance, NULL);
	if (!h_main_window) {
		error_msg("Failed to create window");
		return 2;
	}

	if (gl_viewport_init(&gl_viewport, WS_CHILD | WS_VISIBLE, MARGIN_PX, MARGIN_PX, 1, 1, h_main_window, WC_LAB2_SCENE, 24, 32) != VRES_OK) {
		error_msg("Failed to init OpenGL viewport");
		return 3;
	}

	if (!load_gl_extensions()) {
		error_msg("OpenGL version in this machine not support one of extensions");
		return 3;
	}

	wglSwapIntervalEXTPfn wglSwapIntervalEXT = (wglSwapIntervalEXTPfn)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(60);

	h_control_panel = CreateWindowExA(WS_EX_DLGMODALFRAME, "static", "", WS_CHILD | WS_VISIBLE, 1, 1, 1, 1, h_main_window, (HMENU)0, NULL, NULL);

	ShowWindow(h_main_window, SW_SHOW);
	UpdateWindow(h_main_window);

	/* LOAD MODEL */
	std::vector<obj_group *> groups;
	const char *p_path = "Bed_done.obj";
	if (!load_wavefront_obj(groups, p_path)) {
		error_msg("Failed to load model '%s'!", p_path);
		return 1;
	}
	
	DBG("groups count: %zd", groups.size());
	SuspendThread(GetCurrentThread());

	glm::vec3 begin_coords_system = glm::vec3(0.f, 0.f, 0.f);
	for (size_t i = 0; i < groups.size(); i++) {
		
		// transform vertices
		obj_group_world_veritces_to_local(*groups[i], begin_coords_system);

		// create buffers
		mesh mesh_group;
		glGenBuffers(NUM_BUFFERS, mesh_group.buffers);
		glBindBuffer(GL_VERTEX_ARRAY, mesh_group.buffers[VBO]);
		//glBufferData(GL_VERTEX_ARRAY, sizeof(vertex) * groups[i]->vertices.size(), );
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

    MSG msg;
	while (1) {
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		/* DRAW SCENE */
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glTranslatef(0.f, 0.f, -100.f);
		glRotatef(90.f, 1.f, 0.f, 0.f);

		for (size_t i = 0; i < groups.size(); i++) {
			glVertexPointer(3, GL_FLOAT, 0, groups[i]->vertices.data());
			glNormalPointer(GL_FLOAT, 0, groups[i]->normals.data());
			//glDrawElements(GL_TRIANGLES, )
		}
		

		SwapBuffers(gl_viewport.h_device_context);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

    return 0;
}

LRESULT CALLBACK scene_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SIZE size;
	switch (message) {
	case WM_CREATE: {
		glEnable(GL_DEPTH_TEST);
		return 0;
	}

	case WM_SIZE: {
		size.cx = LOWORD(lParam);
		size.cy = HIWORD(lParam);
		if (!size.cy)
			size.cy = 1;

		glViewport(0, 0, size.cx, size.cy);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.f, size.cx / (double)size.cy, 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;
	}

	case WM_ERASEBKGND:
		return 1;

	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
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

	case WM_SIZE: {
		GetClientRect(hWnd, &rect);
		resize_ui(&rect);
		break;
	}

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	case WM_LBUTTONDOWN:
		break;

	case WM_MOUSEMOVE: {
	}

	case WM_LBUTTONUP:
		break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}