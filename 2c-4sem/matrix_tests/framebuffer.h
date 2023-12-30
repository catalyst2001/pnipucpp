#pragma once
#include "common.h"

#define FB_NEED_TO_UPDATE (1 << 0)
#define FB_SCISSOR (1 << 1)

typedef struct frame_buffer_s {
	uint32_t flags;
	uint32_t depth_bits;
	uint32_t bits_per_pixel;
	uint32_t width;
	uint32_t height;
	color4_t *p_pixels;
} frame_buffer_t;

bool fb_create(frame_buffer_t *p_fbs, uint32_t count);
bool fb_delete(frame_buffer_t *p_fbs, uint32_t count);
void fb_scissor(frame_buffer_t *p_fb, int x, int y, int width, int height);