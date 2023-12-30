#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "cmathlib.h"
#include <stdio.h>

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

int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("conout$", "w", stdout);
	freopen("conin$", "w", stdin);

	mat33_t mat1 = {
		{ 2.f, 1.f, 1.f },
		{ 2.f, 1.f, 1.f },
		{ 2.f, 1.f, 1.f }
	};

	mat33_t mat2 = {
		{ 2.f, 2.f, 2.f },
		{ 1.f, 1.f, 1.f },
		{ 1.f, 1.f, 1.f }
	};

	mat33_t result;
	mat33_ident(result);
	result[0][2] = 10.f;

	//mat33_mul(result, mat1, mat2);
	mat_print((const float *)result, 3, 3);


	while (1);
    return 0;
}