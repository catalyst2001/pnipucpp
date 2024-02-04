#include "renderer.h"
#include <assert.h>

bool proj_buffer_resize(projbuf_t *p_buf, uint32_t num_verts)
{
	p_buf->num_verts = num_verts;
	if (!p_buf->num_verts)
		return true;

	if (p_buf->last_size < p_buf->num_verts) {
		p_buf->p_proj_verts = (vec4_t *)realloc(p_buf->p_proj_verts, p_buf->num_verts * sizeof(vec4_t));
		return p_buf->p_proj_verts != NULL;
	}
	return true;
}

void proj_buffer_free(projbuf_t *p_buf)
{
	if (p_buf && p_buf->p_proj_verts) {
		free(p_buf->p_proj_verts);
		p_buf->p_proj_verts = NULL;
	}
}

void proj_buf_reset(projbuf_t *p_buf)
{
	p_buf->position = 0;
}

void proj_buf_put_projected_vert(projbuf_t *p_buf, const vec4_t *p_src)
{
	assert(p_buf->position < p_buf->num_verts);
	p_buf->p_proj_verts[p_buf->position++] = *p_src;
}

void r_recompute_globals(renderer_t *p_render)
{
	p_render->screen.x = p_render->fb_rect.right / 2.f;
	p_render->screen.y = p_render->fb_rect.bottom / 2.f;
	mat44_mul(p_render->mp, p_render->projection, p_render->model);
}

bool r_init(renderer_t *p_render)
{
	memset(p_render, 0, sizeof(*p_render));
	mat44_ident(&p_render->projection);
	mat44_ident(&p_render->model);
	p_render->current_matrix = R_MAT_MODEL;
	return true;
}

void win32_draw_line(HDC hdc, int sx, int sy, int ex, int ey)
{
	MoveToEx(hdc, sx, sy, NULL);
	LineTo(hdc, ex, ey);
}

void win32_draw_triangle(HDC hdc, int p1x, int p1y, int p2x, int p2y, int p3x, int p3y)
{
	MoveToEx(hdc, p1y, p1y, NULL);
	LineTo(hdc, p2x, p2y);
	LineTo(hdc, p3x, p3y);
	LineTo(hdc, p1y, p1y);
}

void win32_fill_color(HDC hdc, HBRUSH h_brush, int x, int y, int width, int height)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
	FillRect(hdc, &rect, h_brush);
}

void r_shutdown(renderer_t *p_render)
{
	proj_buffer_free(&p_render->proj_buffer);
}

inline void project_vert(vec4_t *p_dst, renderer_t *p_render, const vec3_t *p_vert)
{
	vec4_copy_vec3(p_dst, p_vert); //copy.  W = 1.f
	mat44_mul_vec4(p_dst, p_render->mp, p_dst); //project
	vec4_divw(p_dst); // X,Y,Z div by W if W greather FLT_EPSILON

	/* scale to screen */
	p_dst->x *= p_render->screen.x;
	p_dst->y *= -p_render->screen.y;

	/* move to center of screen */
	p_dst->x += p_render->screen.x;
	p_dst->y += p_render->screen.y;

	/* push projected vertex to buffer if this enabled */
	if (r_is_enabled(p_render, R_GEN_PROJECT_BUFFER))
		proj_buf_put_projected_vert(&p_render->proj_buffer, p_dst);
}

bool r_draw_quads(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts)
{
	if (r_is_enabled(p_render, R_GEN_PROJECT_BUFFER) && !proj_buffer_resize(&p_render->proj_buffer, num_verts))
		return false;

	return true;
}

bool r_draw_tris(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts)
{
	vec4_t p0, p1, p2;
	if (r_is_enabled(p_render, R_GEN_PROJECT_BUFFER)) {
		proj_buf_reset(&p_render->proj_buffer); // reset position in buffer
		if (!proj_buffer_resize(&p_render->proj_buffer, num_verts)) // resize projected buffer
			return false;
	}

	r_recompute_globals(p_render); //recompute mp mat and screen half width and height
	for (uint32_t i = 0; i < num_verts; i += 3) {
		project_vert(&p0, p_render, &p_verts[i + 0]);
		project_vert(&p1, p_render, &p_verts[i + 1]);
		project_vert(&p2, p_render, &p_verts[i + 2]);
		win32_draw_triangle(p_render->h_memdc, (int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
	}
	return true;
}

bool r_draw_lines(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts)
{
	vec4_t p0, p1;
	if (r_is_enabled(p_render, R_GEN_PROJECT_BUFFER)) {
		proj_buf_reset(&p_render->proj_buffer); // reset position in buffer
		if (!proj_buffer_resize(&p_render->proj_buffer, num_verts)) // resize projected buffer
			return false;
	}

	r_recompute_globals(p_render); //recompute mp mat and screen half width and height
	for (uint32_t i = 0; i < num_verts; i += 2) {
		project_vert(&p0, p_render, &p_verts[i + 0]);
		project_vert(&p1, p_render, &p_verts[i + 1]);
		win32_draw_line(p_render->h_memdc, (int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y);
	}
	return true;
}

bool r_draw_lines_indexed(renderer_t *p_render, const vec3_t *p_verts, const uint32_t *p_indices, uint32_t num_indices)
{
	vec4_t p0, p1;
	if (r_is_enabled(p_render, R_GEN_PROJECT_BUFFER)) {
		proj_buf_reset(&p_render->proj_buffer); // reset position in buffer
		if(!proj_buffer_resize(&p_render->proj_buffer, num_indices)) // resize projected buffer
			return false;
	}
	
	r_recompute_globals(p_render); //recompute mp mat and screen half width and height
	for (uint32_t i = 0; i < num_indices; i += 2) {
		project_vert(&p0, p_render, &p_verts[p_indices[i + 0]]);
		project_vert(&p1, p_render, &p_verts[p_indices[i + 1]]);
		win32_draw_line(p_render->h_memdc, (int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y);
	}
	return true;
}

bool r_draw_polyline(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts)
{
}

bool r_draw_points(renderer_t *p_render, const vec3_t *p_verts, uint32_t num_verts)
{
}

void r_clear(renderer_t *p_render, int flags)
{
	if (flags & R_CLEAR_COLOR) {
		win32_fill_color(p_render->h_memdc,
			p_render->h_background_brush,
			p_render->fb_rect.left,
			p_render->fb_rect.top,
			abs(p_render->fb_rect.right - p_render->fb_rect.left),
			abs(p_render->fb_rect.bottom - p_render->fb_rect.top));
	}
}

void r_enable(renderer_t *p_render, int opt)
{
	p_render->flags |= (1 << opt);
}

void r_disable(renderer_t *p_render, int opt)
{
	p_render->flags &= ~(1 << opt);
}

bool r_is_enabled(renderer_t *p_render, int opt)
{
	return p_render->flags & (1 << opt);
}

void r_point_size(renderer_t *p_render, int sizeinpixels)
{
	p_render->point_size = sizeinpixels;
}

int r_select_matrix(renderer_t *p_render, int matrix)
{
	if (matrix < 0 || matrix >= MAX_STACKS)
		return R_INVALID_PARAMETER;

	p_render->current_matrix = matrix;
	return R_NO_ERROR;
}

int r_push_matrix(renderer_t *p_render)
{
	mat_stack_t *p_matrix_stack = &p_render->stacks[p_render->current_matrix];
	if (p_matrix_stack->position == R_MAX_MAT_STACK)
		return R_STACK_OVERFLOW;

	memcpy(p_matrix_stack->stack[p_matrix_stack->position], p_render->matrices[p_render->current_matrix], sizeof(mat44_t));
	p_matrix_stack->position++;
	return R_NO_ERROR;
}

int r_pop_matrix(renderer_t *p_render)
{
	mat_stack_t *p_matrix_stack = &p_render->stacks[p_render->current_matrix];
	if (!p_matrix_stack->position)
		return R_STACK_UNDERFLOW;

	p_matrix_stack->position--;
	memcpy(p_render->matrices[p_render->current_matrix], p_matrix_stack->stack[p_matrix_stack->position], sizeof(mat44_t));
}

void r_clear_color(renderer_t *p_render, vec3_t color)
{
	p_render->clear_color = color;
}

void r_color(renderer_t *p_render, vec4_t color)
{

}

const mat_stack_t *r_get_stack(renderer_t *p_render, int mat_stack)
{
	return &p_render->stacks[mat_stack]; //TODO: not checked errors
}

const projbuf_t *r_query_proj_buf(renderer_t *p_render)
{
	return &p_render->proj_buffer;
}

void r_begin_frame(void *p_device, renderer_t *p_render)
{
	HDC hdc = (HDC)p_device;
	if (!p_render->h_memdc) {
		p_render->h_memdc = CreateCompatibleDC(hdc);
	}

	p_render->h_framebuffer = CreateCompatibleBitmap(hdc,
		p_render->fb_rect.right,
		p_render->fb_rect.bottom
	);
	SelectObject(p_render->h_memdc, p_render->h_framebuffer);
}

void r_end_frame(renderer_t *p_render)
{
	if (p_render->h_framebuffer) {
		DeleteObject(p_render->h_framebuffer);
		p_render->h_framebuffer = NULL;
	}
}

void r_present(void *p_device, renderer_t *p_render)
{
	HDC hdc = (HDC)p_device;
	BitBlt(hdc, p_render->fb_rect.left,
		p_render->fb_rect.top,
		p_render->fb_rect.right,
		p_render->fb_rect.bottom,
		p_render->h_memdc,
		0, 0,
		SRCCOPY);
}
