#include <stdio.h>
#include <string.h>
#include <windowsx.h>
#include "gl_exts.h"
#include "../cgcommon/gl_viewport.h"
#include "main.h"
#include "dbg.h"
#include "model.h"
#include "trackball.h"
#include "win32_controls.h"

HINSTANCE g_instance;
HWND h_main_window;
HWND h_control_panel;
GLVIEWPORT gl_viewport;

glm::mat4x4 rotation_matrix;
glm::quat curr_rot_quat, last_rot_quat;

typedef BOOL(WINAPI *wglSwapIntervalEXTPfn)(int interval);

ctls::checkbox wireframe_check;

union shade_u {
	shade_u() {}
	~shade_u() {}

	struct {
		ctls::toggle_button flat_shaded;
		ctls::toggle_button smooth_shaded;
	};
	ctls::toggle_button arr[2];
};

shade_u shading;

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
	SetFocus(gl_viewport.h_viewport);
}

LRESULT CALLBACK wnd_proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK scene_wnd_proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK controlpanel_wnd_proc(HWND, UINT, WPARAM, LPARAM);

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

	/* CONTROL PANEL */
	wcex.lpfnWndProc = controlpanel_wnd_proc;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszClassName = WC_LAB2_CONTROLPANEL;
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

model scene_model;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DBG_INIT();

	INITCOMMONCONTROLSEX iccex;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC = ICC_WIN95_CLASSES;
	if (!InitCommonControlsEx(&iccex)) {
		DWORD error = GetLastError();
		error_msg("Failed to initialize new version on windows common controls!\r\nError: %d (0x%x)", error, error);
		return 1;
	}

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
	gl_load_extensions();

	wglSwapIntervalEXTPfn wglSwapIntervalEXT = (wglSwapIntervalEXTPfn)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(60);

	h_control_panel = CreateWindowExA(WS_EX_DLGMODALFRAME, WC_LAB2_CONTROLPANEL, "", WS_CHILD | WS_VISIBLE, 1, 1, 1, 1, h_main_window, (HMENU)0, NULL, NULL);

	//CreateWindowExA(0, WC_BUTTONA, "Button test", WS_VISIBLE | WS_CHILD, 0, 0, 200, 100, h_control_panel, (HMENU)0, 0, 0);

	ctls::set_default_font();

	wireframe_check = ctls::checkbox(h_control_panel, IDC_WIREFRAME, "Wireframe", 0, 0, 400, 30);

	glShadeModel(GL_FLAT);
	shading.flat_shaded = ctls::toggle_button(h_control_panel, IDC_SHADE_MODEL_FLAT, "Flat", 0, 30 + 2, 100, 20, 0, true);
	shading.smooth_shaded = ctls::toggle_button(h_control_panel, IDC_SHADE_MODEL_SMOOTH, "Smooth", 100+2, 30 + 2, 100, 20);

	ShowWindow(h_main_window, SW_SHOW);
	UpdateWindow(h_main_window);

	if (!scene_model.load_model("models/Bed_done.obj")) {
		printf("Failed to load model!\n");
		return 1;
	}

	//SuspendThread(GetCurrentThread());

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);

	GetClientRect(gl_viewport.h_viewport, &rect);
	glViewport(0, 0, rect.right, rect.bottom);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, rect.right / (double)rect.bottom, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.5f, 0.5f, 0.5f, 1.5f);
	glClearDepth(1.0f);

	GLfloat g_LighPos[] = { 10.0f, 100.0f, 10.0f, 1.0f };
	GLfloat g_LightAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat g_LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat g_LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat g_LighAttenuation0 = 1.0f;
	GLfloat g_LighAttenuation1 = 0.0f;
	GLfloat g_LighAttenuation2 = 0.0f;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, g_LighPos);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_LightSpecular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, g_LighAttenuation0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, g_LighAttenuation1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, g_LighAttenuation2);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
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


		//GetClientRect(gl_viewport.h_viewport, &rect);
		//glLoadIdentity();
		//gluPerspective(45.0f, rect.right / (float)rect.bottom, 0.1f, 2000.f);

		glTranslatef(0.0f, -20.0f, -320.0f);
		glRotatef(45.f, 1.f, 1.f, 0.f);

		scene_model.draw_model();

		SwapBuffers(gl_viewport.h_device_context);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);

    return 0;
}

#define MBUTTON_LEFT (1 << 0)
#define MBUTTON_RIGHT (1 << 1)
#define MBUTTON_CENTER (1 << 2)

void build_rotmatrix(glm::mat4 &dst_mat, const glm::quat &rot_quat)
{
	float rot_mat[4][4];
	build_rotmatrix(rot_mat, glm::value_ptr(rot_quat));
	dst_mat = glm::mat4(rot_mat[0][0], rot_mat[0][1], rot_mat[0][2], rot_mat[0][3],
		rot_mat[1][0], rot_mat[1][1], rot_mat[1][2], rot_mat[1][3],
		rot_mat[2][0], rot_mat[2][1], rot_mat[2][2], rot_mat[2][3],
		rot_mat[3][0], rot_mat[3][1], rot_mat[3][2], rot_mat[3][3]);
}

void norm_mouse_position(glm::vec2 &dst_mouse_pt, HWND h_wnd, int x, int y)
{
	RECT rect;
	GetClientRect(h_wnd, &rect);
	dst_mouse_pt.x = x / (float)rect.right;
	dst_mouse_pt.y = y / (float)rect.bottom;
}

#define ROTATION_SENSITIVITY (-0.01f)

LRESULT CALLBACK scene_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	glm::vec2 curr_pt(0.f, 0.f);
	static int mbflags = 0;
	static glm::vec2 last_pt;

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

	case WM_KEYDOWN: {
		if (wParam == VK_F1) {
			static bool has_wireframe = true;
			glPolygonMode(GL_FRONT_AND_BACK, (has_wireframe) ? GL_LINE : GL_FILL);
			has_wireframe = !has_wireframe;
		}
		
		break;
	}

	case WM_LBUTTONDOWN:
		mbflags |= MBUTTON_LEFT;
		norm_mouse_position(last_pt, hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_LBUTTONUP:
		mbflags &= ~MBUTTON_LEFT;
		norm_mouse_position(last_pt, hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_RBUTTONDOWN:
		mbflags |= MBUTTON_RIGHT;
		break;

	case WM_RBUTTONUP:
		mbflags &= ~MBUTTON_RIGHT;
		break;

	case WM_MBUTTONDOWN:
		mbflags |= MBUTTON_CENTER;
		break;

	case WM_MBUTTONUP:
		mbflags &= ~MBUTTON_CENTER;
		break;

	case WM_MOUSEMOVE: {
		norm_mouse_position(curr_pt, hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (mbflags & MBUTTON_LEFT) {


		}
		last_pt = curr_pt;
		break;
	}

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

	case WM_ACTIVATE:
		SetFocus(gl_viewport.h_viewport);
		break;

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
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK controlpanel_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		SetFocus(gl_viewport.h_viewport);
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDC_WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, (wireframe_check.is_checked()) ? GL_LINE : GL_FILL);
			break;

		case IDC_SHADE_MODEL_FLAT:
			glShadeModel(GL_FLAT);
			shading.smooth_shaded.set_check(false);
			break;

		case IDC_SHADE_MODEL_SMOOTH:
			glShadeModel(GL_SMOOTH);
			shading.flat_shaded.set_check(false);
			break;

		default:
			return DefWindowProcA(hWnd, message, wParam, lParam);
		}
	}
	break;

	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return 0;
}