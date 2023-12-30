/*
 Author: $username$
 Date: $time$
 File: $itemname$
 
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct color4_s {
	union {
		struct { uint8_t r, g, b, a; };
		uint8_t v[4];
		uint32_t color;
	};
} color4_t;