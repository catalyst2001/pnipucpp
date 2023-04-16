#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <string.h>
#include <stdio.h>

enum VIEWPORTRESULT
{
	VRES_OK = 0, //нет ошибок
	VRES_FAILED_TO_CREATE_WINDOW, //не удалось создать окно
	VRES_FAILED_TO_CAPTURE_DEVICE_CONTEXT, //не удалось выполнить захват контекста устройства gdi
	VRES_FAILED_TO_CHOOSE_PIXELFORMAT, //не удалось получить формат пикселей
	VRES_FAILED_TO_SET_PIXELFORMAT, //не удалось установить формат пикселей
	VRES_FAILED_TO_CREATE_GL_CONTEXT,
	VRES_FAILED_TO_SELECT_GL_CONTEXT
};

struct GLVIEWPORT
{
	HWND h_viewport;
	HDC h_device_context;
	HGLRC h_gl_context;
};

// 
// init_viewport
// 
// Создает окно сцены OpenGL
// 
VIEWPORTRESULT gl_viewport_init(GLVIEWPORT *p_viewport, DWORD flags, INT x, INT y, INT width, INT height, HWND h_parent, LPCSTR p_classname, INT bpp, INT dbits);
void		   gl_viewport_shutdown(GLVIEWPORT *p_viewport);


void msgbox_with_lasterror(HWND h_parentwnd, LPCSTR text);