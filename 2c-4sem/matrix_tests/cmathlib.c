#include "cmathlib.h"

void vec2_set(vec2_t *p_dst, float x, float y, float z)
{
	p_dst->x = x;
	p_dst->y = y;
}

void vec2_mul(vec2_t *p_dst, const vec2_t *p_veca, const vec2_t *p_vecb)
{
	p_dst->x = p_veca->x * p_vecb->x;
	p_dst->y = p_veca->y * p_vecb->y;
}

void vec2_scale(vec2_t *p_dst, const vec2_t *p_src, float sc)
{
	p_dst->x = p_src->x * sc;
	p_dst->y = p_src->y * sc;
}

void vec2_div(vec2_t *p_dst, const vec2_t *p_veca, const vec2_t *p_vecb)
{
	p_dst->x = p_veca->x / p_vecb->x;
	p_dst->y = p_veca->y / p_vecb->y;
}

void vec2_divs(vec2_t *p_dst, const vec2_t *p_veca, float sc)
{
	p_dst->x = p_veca->x / sc;
	p_dst->y = p_veca->y / sc;
}

bool vec2_normalable(const vec2_t *p_src_vec)
{
	return false;
}

void vec2_trynorm(vec2_t *p_vec)
{
	float l = vec2_length(p_vec);
	if (l < FLT_EPSILON)
		l = 1.f;

	p_vec->x /= l;
	p_vec->y /= l;
}

void vec2_norm(vec2_t *p_vec)
{
	float l = vec2_length(p_vec);
	p_vec->x /= l;
	p_vec->y /= l;
}

float vec2_lengthsq(const vec2_t *p_src)
{
	return p_src->x * p_src->x + p_src->y * p_src->y;
}

float vec2_length(const vec2_t *p_src)
{
	return sqrtf(vec2_lengthsq(p_src));
}

void vec2_reflect(vec2_t *p_vec)
{
	p_vec->x *= -1.f;
	p_vec->y *= -1.f;
}

float vec2_dot(const vec2_t *p_veca, const vec2_t *p_vecb)
{
}

void vec2_cross(vec2_t *p_dst, const vec2_t *p_veca, const vec2_t *p_vecb)
{
}

void vec3_set(vec3_t *p_dst, float x, float y, float z)
{
	p_dst->x = x;
	p_dst->y = y;
	p_dst->z = z;
}

void vec3_mul(vec3_t *p_dst, const vec3_t *p_veca, const vec3_t *p_vecb)
{
	p_dst->x = p_veca->x * p_vecb->x;
	p_dst->y = p_veca->y * p_vecb->y;
	p_dst->z = p_veca->z * p_vecb->z;
}

void vec3_muls(vec3_t *p_dst, const vec3_t *p_src, float sc)
{
}

void vec3_div(vec3_t *p_dst, const vec3_t *p_veca, const vec3_t *p_vecb)
{
}

void vec3_scale(vec3_t *p_dst, const vec3_t *p_veca, float sc)
{
}

bool vec3_normalable(const vec3_t *p_src_vec)
{
}

void vec3_trynorm(vec3_t *p_vec)
{
}

void vec3_norm(vec3_t *p_vec)
{
}

float vec3_lengthsq(const vec3_t *p_src)
{
}

float vec3_length(const vec3_t *p_src)
{
}

void  vec3_reflect(vec3_t *p_vec)
{
}

float vec3_dot(const vec3_t *p_veca, const vec3_t *p_vecb)
{
}

void vec3_cross(vec3_t *p_dst, const vec3_t *p_veca, const vec3_t *p_vecb)
{
}

bool vec3_less(const vec3_t *p_veca, const vec3_t *p_vecb)
{
	return (p_veca->x < p_vecb->x && p_veca->y < p_vecb->y && p_veca->z < p_vecb->z);
}

bool vec3_lesseq(const vec3_t * p_veca, const vec3_t * p_vecb)
{
	return (p_veca->x <= p_vecb->x && p_veca->y <= p_vecb->y && p_veca->z <= p_vecb->z);
}

bool vec3_equals(const vec3_t *p_veca, const vec3_t *p_vecb)
{
	return (p_veca->x == p_vecb->x && p_veca->y == p_vecb->y && p_veca->z == p_vecb->z);
}

bool vec3_greater(const vec3_t *p_veca, const vec3_t *p_vecb)
{
	return (p_veca->x > p_vecb->x && p_veca->y > p_vecb->y && p_veca->z > p_vecb->z);
}

bool vec3_greateq(const vec3_t * p_veca, const vec3_t * p_vecb)
{
	return (p_veca->x >= p_vecb->x && p_veca->y >= p_vecb->y && p_veca->z >= p_vecb->z);
}

void vec4_copy_vec3(vec4_t *p_dst, const vec3_t *p_src)
{
	p_dst->x = p_src->x;
	p_dst->y = p_src->y;
	p_dst->z = p_src->z;
	p_dst->w = 1.f;
}

void vec4_set(vec4_t *p_dst, float x, float y, float z, float w)
{
	p_dst->x = x;
	p_dst->y = y;
	p_dst->z = z;
	p_dst->w = w;
}

void vec4_mul(vec4_t *p_dst, const vec4_t *p_veca, const vec4_t *p_vecb)
{
}

void vec4_muls(vec4_t *p_dst, const vec4_t *p_src, float sc)
{
}

void vec4_div(vec4_t *p_dst, const vec4_t *p_veca, const vec4_t *p_vecb)
{
}

void vec4_scale(vec3_t *p_dst, const vec4_t *p_veca, float sc)
{
}

void vec4_divw(vec4_t *p_dst)
{
	if (p_dst->w >= FLT_EPSILON) {
		p_dst->x /= p_dst->w;
		p_dst->y /= p_dst->w;
		p_dst->z /= p_dst->w;
	}
}

bool vec4_normalable(const vec4_t *p_src_vec)
{
}

void vec4_trynorm(vec4_t *p_vec)
{
}

void vec4_norm(vec4_t *p_vec)
{
}

float vec4_lengthsq(const vec4_t *p_src)
{
}

float vec4_length(const vec4_t *p_src)
{
}

void vec4_reflect(vec4_t *p_vec)
{
}

float vec4_dot(const vec4_t *p_veca, const vec4_t *p_vecb)
{
}

void vec4_cross(vec4_t *p_dst, const vec4_t *p_veca, const vec4_t *p_vecb)
{
}

void mat33_ident(mat33_t p_dst)
{
	p_dst[0][0] = 1.f;
	p_dst[0][1] = 0.f;
	p_dst[0][2] = 0.f;
	p_dst[1][0] = 0.f;
	p_dst[1][1] = 1.f;
	p_dst[1][2] = 0.f;
	p_dst[2][0] = 0.f;
	p_dst[2][1] = 0.f;
	p_dst[2][2] = 1.f;
}

void mat33_transpose(mat33_t p_dst)
{
	float temp;
	for (size_t j = 0; j < 3; j++) {
		for (size_t i = 0; i < 3; i++) {
			temp = p_dst[i][j];
			p_dst[i][j] = p_dst[j][i];
			p_dst[j][i] = temp;
		}
	}
}

void mat33_mul(mat33_t p_dst, const mat33_t p_mata, const mat33_t p_matb)
{
	p_dst[0][0] = p_mata[0][0] * p_matb[0][0] + p_mata[0][1] * p_matb[1][0] + p_mata[0][2] * p_matb[2][0];
	p_dst[0][1] = p_mata[0][0] * p_matb[0][1] + p_mata[0][1] * p_matb[1][1] + p_mata[0][2] * p_matb[2][1];
	p_dst[0][2] = p_mata[0][0] * p_matb[0][2] + p_mata[0][1] * p_matb[1][2] + p_mata[0][2] * p_matb[2][2];

	p_dst[1][0] = p_mata[1][0] * p_matb[0][0] + p_mata[1][1] * p_matb[1][0] + p_mata[1][2] * p_matb[2][0];
	p_dst[1][1] = p_mata[1][0] * p_matb[0][1] + p_mata[1][1] * p_matb[1][1] + p_mata[1][2] * p_matb[2][1];
	p_dst[1][2] = p_mata[1][0] * p_matb[0][2] + p_mata[1][1] * p_matb[1][2] + p_mata[1][2] * p_matb[2][2];

	p_dst[2][0] = p_mata[2][0] * p_matb[0][0] + p_mata[2][1] * p_matb[1][0] + p_mata[2][2] * p_matb[2][0];
	p_dst[2][1] = p_mata[2][0] * p_matb[0][1] + p_mata[2][1] * p_matb[1][1] + p_mata[2][2] * p_matb[2][1];
	p_dst[2][2] = p_mata[2][0] * p_matb[0][2] + p_mata[2][1] * p_matb[1][2] + p_mata[2][2] * p_matb[2][2];
}
void mat33_scale(mat33_t p_mat, float sx, float sy)
{
	p_mat[0][0] = sx;
	p_mat[0][1] = 0.f;
	p_mat[0][2] = 0.f;

	p_mat[1][0] = 0.f;
	p_mat[1][1] = sy;
	p_mat[1][2] = 0.f;

	p_mat[2][0] = 0.f;
	p_mat[2][1] = 0.f;
	p_mat[2][2] = 1.f;
}

void mat44_row(vec4_t *p_dst, const mat44_t p_mat, int rowidx)
{
	p_dst->x = p_mat[rowidx][0];
	p_dst->y = p_mat[rowidx][1];
	p_dst->z = p_mat[rowidx][2];
	p_dst->w = p_mat[rowidx][3];
}

void mat44_col(vec4_t *p_dst, const mat44_t p_mat, int colidx)
{
	p_dst->x = p_mat[0][colidx];
	p_dst->y = p_mat[1][colidx];
	p_dst->z = p_mat[2][colidx];
	p_dst->w = p_mat[3][colidx];
}

void mat44_ident(mat44_t p_dst)
{
	p_dst[0][0] = 1.f;
	p_dst[0][1] = 0.f;
	p_dst[0][2] = 0.f;
	p_dst[0][3] = 0.f;

	p_dst[1][0] = 0.f;
	p_dst[1][1] = 1.f;
	p_dst[1][2] = 0.f;
	p_dst[1][3] = 0.f;

	p_dst[2][0] = 0.f;
	p_dst[2][1] = 0.f;
	p_dst[2][2] = 1.f;
	p_dst[2][3] = 0.f;

	p_dst[3][0] = 0.f;
	p_dst[3][1] = 0.f;
	p_dst[3][2] = 0.f;
	p_dst[3][3] = 1.f;
}

void mat44_transpose(mat44_t p_dst)
{
	float temp;
	for (int i = 0; i < 4; ++i) {
		for (int j = i + 1; j < 4; ++j) {
			temp = p_dst[i][j];
			p_dst[i][j] = p_dst[j][i];
			p_dst[j][i] = temp;
		}
	}
}

// [row][col]
void mat44_mul(mat44_t p_dst, const mat44_t p_mata, const mat44_t p_matb)
{
	p_dst[0][0] = p_mata[0][0] * p_matb[0][0] + p_mata[0][1] * p_matb[1][0] + p_mata[0][2] * p_matb[2][0] + p_mata[0][3] * p_matb[3][0];
	p_dst[0][1] = p_mata[0][0] * p_matb[0][1] + p_mata[0][1] * p_matb[1][1] + p_mata[0][2] * p_matb[2][1] + p_mata[0][3] * p_matb[3][1];
	p_dst[0][2] = p_mata[0][0] * p_matb[0][2] + p_mata[0][1] * p_matb[1][2] + p_mata[0][2] * p_matb[2][2] + p_mata[0][3] * p_matb[3][2];
	p_dst[0][3] = p_mata[0][0] * p_matb[0][3] + p_mata[0][1] * p_matb[1][3] + p_mata[0][2] * p_matb[2][3] + p_mata[0][3] * p_matb[3][3];

	p_dst[1][0] = p_mata[1][0] * p_matb[0][0] + p_mata[1][1] * p_matb[1][0] + p_mata[1][2] * p_matb[2][0] + p_mata[1][3] * p_matb[3][0];
	p_dst[1][1] = p_mata[1][0] * p_matb[0][1] + p_mata[1][1] * p_matb[1][1] + p_mata[1][2] * p_matb[2][1] + p_mata[1][3] * p_matb[3][1];
	p_dst[1][2] = p_mata[1][0] * p_matb[0][2] + p_mata[1][1] * p_matb[1][2] + p_mata[1][2] * p_matb[2][2] + p_mata[1][3] * p_matb[3][2];
	p_dst[1][3] = p_mata[1][0] * p_matb[0][3] + p_mata[1][1] * p_matb[1][3] + p_mata[1][2] * p_matb[2][3] + p_mata[1][3] * p_matb[3][3];

	p_dst[2][0] = p_mata[2][0] * p_matb[0][0] + p_mata[2][1] * p_matb[1][0] + p_mata[2][2] * p_matb[2][0] + p_mata[2][3] * p_matb[3][0];
	p_dst[2][1] = p_mata[2][0] * p_matb[0][1] + p_mata[2][1] * p_matb[1][1] + p_mata[2][2] * p_matb[2][1] + p_mata[2][3] * p_matb[3][1];
	p_dst[2][2] = p_mata[2][0] * p_matb[0][2] + p_mata[2][1] * p_matb[1][2] + p_mata[2][2] * p_matb[2][2] + p_mata[2][3] * p_matb[3][2];
	p_dst[2][3] = p_mata[2][0] * p_matb[0][3] + p_mata[2][1] * p_matb[1][3] + p_mata[2][2] * p_matb[2][3] + p_mata[2][3] * p_matb[3][3];

	p_dst[3][0] = p_mata[3][0] * p_matb[0][0] + p_mata[3][1] * p_matb[1][0] + p_mata[3][2] * p_matb[2][0] + p_mata[3][3] * p_matb[3][0];
	p_dst[3][1] = p_mata[3][0] * p_matb[0][1] + p_mata[3][1] * p_matb[1][1] + p_mata[3][2] * p_matb[2][1] + p_mata[3][3] * p_matb[3][1];
	p_dst[3][2] = p_mata[3][0] * p_matb[0][2] + p_mata[3][1] * p_matb[1][2] + p_mata[3][2] * p_matb[2][2] + p_mata[3][3] * p_matb[3][2];
	p_dst[3][3] = p_mata[3][0] * p_matb[0][3] + p_mata[3][1] * p_matb[1][3] + p_mata[3][2] * p_matb[2][3] + p_mata[3][3] * p_matb[3][3];
}

void mat44_mul_vec4(vec4_t *p_dst, const mat44_t p_mat, const vec4_t *p_src)
{
	p_dst->x = p_mat[0][0] * p_src->x + p_mat[0][1] * p_src->y + p_mat[0][2] * p_src->z + p_mat[0][3] * p_src->w;
	p_dst->y = p_mat[1][0] * p_src->x + p_mat[1][1] * p_src->y + p_mat[1][2] * p_src->z + p_mat[1][3] * p_src->w;
	p_dst->z = p_mat[2][0] * p_src->x + p_mat[2][1] * p_src->y + p_mat[2][2] * p_src->z + p_mat[2][3] * p_src->w;
	p_dst->w = p_mat[3][0] * p_src->x + p_mat[3][1] * p_src->y + p_mat[3][2] * p_src->z + p_mat[3][3] * p_src->w;
}


float mat44_det(const mat44_t p_src) {
	return
		p_src[0][0] * (p_src[1][1] * (p_src[2][2] * p_src[3][3] - p_src[3][2] * p_src[2][3]) -
			p_src[1][2] * (p_src[2][1] * p_src[3][3] - p_src[3][1] * p_src[2][3]) +
			p_src[1][3] * (p_src[2][1] * p_src[3][2] - p_src[3][1] * p_src[2][2])) -
		p_src[0][1] * (p_src[1][0] * (p_src[2][2] * p_src[3][3] - p_src[3][2] * p_src[2][3]) -
			p_src[1][2] * (p_src[2][0] * p_src[3][3] - p_src[3][0] * p_src[2][3]) +
			p_src[1][3] * (p_src[2][0] * p_src[3][2] - p_src[3][0] * p_src[2][2])) +
		p_src[0][2] * (p_src[1][0] * (p_src[2][1] * p_src[3][3] - p_src[3][1] * p_src[2][3]) -
			p_src[1][1] * (p_src[2][0] * p_src[3][3] - p_src[3][0] * p_src[2][3]) +
			p_src[1][3] * (p_src[2][0] * p_src[3][1] - p_src[3][0] * p_src[2][1])) -
		p_src[0][3] * (p_src[1][0] * (p_src[2][1] * p_src[3][2] - p_src[3][1] * p_src[2][2]) -
			p_src[1][1] * (p_src[2][0] * p_src[3][2] - p_src[3][0] * p_src[2][2]) +
			p_src[1][2] * (p_src[2][0] * p_src[3][1] - p_src[3][0] * p_src[2][1]));
}

void mat44_scale(mat44_t p_mat, const vec3_t *p_scl)
{
	p_mat[0][0] = p_scl->x;
	p_mat[1][1] = p_scl->y;
	p_mat[2][2] = p_scl->z;
}

void mat44_translate(mat44_t p_mat, const vec3_t *p_pos)
{
	p_mat[3][0] = p_pos->x;
	p_mat[3][1] = p_pos->y;
	p_mat[3][2] = p_pos->z;
}

void mat44_rotate_x(mat44_t p_mat, float angle)
{
	float cos_phi = cosf(angle);
	float sin_phi = sinf(angle);
	p_mat[1][1] = cos_phi;
	p_mat[1][2] = -sin_phi;
	p_mat[2][1] = sin_phi;
	p_mat[2][2] = cos_phi;
}

void mat44_rotate_y(mat44_t p_mat, float angle)
{
	float cos_phi = cosf(angle);
	float sin_phi = sinf(angle);
	p_mat[0][0] = cos_phi;
	p_mat[0][3] = sin_phi;
	p_mat[2][0] = -sin_phi;
	p_mat[2][3] = cos_phi;
}

void mat44_rotate_z(mat44_t p_mat, float angle)
{
	float cos_phi = cosf(angle);
	float sin_phi = sinf(angle);
	p_mat[0][0] = cos_phi;
	p_mat[0][1] = -sin_phi;
	p_mat[1][0] = sin_phi;
	p_mat[1][1] = cos_phi;
}

void mat44_rotate(mat44_t p_mat, const vec3_t *p_rot)
{
	mat44_t rot;	
	mat44_ident(rot);
	float sx = sin(p_rot->x);
	float cx = cos(p_rot->x);
	float sy = sin(p_rot->y);
	float cy = cos(p_rot->y);
	float sz = sin(p_rot->z);
	float cz = cos(p_rot->z);

	rot[0][0] = cy * cz;
	rot[0][1] = -cy * sz;
	rot[0][2] = sy;
	rot[1][0] = sx * sy * cz + cx * sz;
	rot[1][1] = -sx * sy * sz + cx * cz;
	rot[1][2] = -sx * cy;
	rot[2][0] = -cx * sy * cz + sx * sz;
	rot[2][1] = cx * sy * sz + sx * cz;
	rot[2][2] = cx * cy;
	mat44_mul(p_mat, p_mat, rot);
}
