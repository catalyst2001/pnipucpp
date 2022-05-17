#include "gldl.h"

struct {
	HWND h_window;
	HDC device_ctx;
	HGLRC gl_ctx;
	gldl_codes_t last_error;
	int wx, wy;
	int wwidth;
	int wheight;
	int colorbits;
	int depthbits;
	struct GLUquadric *p_quadric;
	unsigned char buttons[1024][2];

	mouse_point_t mouse;
	int mouse_wheel_delta;

	gldl_events_dt_t *p_events_callbacks;
} wd;


#define STR(x) #x
#define GLDL_CLASS "gldl"STR(__TIMESTAMP__)
#define KEY_REPEAT (1 << 30)

void begin2d()
{
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, wd.wwidth, wd.wheight);

#ifndef FLIP
	glOrtho(0.0, (double)wd.wwidth, 0.0, (double)wd.wheight, -1.0, 1.0);
#else
	glOrtho(0.0, (double)wd.wwidth, (double)wd.wheight, 0.0, -1.0, 1.0);
#endif
}

void end2d()
{
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void draw_sphere(float x, float y, float z, float radius)
{
	glTranslatef(x, y, z);
	gluSphere(wd.p_quadric, radius, 20, 20);
	glTranslatef(-x, -y, -z);
}

void draw_cylinder(float x, float y, float z, float width, float height)
{
	glTranslatef(x, y, z);
	gluCylinder(wd.p_quadric, width, width, height, 20, 20);
	glTranslatef(-x, -y, -z);
}

void draw_3d_gird(float x, float y, float z, float width, float step)
{
	float i;
	glBegin(GL_LINES);
	for (i = -width; i <= width; i += step) {
		glVertex3f(-width, 0.f, i);
		glVertex3f(width, 0.f, i);
		glVertex3f(i, 0.f, -width);
		glVertex3f(i, 0.f, width);
	}
	glEnd();
}

void draw_axises_imp(float line_width, float axis_length)
{
	glPushAttrib(GL_CURRENT_BIT);
	glLineWidth(line_width);
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(axis_length, 0, 0);

	glColor3ub(0, 255, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, axis_length, 0);

	glColor3ub(0, 0, 255);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, axis_length);
	glEnd();
	glLineWidth(1.0f);
	glPopAttrib();
}

void draw_bbox(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
{
	//nothing...
}

int get_colorbits() { return wd.colorbits; }
int get_depthbits() { return wd.depthbits; }
void get_mousepos(mouse_point_t *p_dest) { *p_dest = wd.mouse; }
int get_mouse_wheel_delta() { return wd.mouse_wheel_delta; }
int get_mousex() { return wd.mouse.x; }
int get_mousey() { return wd.mouse.y; }
int get_windowposx() { return wd.wx; }
int get_windowposy() { return wd.wy; }
int get_windowwidth() { return wd.wwidth; }
int get_windowheight() { return wd.wheight; }
bool is_window_opened_imp() {
	MSG msg = { 0 };
	//if (PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE) && msg.message != WM_QUIT) {
	if (GetMessageA(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
		SwapBuffers(wd.device_ctx);
		return true;
	}
	printf("msg: %d (0x%x)\n", msg.message, msg.message);
	return false;
}

bool is_button_pressed_imp(int button)
{
	return wd.buttons[button][0];
}

bool is_button_pressed_one(int button)
{
	return wd.buttons[button][0] && !wd.buttons[button][1];
}

void print_text_imp(int x, int y, const char *p_text, ...)
{
	begin2d();
	// not implemented
	end2d();
}

void draw_2d_circle(int x, int y, int mode, int nsegs, int radius)
{
	begin2d();
	glBegin(!mode ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i <= nsegs; i++) {
		float angle = 2 * 3.1415926535897932384 * i / (float)nsegs;
		float xofs = cos(angle) * (double)radius;
		float yofs = sin(angle) * (double)radius;
		glVertex2i(x + (int)xofs, y + (int)yofs);
	}
	glEnd();
	end2d();
}

gldl_dt_t *init_dispatch_table()
{
	static gldl_dt_t dispatch_table;
	dispatch_table.Begin2D = begin2d;
	dispatch_table.End2D = end2d;
	dispatch_table.DrawSphere = draw_sphere;
	dispatch_table.DrawCylinder = draw_cylinder;
	dispatch_table.Draw3DGird = draw_3d_gird;
	dispatch_table.Draw3DAxises = draw_axises_imp;
	dispatch_table.DrawBBox = draw_bbox;
	dispatch_table.GetColorBits = get_colorbits;
	dispatch_table.GetDepthBits = get_depthbits;
	dispatch_table.GetMousePosition = get_mousepos;
	dispatch_table.GetMouseWheelDelta = get_mouse_wheel_delta;
	dispatch_table.GetMouseX = get_mousex;
	dispatch_table.GetMouseY = get_mousey;
	dispatch_table.GetWindowPosX = get_windowposx;
	dispatch_table.GetWindowPosY = get_windowposy;
	dispatch_table.GetWindowWidth = get_windowwidth;
	dispatch_table.GetWindowHeight = get_windowheight;
	dispatch_table.IsWindowOpened = is_window_opened_imp;
	dispatch_table.IsButtonPressed = is_button_pressed_imp;
	dispatch_table.IsButtonPressedOne = is_button_pressed_one; //TODO: not implemented
	dispatch_table.PrintText = print_text_imp;
	dispatch_table.Draw2DCircle = draw_2d_circle;
	return &dispatch_table;
}

LRESULT CALLBACK window_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT res = 0;
	switch (message)
	{
	case WM_SIZE:
	{
		wd.wwidth = LOWORD(lParam);
		wd.wheight = HIWORD(lParam);
		if (!wd.wheight)
			wd.wheight = 1;

		glViewport(0, 0, (GLsizei)wd.wwidth, (GLsizei)wd.wheight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, wd.wwidth / (double)wd.wheight, 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (wd.p_events_callbacks && wd.p_events_callbacks->p_window_resize_event)
			wd.p_events_callbacks->p_window_resize_event(wd.wwidth, wd.wheight);

		break;
	}

	case WM_KEYDOWN: {
		wd.buttons[wParam][0] = true;
		wd.buttons[wParam][1] = ((lParam & KEY_REPEAT) == KEY_REPEAT);
		//printf("key: %d  state: %d  repeat: %d\n", wParam, wd.buttons[wParam][0], wd.buttons[wParam][1]);
		if (wd.p_events_callbacks && wd.p_events_callbacks->p_keydown_input_event)
			wd.p_events_callbacks->p_keydown_input_event(wParam, STATE_PRESSED);

		break;
	}

	case WM_KEYUP: {
		wd.buttons[wParam][0] = false;
		//printf("key: %d  state: %d  repeat: %d\n", wParam, wd.buttons[wParam][0], wd.buttons[wParam][1]);
		if (wd.p_events_callbacks && wd.p_events_callbacks->p_keydown_input_event)
			wd.p_events_callbacks->p_keydown_input_event(wParam, STATE_RELEASED);

		break;
	}

	case WM_MOUSEMOVE:
		if (wd.p_events_callbacks && wd.p_events_callbacks->p_mouse_move_event)
			wd.p_events_callbacks->p_mouse_move_event(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_LBUTTONDOWN:
		if (wd.p_events_callbacks && wd.p_events_callbacks->p_mouse_click_event)
			wd.p_events_callbacks->p_mouse_click_event(LOWORD(lParam), HIWORD(lParam), LBUTTON, STATE_PRESSED);
		break;

	case WM_LBUTTONUP:
		if (wd.p_events_callbacks && wd.p_events_callbacks->p_mouse_click_event)
			wd.p_events_callbacks->p_mouse_click_event(LOWORD(lParam), HIWORD(lParam), LBUTTON, STATE_RELEASED);
		break;

	case WM_RBUTTONDOWN:
		if (wd.p_events_callbacks && wd.p_events_callbacks->p_mouse_click_event)
			wd.p_events_callbacks->p_mouse_click_event(LOWORD(lParam), HIWORD(lParam), RBUTTON, STATE_PRESSED);
		break;

	case WM_RBUTTONUP:
		if (wd.p_events_callbacks && wd.p_events_callbacks->p_mouse_click_event)
			wd.p_events_callbacks->p_mouse_click_event(LOWORD(lParam), HIWORD(lParam), RBUTTON, STATE_RELEASED);
		break;

	case WM_CHAR:
		if (wd.p_events_callbacks && wd.p_events_callbacks->p_char_input_event)
			wd.p_events_callbacks->p_char_input_event((char)wParam);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_ERASEBKGND:
		return 1;

	default:
		res = DefWindowProc(hWnd, message, wParam, lParam);
	}
	return res;
}

gldl_dt_t *gldl_initialize(int x, int y, int width, int height, int color_bits, int depth_bits, const char *p_title, gldl_events_dt_t *p_events_dispatch_table)
{
	gldl_dt_t *p_dt = NULL;

	WNDCLASSEXA wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.hCursor = LoadCursorA(NULL, IDC_ARROW);
	wcex.hInstance = GetModuleHandleA(0);
	wcex.lpszClassName = GLDL_CLASS;
	wcex.lpfnWndProc = window_proc;
	if (!RegisterClassExA(&wcex)) {
		wd.last_error = GLDL_ERROR_CREATING_WCLASS;
		goto __end;
	}

	if (!p_title)
		p_title = "Demo OpenGL Application";

	wd.wwidth = width;
	wd.wheight = height;
	if (x == -1 || y == -1) {
		wd.wx = (GetSystemMetrics(SM_CXSCREEN) / 2) - (wd.wwidth / 2);
		wd.wy = (GetSystemMetrics(SM_CYSCREEN) / 2) - (wd.wheight / 2);
	}
	else {
		wd.wx = x;
		wd.wy = y;
	}

	wd.h_window = CreateWindowExA(0, wcex.lpszClassName, p_title, WS_OVERLAPPEDWINDOW, wd.wx, wd.wy, width, height, NULL, (HMENU)NULL, wcex.hInstance, NULL);
	if(!wd.h_window) {
		wd.last_error = GLDL_ERROR_CREATING_WINDOW;
		goto __end;
	}
	ShowWindow(wd.h_window, SW_SHOW);
	UpdateWindow(wd.h_window);

	wd.device_ctx = GetDC(wd.h_window);
	if(!wd.device_ctx) {
		wd.last_error = GLDL_ERROR_GET_DEVICE_CONTEXT;
		goto __end;
	}

	//Структура формата пикселей
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
		1,                     // version number  
		PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		color_bits,             // 24-bit color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		0,                     // no alpha buffer  
		0,                     // shift bit ignored  
		0,                     // no accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		depth_bits,             // 32-bit z-buffer      
		0,                     // no stencil buffer  
		0,                     // no auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	};

	int iPixelFormat = ChoosePixelFormat(wd.device_ctx, &pfd); //Узнаем формат пикселей совместимый с контекстом устройства
	if (!iPixelFormat) {
		wd.last_error = GLDL_ERROR_CHOOSE_PIXELFORMAT;
		goto __end;
	}
	
	if (SetPixelFormat(wd.device_ctx, iPixelFormat, &pfd) == FALSE) { //Устанавливаем совместимый с контекстом устройства формат пикселей
		wd.last_error = GLDL_ERROR_SET_PIXEL_FORMAT;
		goto __end;
	}

	wd.gl_ctx = wglCreateContext(wd.device_ctx); //Создаем контекст визуализации OpenGL
	if (!wd.gl_ctx) {
		wd.last_error = GLDL_ERROR_CREATING_GL_CONTEXT;
		goto __end;
	}

	if (!wglMakeCurrent(wd.device_ctx, wd.gl_ctx)) {
		wd.last_error = GLDL_ERROR_MAKE_CURRENT_CONTEXT;
		goto __end;
	}

	memset(wd.buttons, 0, sizeof(wd.buttons));
	wd.p_quadric = gluNewQuadric();
	wd.p_events_callbacks = p_events_dispatch_table;
	p_dt = init_dispatch_table();

	RECT rct;
	GetClientRect(wd.h_window, &rct);
	glViewport(0, 0, (GLsizei)rct.right, (GLsizei)rct.bottom);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, rct.right / (double)rct.bottom, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

__end:
	return p_dt;
}

gldl_codes_t gldl_last_error() { return wd.last_error; }

const char *gldl_last_error_to_string(gldl_codes_t code)
{
#define CASE_STR(exp, str) case exp: return str;
	switch (code) {
		CASE_STR(GLDL_OK, "No errors")
		CASE_STR(GLDL_ERROR_CREATING_WCLASS,		"Failed to create window class")
		CASE_STR(GLDL_ERROR_CREATING_WINDOW,		"Failed to create window")
		CASE_STR(GLDL_ERROR_GET_DEVICE_CONTEXT,		"Failed to get window device context")
		CASE_STR(GLDL_ERROR_CHOOSE_PIXELFORMAT,		"Failed choose pixel format")
		CASE_STR(GLDL_ERROR_SET_PIXEL_FORMAT,		"Failed set pixel format")
		CASE_STR(GLDL_ERROR_CREATING_GL_CONTEXT,	"Failed to create OpenGL context")
		CASE_STR(GLDL_ERROR_MAKE_CURRENT_CONTEXT,	"Failed select GL context")
	}
#undef CASE_STR
	return "Unknown error";
}

const char *gldl_last_error_string()
{
	return gldl_last_error_to_string(gldl_last_error());
}

gldl_codes_t gldl_shutdown(gldl_dt_t *p_gldl_window)
{
	wglMakeCurrent(NULL, NULL);
	if (wglDeleteContext(wd.gl_ctx) && ReleaseDC(wd.h_window, wd.device_ctx) && DestroyWindow(wd.h_window) && UnregisterClassA(GLDL_CLASS, GetModuleHandleA(0)))
		return GLDL_OK;

	return GLDL_ERROR_DEINIT;
}
