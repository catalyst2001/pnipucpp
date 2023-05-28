#include <Windows.h>
#include <Windowsx.h>
#include <stdio.h>
#include <glm/glm.hpp>

using glm::vec2;

#define prt(x, ...) printf(x, __VA_ARGS__)
#define err(c, x, ...) { printf(x, __VA_ARGS__); return c; }
#define msgbox(msg) MessageBoxA(0, 0, msg, 0);

HWND h_wnd;
HDC cdc;
HDC memdc;
HBITMAP hbitmap;
RECT rect;
RECT prevrect;

#define MAX_POINTS 100
size_t num_points = 0;
POINT saves[MAX_POINTS];
int hw = 0;
int hh = 0;

void save_pt(const LPPOINT ppt)
{
	if (num_points == MAX_POINTS) {
		msgbox("points max");
		return;
	}
	saves[num_points].x = (float)ppt->x;
	saves[num_points].y = (float)ppt->y;
	num_points++;
}

void toscreen(LPPOINT pdst, const LPPOINT psrc)
{
	pdst->x = hw + psrc->x;
	pdst->y = hh + psrc->y;
}

void tolocal(LPPOINT pdst, const LPPOINT psrc)
{
	pdst->x = psrc->x - hw;
	pdst->y = psrc->y - hh;
}

void drawpoly(HDC hdc)
{
	if (num_points < 2)
		return;

	POINT pt;
	POINT transformed;
	toscreen(&transformed, &saves[0]);
	MoveToEx(hdc, transformed.x, transformed.y, &pt);
	for (size_t i = 1; i < num_points; i++) {
		toscreen(&transformed, &saves[i]);
		LineTo(hdc, transformed.x, transformed.y);
	}
	toscreen(&transformed, &saves[0]);
	LineTo(hdc, transformed.x, transformed.y);
	MoveToEx(hdc, pt.x, pt.y, NULL);
}

void drawgird(HDC hdc, LPRECT prect, int factor)
{
	for (int y = 0; y < prect->bottom; y += factor) {
		for (int x = 0; x < prect->right; x += factor) {
			SetPixel(hdc, x, y, RGB(0, 0, 0));
		}
	}
}

void drawline(HDC hdc, int x1, int y1, int x2, int y2)
{
	POINT pt;
	MoveToEx(hdc, x1, y1, &pt);
	LineTo(hdc, x2, y2);
	MoveToEx(hdc, pt.x, pt.y, &pt);
}

void drawcentercoords(HDC hdc, LPCRECT prect)
{
	drawline(hdc, hw, 0, hw, prect->bottom);
	drawline(hdc, 0, hh, prect->right, hh);
}

LRESULT CALLBACK wndproc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	switch (msg)
	{
	case WM_PAINT:{
		PAINTSTRUCT ps;
		GetClientRect(wnd, &rect);
		HDC hdc = BeginPaint(wnd, &ps);
		SetBkMode(memdc, TRANSPARENT);
		hw = rect.right / 2;
		hh = rect.bottom / 2;

		if (!EqualRect(&rect, &prevrect)) {
			cdc = GetDC(wnd);
			hbitmap = CreateCompatibleBitmap(cdc, rect.right, rect.bottom);
			HBITMAP oldbitmap = (HBITMAP)SelectObject(memdc, hbitmap);
			if (oldbitmap) {
				DeleteObject(oldbitmap);
				prt("remove bmp\n");
			}

			ReleaseDC(wnd, cdc);
		}
		FillRect(memdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		drawgird(memdc, &rect, 10);
		drawcentercoords(memdc, &rect);
		drawpoly(memdc);


		TextOutA(memdc, rect.right-20, hh, "X", 1);
		TextOutA(memdc, hw, 0, "Y", 1);

		BitBlt(hdc, 0, 0, rect.right, rect.bottom, memdc, 0, 0, SRCCOPY);
		EndPaint(wnd, &ps);
		prevrect = rect;
		break;
		}


	case WM_LBUTTONDOWN: {
		POINT mpt;
		mpt.x = GET_X_LPARAM(lparam);
		mpt.y = GET_Y_LPARAM(lparam);
		tolocal(&mpt, &mpt);
		save_pt(&mpt);
		InvalidateRect(wnd, NULL, TRUE);
		break;
	}

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_SIZE: {
		
		
		break;
	}

	default:
		return DefWindowProcA(wnd, msg, wparam, lparam);
	}
	return 0;
}

int main()
{
	WNDCLASSA wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = "Bystreee";
	wc.hInstance = GetModuleHandleW(0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = wndproc;
	if (!RegisterClassA(&wc))
		err(1, "register class faiuled")

	int ww = 800;
	int wh = 600;
	int x = GetSystemMetrics(SM_CXSCREEN) / 2 - ww / 2;
	int y = GetSystemMetrics(SM_CYSCREEN) / 2 - wh / 2;
	h_wnd = CreateWindowExA(0, wc.lpszClassName, "points editor fast", WS_OVERLAPPEDWINDOW, x, y, ww, wh, 0, (HMENU)0, 0,0);
	if (!h_wnd)
		err(1, "failed to create window")

	GetClientRect(h_wnd, &rect);
	 cdc = GetDC(h_wnd);
	memdc = CreateCompatibleDC(cdc);
	hbitmap = CreateCompatibleBitmap(cdc, rect.right, rect.bottom);
	SelectObject(memdc, hbitmap);
	ReleaseDC(h_wnd, memdc);

	ShowWindow(h_wnd, SW_SHOW);
	UpdateWindow(h_wnd);

	MSG msg;
	while (GetMessageA(&msg, 0, 0, 0))
		DispatchMessageA(&msg);

	DeleteObject(wc.hbrBackground);
	return 0;
}