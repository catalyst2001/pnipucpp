#include "gl_viewport.h"

void msgbox_with_lasterror(HWND h_parentwnd, LPCSTR text)
{
	CHAR errbuf[512];
	DWORD last_error = GetLastError();
	sprintf_s(errbuf, sizeof(errbuf), "%s\nError: %d (0x%x)", last_error, last_error);
	MessageBoxA(h_parentwnd, errbuf, "Error!", MB_OK | MB_ICONERROR);
}

VIEWPORTRESULT gl_viewport_init(GLVIEWPORT *p_viewport, DWORD flags, INT x, INT y, INT width, INT height, HWND h_parent, LPCSTR p_classname, INT bpp, INT dbits)
{
	// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-choosepixelformat
	int iPixelFormat;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
		1,                     // version number  
		PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		24,                    // 24-bit color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		0,                     // no alpha buffer  
		0,                     // shift bit ignored  
		0,                     // no accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		32,                    // 32-bit z-buffer      
		0,                     // no stencil buffer  
		0,                     // no auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	};


	p_viewport->h_viewport = CreateWindowExA(WS_EX_CLIENTEDGE, p_classname, "Viewport",
		flags, x, x, width, height, h_parent, (HMENU)0, NULL, NULL);
	if (!p_viewport->h_viewport) {
		gl_viewport_shutdown(p_viewport);
		return VRES_FAILED_TO_CREATE_WINDOW;
	}

	p_viewport->h_device_context = GetDC(p_viewport->h_viewport);
	if (!p_viewport->h_device_context) {
		MessageBoxA(h_parent, "Failed to capture device context!", "Error!", MB_OK | MB_ICONERROR);
		gl_viewport_shutdown(p_viewport);
		return VRES_FAILED_TO_CAPTURE_DEVICE_CONTEXT;
	}

	iPixelFormat = ChoosePixelFormat(p_viewport->h_device_context, &pfd);
	if (!iPixelFormat) {
		msgbox_with_lasterror(h_parent, "Failed to choose pixel format!");
		gl_viewport_shutdown(p_viewport);
		return VRES_FAILED_TO_CHOOSE_PIXELFORMAT;
	}

	if (!SetPixelFormat(p_viewport->h_device_context, iPixelFormat, &pfd)) {
		msgbox_with_lasterror(h_parent, "Failed to set pixel format!");
		gl_viewport_shutdown(p_viewport);
		return VRES_FAILED_TO_SET_PIXELFORMAT;
	}

	if (!(p_viewport->h_gl_context = wglCreateContext(p_viewport->h_device_context))) {
		msgbox_with_lasterror(h_parent, "Failed to create OpenGL context!");
		gl_viewport_shutdown(p_viewport);
		return VRES_FAILED_TO_CREATE_GL_CONTEXT;
	}

	if (!wglMakeCurrent(p_viewport->h_device_context, p_viewport->h_gl_context)) {
		msgbox_with_lasterror(h_parent, "Failed to select OpenGL context");
		gl_viewport_shutdown(p_viewport);
		return VRES_FAILED_TO_SELECT_GL_CONTEXT;
	}
	return VRES_OK;
}

void gl_viewport_shutdown(GLVIEWPORT *p_viewport)
{
	if (p_viewport->h_gl_context) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(p_viewport->h_gl_context);
	}

	if (p_viewport->h_viewport)
		DestroyWindow(p_viewport->h_viewport);

	if (p_viewport->h_viewport && p_viewport->h_device_context)
		ReleaseDC(p_viewport->h_viewport, p_viewport->h_device_context);
}