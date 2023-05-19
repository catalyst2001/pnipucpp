#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "../cgcommon/glm/glm.hpp"

using glm::vec2;

HINSTANCE g_instance;
HWND h_wnd;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

struct vec2i {
	vec2i() : x(0), y(0) { }
	vec2i(int xx, int yy) : x(xx), y(yy) { }

	int x, y;
};

vec2i rocket[] = {
	vec2i(0, 3),
	vec2i(1, 2),
	vec2i(1, -2),
	vec2i(2, -3),
	vec2i(-2, -3),
	vec2i(-1, -2),
	vec2i(-1, 2)
};

void draw_polygon(HDC hdc, const vec2i *p_verts, int count)
{
	if (count < 2)
		return;

	POINT previous;
	MoveToEx(hdc, p_verts[0].x, p_verts[0].y, &previous);
	Polygon(hdc, (const POINT *)&p_verts[1], count - 1);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEXA wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
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
	h_wnd = CreateWindowExA(0, wcex.lpszClassName, "Lab1 cg", WS_OVERLAPPEDWINDOW, xpos, ypos, width, height, NULL, NULL, hInstance, NULL);
	if (!h_wnd) {
		MessageBoxA(HWND_DESKTOP, "Couldn't create window", "Critical error", MB_OK | MB_ICONERROR);
		return 2;
	}
	ShowWindow(h_wnd, nCmdShow);
	UpdateWindow(h_wnd);

	for (int i = 0; i < sizeof(rocket) / sizeof(rocket[0]); i++) {
		rocket[i].x *= 20;
		rocket[i].y *= 20;
	}

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	static POINT pt;
	static POINT start_pt = {0, 0};
	static BOOL is_move = FALSE;
    switch (message)
    {

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			draw_polygon(hdc, rocket, sizeof(rocket) / sizeof(rocket[0]));
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	case WM_LBUTTONDOWN:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		is_move = TRUE;
		break;

	case WM_MOUSEMOVE: {
		POINT mousept;
		if (is_move) {
			mousept.x = LOWORD(lParam);
			mousept.y = HIWORD(lParam);
			start_pt.x += mousept.x - pt.x;
			start_pt.y += mousept.y - pt.y;
			pt = mousept;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	}

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}