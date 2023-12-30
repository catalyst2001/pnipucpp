#pragma once
#include <Windows.h> //temp use
#include <stdbool.h>

typedef struct renderer_s {
	RECT fb_rect;
	HDC h_memdc;
	HBITMAP h_framebuffer;
} renderer_t;

bool r_init();
void r_shutdown();
void r_draw_quad();
void r_draw_tri();
void r_draw_line();
void r_darw_polyline();