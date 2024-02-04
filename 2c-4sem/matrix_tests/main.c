#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "cmathlib.h"
#include <stdio.h>
#include "renderer.h"
#include <CommCtrl.h>

HWND h_dialog;
HWND h_p, h_q, h_r;

void mat_print(const float *p_mat_data, int xdim, int ydim)
{
	putc('\n', stdout);
	for (int i = 0; i < xdim; i++) {
		putc('|', stdout);
		for (int j = 0; j < ydim; j++) {
			printf("%f ", p_mat_data[i * ydim + j]);
		}
		puts("|\n");
	}
	putc('\n', stdout);
}

renderer_t render;
renderer_t *p_render = &render;

//win32 specific
float sensitivity = 0.1f;
POINT last_pt;
POINT curr_pt;
bool mouse_dragged = false;
vec3_t rotation;
int sel = 0;
float pqr[3] = { 0, 0, 10 };

vec3_t cube_verts[] = {
	{ -1, -1, -1},
	{ -1, -1, 1},
	{ 1, -1, 1},
	{ 1, -1, -1},

	{ -1, 1, -1},
	{ -1, 1, 1},
	{ 1, 1, 1},
	{ 1, 1, -1},
};

uint32_t cube_indices[] = {
	0, 1,
	1, 2,
	2, 3,
	3, 0,

	0,4,
	1,5,
	2,6,
	3,7,

	4,5,
	5,6,
	6,7,
	7,4
};

void draw_scene(HDC dst_dc)
{
	vec3_t rot_radians;
	rot_radians.x = rotation.x * DEG2RAD;
	rot_radians.y = rotation.y * DEG2RAD;
	rot_radians.z = 0.f;

	r_enable(p_render, R_GEN_PROJECT_BUFFER);
	r_begin_frame(dst_dc, p_render);
	r_clear(p_render, R_CLEAR_COLOR);

	mat44_ident(&p_render->model);
	mat44_rotate(&p_render->model, &rot_radians);
	mat44_translate(&p_render->model, pqr);

	const projbuf_t *p_projbuf = r_query_proj_buf(p_render);

	mat44_ident(&p_render->projection);

	r_draw_lines_indexed(p_render, cube_verts, cube_indices, sizeof(cube_indices) / sizeof(cube_indices[0]));


	r_present(dst_dc, p_render);
	r_end_frame(p_render);
}

LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_SIZE:
		GetClientRect(hwnd, &render.fb_rect);
		break;

	case WM_KEYDOWN:
		switch (wparam)
		{
		case '1':
			sel = 0;
			break;

		case '2':
			sel = 1;
			break;

		case '3':
			sel = 2;
			break;
		}
		printf("sel %d\n", sel);
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_MOUSEWHEEL: {
		SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
		printf("%d\n", zDelta);
		pqr[sel] += (float)zDelta / 120.f;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	}

	case WM_KEYUP:
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_LBUTTONDOWN:
		mouse_dragged = true;
		curr_pt.x = GET_X_LPARAM(lparam);
		curr_pt.y = GET_X_LPARAM(lparam);
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		mouse_dragged = false;
		last_pt.x = curr_pt.x = GET_X_LPARAM(lparam);
		last_pt.x = curr_pt.y = GET_X_LPARAM(lparam);
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_MOUSEMOVE: {
		curr_pt.x = GET_X_LPARAM(lparam);
		curr_pt.y = GET_X_LPARAM(lparam);
		if (mouse_dragged) {
			if (last_pt.x != curr_pt.x && last_pt.y != curr_pt.y) {
				rotation.y += (curr_pt.x - last_pt.x) * sensitivity;
				rotation.x += (curr_pt.y - last_pt.y) * sensitivity;
				InvalidateRect(hwnd, NULL, FALSE);
			}
		}
		last_pt = curr_pt;
		break;
	}

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		draw_scene(hdc);
		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_HSCROLL: {
		int ip = (int)SendMessageA(h_p, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		int iq = (int)SendMessageA(h_q, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		int ir = (int)SendMessageA(h_r, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		pqr[0] = ip * 0.1f;
		pqr[1] = iq * 0.1f;
		pqr[2] = ir * 0.1f;
		printf("p: %f  q: %f  r: %f\n", pqr[0], pqr[1], pqr[2]);
		InvalidateRect(p_render->h_wnd, NULL, TRUE);
	}
	break;
	
	}
	return DefWindowProcA(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK dialogproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_HSCROLL: {
		int ip = (int)SendMessageA(h_p, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		int iq = (int)SendMessageA(h_q, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		int ir = (int)SendMessageA(h_r, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		pqr[0] = ip * 0.1f;
		pqr[1] = iq * 0.1f;
		pqr[2] = ir * 0.1f;
		printf("p: %f  q: %f  r: %f\n", pqr[0], pqr[1], pqr[2]);
		InvalidateRect(p_render->h_wnd, NULL, TRUE);
	}
	break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcA(hwnd, msg, wparam, lparam);
	}
	return 0;
}

int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("conout$", "w", stdout);
	freopen("conin$", "w", stdin);
	
	WNDCLASSA wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "software_renderer";
	wc.lpfnWndProc = wndproc;
	wc.hInstance = GetModuleHandleW(0);
	if (!RegisterClassA(&wc)) {
		MessageBoxA(0, "RegisterClassA failed", "", MB_OK | MB_ICONERROR);
		return 1;
	}

	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	int ww = 800;
	int wh = 600;
	int posx = (sw - ww) >> 1;
	int posy = (sh - wh) >> 1;
	p_render->h_wnd = CreateWindowExA(0, wc.lpszClassName, "software renderer", WS_OVERLAPPEDWINDOW, posx, posy, ww, wh, 0, (HMENU)0, NULL, NULL);
	if (!p_render->h_wnd) {
		MessageBoxA(0, "failed to create window", "", MB_OK);
		return 2;
	}

	h_dialog = CreateDialogW(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), p_render->h_wnd, (DLGPROC)wndproc);
	h_p = GetDlgItem(h_dialog, IDC_SLIDER1);
	h_q = GetDlgItem(h_dialog, IDC_SLIDER2);
	h_r = GetDlgItem(h_dialog, IDC_SLIDER3);

	ShowWindow(h_dialog, SW_SHOW);
	UpdateWindow(h_dialog);

	ShowWindow(p_render->h_wnd, SW_SHOW);
	UpdateWindow(p_render->h_wnd);

	MSG msg;
	while (GetMessageA(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
    return 0;
}