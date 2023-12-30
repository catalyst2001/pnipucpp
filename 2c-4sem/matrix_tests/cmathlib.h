#pragma once
#include <stdbool.h>
#include <float.h>
#include <math.h>

/* VECTOR 2 */
typedef struct vec2_s {
	union {
		float v[2];
		struct { float x, y; };
	};
} vec2_t;

void  vec2_set(vec2_t *p_dst, float x, float y, float z);
void  vec2_mul(vec2_t *p_dst, const vec2_t *p_veca, const vec2_t *p_vecb);
void  vec2_scale(vec2_t *p_dst, const vec2_t *p_src, float sc);
void  vec2_div(vec2_t *p_dst, const vec2_t *p_veca, const vec2_t *p_vecb);
void  vec2_divs(vec2_t *p_dst, const vec2_t *p_veca, float sc);
bool  vec2_normalable(const vec2_t *p_src_vec);
void  vec2_trynorm(vec2_t *p_vec);
void  vec2_norm(vec2_t *p_vec);
float vec2_lengthsq(const vec2_t *p_src);
float vec2_length(const vec2_t *p_src);
void  vec2_reflect(vec2_t *p_vec);
float vec2_dot(const vec2_t *p_veca, const vec2_t *p_vecb);
void  vec2_cross(vec2_t *p_dst, const vec2_t *p_veca, const vec2_t *p_vecb);

/* VECTOR 3 */
typedef struct vec3_s {
	union {
		float v[3];
		struct { float x, y, z; };
	};
} vec3_t;

void  vec3_set(vec3_t *p_dst, float x, float y, float z);
void  vec3_mul(vec3_t *p_dst, const vec3_t *p_veca, const vec3_t *p_vecb);
void  vec3_muls(vec3_t *p_dst, const vec3_t *p_src, float sc);
void  vec3_div(vec3_t *p_dst, const vec3_t *p_veca, const vec3_t *p_vecb);
void  vec3_scale(vec3_t *p_dst, const vec3_t *p_veca, float sc);
bool  vec3_normalable(const vec3_t *p_src_vec);
void  vec3_trynorm(vec3_t *p_vec);
void  vec3_norm(vec3_t *p_vec);
float vec3_lengthsq(const vec3_t *p_src);
float vec3_length(const vec3_t *p_src);
void  vec3_reflect(vec3_t *p_vec);
float vec3_dot(const vec3_t *p_veca, const vec3_t *p_vecb);
void  vec3_cross(vec3_t *p_dst, const vec3_t *p_veca, const vec3_t *p_vecb);

/* VECTOR 4 */
typedef struct vec4_s {
	union {
		struct { float x, y, z, w; };
		float v[4];
	};
} vec4_t;

void  vec4_set(vec4_t *p_dst, float x, float y, float z, float w);
void  vec4_mul(vec4_t *p_dst, const vec4_t *p_veca, const vec4_t *p_vecb);
void  vec4_muls(vec4_t *p_dst, const vec4_t *p_src, float sc);
void  vec4_div(vec4_t *p_dst, const vec4_t *p_veca, const vec4_t *p_vecb);
void  vec4_scale(vec3_t *p_dst, const vec4_t *p_veca, float sc);
bool  vec4_normalable(const vec4_t *p_src_vec);
void  vec4_trynorm(vec4_t *p_vec);
void  vec4_norm(vec4_t *p_vec);
float vec4_lengthsq(const vec4_t *p_src);
float vec4_length(const vec4_t *p_src);
void  vec4_reflect(vec4_t *p_vec);
float vec4_dot(const vec4_t *p_veca, const vec4_t *p_vecb);
void  vec4_cross(vec4_t *p_dst, const vec4_t *p_veca, const vec4_t *p_vecb);

/* MATRIX2x2 */
typedef float mat22_t[2][2];
void  mat22_ident(mat22_t p_dst);
void  mat22_transpose(mat22_t p_dst);
void  mat22_mult(mat22_t p_dst, const mat22_t p_mata, const mat22_t p_matb);
float mat22_det(const mat22_t p_src);

/* MATRIX3x3 */
typedef float mat33_t[3][3];
void  mat33_ident(mat33_t p_dst);
void  mat33_transpose(mat33_t p_dst);
void  mat33_mul(mat33_t p_dst, const mat33_t p_mata, const mat33_t p_matb);
void  mat33_mult_vec2(vec2_t *p_dst, const vec2_t p_src, const mat33_t p_mat);
float mat33_det(const mat33_t p_src);

// [sx] [  ] [   ]
// [  ] [sy] [   ]
// [  ] [  ] [1.0]
void  mat33_scale(mat33_t p_mat, float sx, float sy, float sz);

// [ ] [ ] [ ]
// [ ] [ ] [ ]
// [m] [n] [ ]
void  mat33_offset(mat33_t p_mat, float m, float n);


/* MATRIX4x4 */
typedef float mat44_t[4][4];
void  mat44_row(vec4_t *p_dst, const mat44_t p_mat, int rowidx);
void  mat44_col(vec4_t *p_dst, const mat44_t p_mat, int colidx);
void  mat44_ident(mat44_t p_dst);
void  mat44_transpose(mat44_t p_dst);
void  mat44_mul(mat44_t p_dst, const mat44_t p_mata, const mat44_t p_matb);
void  mat44_mul_vec4(vec4_t *p_dst, const mat44_t p_mat, const vec4_t *p_src);
float mat44_det(const mat44_t p_src);
void  mat44_translate(mat44_t p_mat, const vec3_t *p_pos);
void  mat44_rotate_x(mat44_t p_mat, float angle);
void  mat44_rotate_y(mat44_t p_mat, float angle);
void  mat44_rotate_z(mat44_t p_mat, float angle);
void  mat44_rotate(mat44_t p_mat, const vec3_t *p_rot); // complex rotation
void  mat44_scale(mat44_t p_mat, const vec3_t *p_scl);

/* QUAT */
typedef vec4_t quat_t;








typedef struct transform_s {
	mat44_t transform;
	vec3_t position;
	quat_t rotation;
} transform_t;