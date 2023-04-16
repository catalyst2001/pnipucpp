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
	VRES_OK = 0, //��� ������
	VRES_FAILED_TO_CREATE_WINDOW, //�� ������� ������� ����
	VRES_FAILED_TO_CAPTURE_DEVICE_CONTEXT, //�� ������� ��������� ������ ��������� ���������� gdi
	VRES_FAILED_TO_CHOOSE_PIXELFORMAT, //�� ������� �������� ������ ��������
	VRES_FAILED_TO_SET_PIXELFORMAT, //�� ������� ���������� ������ ��������
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
// ������� ���� ����� OpenGL
// 
VIEWPORTRESULT gl_viewport_init(GLVIEWPORT *p_viewport, DWORD flags, INT x, INT y, INT width, INT height, HWND h_parent, LPCSTR p_classname, INT bpp, INT dbits);
void		   gl_viewport_shutdown(GLVIEWPORT *p_viewport);


void msgbox_with_lasterror(HWND h_parentwnd, LPCSTR text);