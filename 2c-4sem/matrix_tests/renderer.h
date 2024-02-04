#pragma once
#include <Windows.h> //temp use
#include <stdbool.h>
#include <stdint.h>
#include "cmathlib.h"

typedef struct projbuf_s {
	uint32_t last_size; //last allocation size
	uint32_t position; //last position in projected verts buffer
	uint32_t num_verts; //current size of vertex buffer (in elements)
	vec3_t *p_unproj_verts; //unproject (original) shape vertices
	vec4_t *p_proj_verts; //project shape vertices
} projbuf_t;

#define R_MAX_MAT_STACK 32

typedef struct mat_stack_s {
	uint32_t position; //current position in stack of matrices
	mat44_t stack[R_MAX_MAT_STACK]; // R_MAX_MAT_STACK matrix instances
} mat_stack_t;

enum R_MAT_STACKS {
	R_MAT_PROJECTION = 0,
	R_MAT_MODEL,

	MAX_STACKS
};

typedef struct renderer_s {
	RECT fb_rect; //frame buffer size
	HDC h_memdc; //win32 memdc
	HBITMAP h_framebuffer; //win32 frame buffer (bitmap)
	HWND h_wnd; //win32 window
	HBRUSH h_background_brush; // win32 brush for background fill

	vec2_t screen; //half of screen size
	uint32_t flags; //render global state flags
	projbuf_t proj_buffer; //projection vertices buffer info
	uint32_t current_matrix; // current matrix index
	union {
		struct {
			mat44_t projection; // proj matrix
			mat44_t model; // model matrix
		};
		mat44_t matrices[MAX_STACKS]; //union of projection and model matrices
	};
	mat44_t mp; // model * projection
	mat_stack_t stacks[MAX_STACKS]; // model and projection matrices stacks
	vec3_t clear_color; //screen background color
	vec4_t prim_color; //current primitive color
	int point_size; //point size in pixels
} renderer_t;

#define R_GEN_PROJECT_BUFFER 0

enum R_ERROR {
	R_NO_ERROR = 0,
	R_INVALID_PARAMETER,
	R_STACK_OVERFLOW,
	R_STACK_UNDERFLOW,
	R_OUT_OF_MEMORY
};

#define R_CLEAR_COLOR (1 << 0)
#define R_CLEAR_DEPTH (1 << 1)
#define R_CLEAR_STENCIL (1 << 2)

bool r_init(renderer_t *p_render);
void r_shutdown(renderer_t *p_render);
bool r_draw_quads(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts);
bool r_draw_tris(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts);
bool r_draw_lines(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts);
bool r_draw_lines_indexed(renderer_t *p_render, const vec3_t *p_verts, const uint32_t *p_indices, uint32_t num_indices);
bool r_draw_polyline(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts);
bool r_draw_points(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts);
void r_clear(renderer_t *p_render, int flags);
void r_enable(renderer_t *p_render, int opt);
void r_disable(renderer_t *p_render, int opt);
bool r_is_enabled(renderer_t *p_render, int opt);
void r_point_size(renderer_t *p_render, int sizeinpixels);
int  r_select_matrix(renderer_t *p_render, int mat_stack);
int  r_push_matrix(renderer_t *p_render);
int  r_pop_matrix(renderer_t *p_render);
void r_clear_color(renderer_t *p_render, vec3_t color);
void r_color(renderer_t *p_render, vec4_t color);
const mat_stack_t *r_get_stack(renderer_t *p_render, int mat_stack);
const projbuf_t *r_query_proj_buf(renderer_t *p_render);
void r_begin_frame(void *p_device, renderer_t *p_render);
void r_present(void *p_device, renderer_t *p_render);
void r_end_frame(renderer_t *p_render);