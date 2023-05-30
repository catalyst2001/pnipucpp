#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

HINSTANCE g_instance;
HWND h_wnd;

glm::vec2 angles = glm::vec2(/*-96.f, 89.f*/);
POINT curr_cursor, last_cursor;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
RECT rect, oldrect;
glm::mat4x4 view;
glm::mat4x4 projection;
float dist = -10.f;

glm::vec3 houseVertices[] = {
	glm::vec3(-0.5f, -0.5f, 0.5f),
	glm::vec3(0.5f, -0.5f, 0.5f),
	glm::vec3(0.5f,  0.5f, 0.5f),
	glm::vec3(-0.5f,  0.5f, 0.5f),

	glm::vec3(-0.5f, -0.5f, -0.5f),
	glm::vec3(-0.5f,  0.5f, -0.5f),
	glm::vec3(0.5f,  0.5f, -0.5f),
	glm::vec3(0.5f, -0.5f, -0.5f),

	glm::vec3(0.5f, -0.5f, 0.5f),
	glm::vec3(0.5f, -0.5f, -0.5f),
	glm::vec3(0.5f,  0.5f, -0.5f),
	glm::vec3(0.5f,  0.5f, 0.5f),

	glm::vec3(-0.5f, -0.5f, 0.5f),
	glm::vec3(-0.5f,  0.5f, 0.5f),
	glm::vec3(-0.5f,  0.5f, -0.5f),
	glm::vec3(-0.5f, -0.5f, -0.5f),

	glm::vec3(-0.5f,  0.5f, 0.5f),
	glm::vec3(0.5f,  0.5f, 0.5f),
	glm::vec3(0.0f,  0.8f, 0.0f),

	glm::vec3(-0.5f, -0.5f, 0.5f),
	glm::vec3(0.5f, -0.5f, 0.5f),
	glm::vec3(0.5f, -0.5f, -0.5f),
	glm::vec3(-0.5f, -0.5f, -0.5f),
};

int houseIndices[] = {
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	10, 11, 8,

	12, 13, 14,
	14, 15, 12,

	16, 17, 18,

	19, 20, 21,
	21, 22, 19,
};

#define H 1

glm::vec3 house[][2] = {
	//bottom
	{ glm::vec3(-1, 0, 1), glm::vec3(-1, 0, -1) },
	{ glm::vec3(1, 0, 1), glm::vec3(1, 0, -1) },
	{ glm::vec3(-1, 0, 1), glm::vec3(1, 0, 1) },
	{ glm::vec3(-1, 0, -1), glm::vec3(1, 0, -1) },

	//left
	{ glm::vec3(-1, 0, 1), glm::vec3(-1, 1, 1) },
	{ glm::vec3(-1, 0, -1), glm::vec3(-1, 1, -1) },
	{ glm::vec3(-1, 0, 1), glm::vec3(-1, 0, -1) },
	{ glm::vec3(-1, 1, 1), glm::vec3(-1, 1, -1) },

	// near
	{ glm::vec3(-1, 0, -1), glm::vec3(-1, 1, -1) },
	{ glm::vec3(1, 0, -1), glm::vec3(1, 1, -1) },
	{ glm::vec3(-1, 0, -1), glm::vec3(1, 0, -1) }, //? glm::vec3(1, 0, 1)
	{ glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1) },

	// right
	{ glm::vec3(1, 0, -1), glm::vec3(1, 1, -1) },
	{ glm::vec3(1, 0, 1), glm::vec3(1, 1, 1) },
	{ glm::vec3(1, 0, -1), glm::vec3(1, 0, 1) },
	{ glm::vec3(1, 1, -1), glm::vec3(1, 1, 1) },

	// far
	{ glm::vec3(1, 0, 1), glm::vec3(1, 1, 1) },
	{ glm::vec3(-1, 0, 1), glm::vec3(-1, 1, 1) },
	{ glm::vec3(1, 0, 1), glm::vec3(-1, 0, 1) },
	{ glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1) },

	// top tri1
	{ glm::vec3(-1, 1, -1), glm::vec3(0, 1.5f, -1) },
	{ glm::vec3(0, 1.5f, -1), glm::vec3(1, 1, -1) },
	{ glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1) },

	// top tri2
	{ glm::vec3(-1, 1, 1), glm::vec3(0, 1.5f, 1) },
	{ glm::vec3(0, 1.5f, 1), glm::vec3(1, 1, 1) },
	{ glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1) },

	// top left
	{ glm::vec3(1, 1, -1), glm::vec3(1, 1, 1) }, //ERROR
	{ glm::vec3(0, 1.5f, -1), glm::vec3(0, 1.5f, 1) },
	{ glm::vec3(-1, 1, -1), glm::vec3(0, 1.5f, -1) }, //28
	{ glm::vec3(-1, 1, 1), glm::vec3(0, 1.5f, 1) }, //29

	// top right
	{ glm::vec3(1, 1, -1), glm::vec3(1, 1, 1) },
	{ glm::vec3(0, 1.5f, -1), glm::vec3(0, 1.5f, 1) },
	{ glm::vec3(1, 1, -1), glm::vec3(0, 1.5f, -1) }, //32
	{ glm::vec3(1, 1, 1), glm::vec3(0, 1.5f, 1) } //33
};
const int num_lines = sizeof(house) / sizeof(house[0]);

glm::vec3 projected_verts[num_lines][2];

void draw_triangle_unfilled(HDC hdc, int x0, int y0, int x1, int y1, int x2, int y2)
{
	POINT pt;
	MoveToEx(hdc, x0, y0, &pt);
	LineTo(hdc, x1, y1);
	LineTo(hdc, x2, y2);
	LineTo(hdc, x0, y0);
	MoveToEx(hdc, pt.x, pt.y, &pt);
}

void draw_triangles_with_indices(HDC hdc, glm::vec3 *p_coords, int *p_indices, size_t indices_count)
{
	for (size_t i = 0; i < indices_count; i += 3) {

		// draw each triangle
		glm::vec3 prepared_verts[3];
		int x0, y0, x1, y1, x2, y2;
		float half_screen_width = rect.right / 2.f;
		float half_screen_height = rect.bottom / 2.f;
		for (size_t j = 0; j < 3; j++) {
			glm::vec4 vec = glm::vec4(
				p_coords[p_indices[i + j]].x,
				p_coords[p_indices[i + j]].y,
				p_coords[p_indices[i + j]].z,
				1.f
			);

			glm::vec4 vec_projected = view * vec * projection;
			if (vec_projected.w != 0.f) {
				vec_projected.x /= vec_projected.w;
				vec_projected.y /= vec_projected.w;
				vec_projected.z /= vec_projected.w;
			}
			prepared_verts[j] = glm::vec3(vec_projected.x, vec_projected.y, vec_projected.z);

			prepared_verts[j].x *= half_screen_width;
			prepared_verts[j].y *= -half_screen_height;

			prepared_verts[j].x += half_screen_width;
			prepared_verts[j].y += half_screen_height;
		}

		x0 = (int)prepared_verts[0].x;
		y0 = (int)prepared_verts[0].y;
		x1 = (int)prepared_verts[1].x;
		y1 = (int)prepared_verts[1].y;
		x2 = (int)prepared_verts[2].x;
		y2 = (int)prepared_verts[2].y;
		draw_triangle_unfilled(hdc, x0, y0, x1, y1, x2, y2);
		//Sleep(1000);
	}
}

glm::mat4x4 mat_identity = glm::mat4x4(
	glm::vec4(1.f, 0.f, 0.f, 0.f),
	glm::vec4(0.f, 1.f, 0.f, 0.f),
	glm::vec4(0.f, 0.f, 1.f, 0.f),
	glm::vec4(0.f, 0.f, 0.f, 1.f)
);

HPEN pen_red;
HPEN pen_blue;

void rotate_model()
{
	for (size_t i = 0; i < sizeof(house) / sizeof(house[0]); i++) {
		for (size_t j = 0; j < 2; j++) {
			glm::vec4 vec = glm::vec4(house[i][j].x, house[i][j].y, house[i][j].z, 1.f);
			glm::mat4x4 mat = glm::rotate(mat_identity, -90.f, glm::vec3(1.f, 0.f, 0.f));
			mat = glm::rotate(mat, 89.f, glm::vec3(0.f, 1.f, 0.f));
			vec = mat * vec;
			house[i][j] = glm::vec3(vec.x, vec.y, vec.z);
		}
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("conout$", "w", stdout);
	printf("OK\n");

	WNDCLASSEXA wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
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
	h_wnd = CreateWindowExA(0, wcex.lpszClassName, "lab3", WS_OVERLAPPEDWINDOW, xpos, ypos, width, height, NULL, NULL, hInstance, NULL);
	if (!h_wnd) {
		MessageBoxA(HWND_DESKTOP, "Couldn't create window", "Critical error", MB_OK | MB_ICONERROR);
		return 2;
	}

	GetClientRect(h_wnd, &rect);
	projection = glm::perspective(45.f, rect.right / (float)rect.bottom, 0.0001f, 1000.f);

	view = glm::translate(mat_identity, glm::vec3(0, 0, dist));
	view = glm::rotate(view, 45.f, glm::vec3(1.f, 1.f, 0.f));

	pen_red = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	pen_blue = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));

	//rotate_model();

	ShowWindow(h_wnd, SW_SHOW);
	UpdateWindow(h_wnd);

	MSG msg;
	while (GetMessageA(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
    return 0;
}

void clear_color_buffer(HDC hdc)
{
	FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

void draw_line(HDC hdc, int x0, int y0, int x1, int y1)
{
	POINT pt;
	MoveToEx(hdc, x0, y0, &pt);
	LineTo(hdc, x1, y1);
	MoveToEx(hdc, pt.x, pt.y, &pt);
}

inline glm::vec3 project_vertex(glm::vec3 &vert_in)
{
	float half_screen_width = rect.right / 2.f;
	float half_screen_height = rect.bottom / 2.f;
	glm::vec4 vec = glm::vec4(vert_in.x, vert_in.y, vert_in.z, 1.f);
	glm::vec4 vec_projected = view * vec * projection;
	if (vec_projected.w != 0.f) {
		vec_projected.x /= vec_projected.z;
		vec_projected.y /= vec_projected.z;
		vec_projected.z /= vec_projected.z;
	}
	
	return glm::vec3(
		(vec_projected.x * half_screen_width) + half_screen_width,
		(vec_projected.y * half_screen_height) + half_screen_height,
		vec_projected.z
	);
}

inline glm::vec3 perspective_trimetric_project(glm::vec3 &in, glm::vec3 &begin, float theta, float phi, float pz)
{
	glm::vec4 in_col = glm::vec4(in.x, in.y, in.z, 1.f);
	glm::mat4x4 mat = mat_identity;
	theta = glm::radians(theta);
	phi = glm::radians(phi);
	mat = glm::rotate(mat, theta, glm::vec3(0.f, 1.f, 0.f)); //Ry
	mat = glm::rotate(mat, phi, glm::vec3(1.f, 0.f, 0.f)); //Rx
	mat = mat * glm::mat4x4(
		glm::vec4(1.f, 0.f, 0.f, 0.f),
		glm::vec4(0.f, 1.f, 0.f, 0.f),
		glm::vec4(0.f, 0.f, -(1.f / pz), 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f)
	);

	glm::vec4 out = glm::translate(mat_identity, begin) * in_col * mat;
	if (out.w != 0.f) {
		out.x /= out.w;
		out.y /= out.w;
		out.z /= out.w;
	}

	float scale = 0.5;
	float half_screen_width = rect.right / 2.f;
	float half_screen_height = rect.bottom / 2.f;
	return glm::vec3(
		(out.x * half_screen_width) + half_screen_width,
		(out.y * -half_screen_height) + half_screen_height,
		out.z
	);
}

glm::vec3 perspective_trimetric_project2(glm::vec3 in, glm::vec3 &begin, float _theta, float _phi, float _rho) {
	float rez;
	float deg2rad = atanf(1.f) / 45.f;

	float th = _theta * deg2rad;
	float ph = (_phi + 90) * deg2rad;
	float costh = cosf(th);
	float sinth = sinf(th);
	float cosph = cosf(ph);
	float sinph = sinf(ph);

	float v11 = -sinth;
	float v12 = -cosph * costh;
	float v13 = -sinph * costh;
	float v21 = costh;
	float v22 = -cosph * sinth;
	float v23 = -sinph * sinth;
	float v32 = sinph;
	float v33 = -cosph;
	float v43 = _rho;

	float xe = v11 * in.x + v21 * in.y;
	float ye = v12 * in.x + v22 * in.y + v32 * in.z;
	float ze = v13 * in.x + v23 * in.y + v33 * in.z + v43;

	float half_screen_width = rect.right / 2.f;
	float half_screen_height = rect.bottom / 2.f;
	float pX = 45 * _rho * xe / ze + half_screen_width - 2 * begin.y;
	float pY = 45 * _rho * ye / ze + half_screen_height - 2 * begin.z;
	return glm::vec3(pX, pY, 1.f);
}

bool cross_lines(glm::vec2 &dst, glm::vec2 &line1b, glm::vec2 &line1e, glm::vec2 &line2b, glm::vec2 &line2e) {
	float n;
	if (line1e.y - line1b.y != 0) {  // a(y)
		float q = (line1e.x - line1b.x) / (line1b.y - line1e.y);
		float sn = (line2b.x - line2e.x) + (line2b.y - line2e.y) * q;
		if (!sn)
			return false;  // c(x) + c(y)*q

		float fn = (line2b.x - line1b.x) + (line2b.y - line1b.y) * q;   // b(x) + b(y)*q
		n = fn / sn;
	}
	else {
		if (!(line2b.y - line2e.y))
			return false; // b(y)

		n = (line2b.y - line1b.y) / (line2b.y - line2e.y);   // c(y)/b(y)
	}
	//idx0 = line2b.x + (-b(x))*n
	//idline1b.x = line2b.y +(-b(y))*n

	dst.x = line2b.x + (line2e.x - line2b.x) * n;
	dst.y = line2b.y + (line2e.y - line2b.y) * n;
	return true;
}

void text_print(HDC hdc, int x, int y, const char *p_format, ...)
{
	va_list argptr;
	char buffer[1024];
	va_start(argptr, p_format);
	vsprintf_s(buffer, sizeof(buffer), p_format, argptr);
	va_end(argptr);

	TextOutA(hdc, x, y, buffer, strlen(buffer));
}

//#define MATRIX_PRINT

void paint_scene(HDC hdc, HWND hwnd)
{
	clear_color_buffer(hdc);

	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 begin(0.f, 0.f, 0.f);
	HGDIOBJ oldpen = SelectObject(hdc, pen_red);

#ifdef MATRIX_PRINT
	printf("--- src ---\n\\begin{bmatrix}\n");
#endif

	for (int i = 0; i < num_lines; i++) {
		v0 = perspective_trimetric_project2(house[i][0], begin, angles.y, angles.x, dist);
		v1 = perspective_trimetric_project2(house[i][1], begin, angles.y, angles.x, dist);

#ifdef MATRIX_PRINT
		printf(
			"%.1f && %.1f && %.1f && 1.0\\\\\n"
			"%.1f && %.1f && %.1f && 1.0\\\\\n",
			house[i][0].x, house[i][0].y, house[i][0].z,
			house[i][1].x, house[i][1].y, house[i][1].z
		);
#endif

		projected_verts[i][0] = v0;
		projected_verts[i][1] = v1;
		draw_line(hdc, v0.x, v0.y, v1.x, v1.y);
	}
#ifdef MATRIX_PRINT
	printf("\\end{bmatrix}\n");


	printf(" --- transformated ---\n\\begin{bmatrix}\n");
	for (size_t i = 0; i < num_lines; i++) {
		printf(
			"%.2f && %.2f && %.2f\\\\\n"
			"%.2f && %.2f && %.2f\\\\\n",
			projected_verts[i][0].x, projected_verts[i][0].y, projected_verts[i][0].z,
			projected_verts[i][1].x, projected_verts[i][1].y, projected_verts[i][1].z
		);
	}
	printf("\\end{bmatrix}\n");
#endif
	SelectObject(hdc, pen_blue);


	/* Z */
	glm::vec2 endpos;
	int vanishing_points = 0;
	glm::vec2 line1b = glm::vec2(projected_verts[0][0].x, projected_verts[0][0].y);
	glm::vec2 line1e = glm::vec2(projected_verts[0][1].x, projected_verts[0][1].y);
	glm::vec2 line2b = glm::vec2(projected_verts[1][0].x, projected_verts[1][0].y);
	glm::vec2 line2e = glm::vec2(projected_verts[1][1].x, projected_verts[1][1].y);
	if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
		draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
		draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
		text_print(hdc, endpos.x, endpos.y, "Z (%d %d)", (int)endpos.x, (int)endpos.y);
		vanishing_points++;
	}


	/* Y */
	line1b = glm::vec2(projected_verts[16][0].x, projected_verts[16][0].y);
	line1e = glm::vec2(projected_verts[16][1].x, projected_verts[16][1].y);
	line2b = glm::vec2(projected_verts[17][0].x, projected_verts[17][0].y);
	line2e = glm::vec2(projected_verts[17][1].x, projected_verts[17][1].y);
	if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
		draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
		draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
		text_print(hdc, endpos.x, endpos.y, "Y (%d %d)", (int)endpos.x, (int)endpos.y);
		vanishing_points++;
	}

	/* X */
	line1b = glm::vec2(projected_verts[2][0].x, projected_verts[2][0].y);
	line1e = glm::vec2(projected_verts[2][1].x, projected_verts[2][1].y);
	line2b = glm::vec2(projected_verts[3][0].x, projected_verts[3][0].y);
	line2e = glm::vec2(projected_verts[3][1].x, projected_verts[3][1].y);
	if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
		draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
		draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
		text_print(hdc, endpos.x, endpos.y, "X (%d %d)", (int)endpos.x, (int)endpos.y);
		vanishing_points++;
	}


	/* crysha left */
	line1b = glm::vec2(projected_verts[28][0].x, projected_verts[28][0].y);
	line1e = glm::vec2(projected_verts[28][1].x, projected_verts[28][1].y);
	line2b = glm::vec2(projected_verts[29][0].x, projected_verts[29][0].y);
	line2e = glm::vec2(projected_verts[29][1].x, projected_verts[29][1].y);
	if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
		draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
		draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
		text_print(hdc, endpos.x, endpos.y, "(%d %d)", (int)endpos.x, (int)endpos.y);
		vanishing_points++;
	}

	/* crysha left */
	line1b = glm::vec2(projected_verts[32][0].x, projected_verts[32][0].y);
	line1e = glm::vec2(projected_verts[32][1].x, projected_verts[32][1].y);
	line2b = glm::vec2(projected_verts[33][0].x, projected_verts[33][0].y);
	line2e = glm::vec2(projected_verts[33][1].x, projected_verts[33][1].y);
	if (cross_lines(endpos, line1b, line1e, line2b, line2e)) {
		draw_line(hdc, line1b.x, line1b.y, endpos.x, endpos.y);
		draw_line(hdc, line2b.x, line2b.y, endpos.x, endpos.y);
		text_print(hdc, endpos.x, endpos.y, "(%d %d)", (int)endpos.x, (int)endpos.y);
		vanishing_points++;
	}

	/* scene distortion factor */

	glm::vec3 distortion_factor;
	distortion_factor.x = glm::length(glm::vec2(projected_verts[2][1].x, projected_verts[2][1].y) - glm::vec2(projected_verts[2][0].x, projected_verts[2][0].y)) / glm::length(glm::vec2(house[2][1].x, house[2][1].y) - glm::vec2(house[2][0].x, house[2][0].y));
	distortion_factor.y = glm::length(glm::vec2(projected_verts[16][1].x, projected_verts[16][1].y) - glm::vec2(projected_verts[16][0].x, projected_verts[16][0].y)) / glm::length(glm::vec2(house[16][1].x, house[16][1].y) - glm::vec2(house[16][0].x, house[16][0].y));
	distortion_factor.z = glm::length(glm::vec2(projected_verts[0][1].x, projected_verts[0][1].y) - glm::vec2(projected_verts[0][0].x, projected_verts[0][0].y)) / (glm::length(glm::vec2(house[0][1].x, house[0][1].y) - glm::vec2(house[0][0].x, house[0][0].y)) + 1.f);

	text_print(hdc, 10, 10, "vanishing points count %d", vanishing_points);
	text_print(hdc, 10, 30, "distortion  x: %f y: %f z: %f", distortion_factor.x, distortion_factor.y, distortion_factor.z);
	text_print(hdc, 10, 50, "scene distortion: %f", sqrtf(powf(distortion_factor.x, 2.f) + powf(distortion_factor.y, 2.f) + powf(distortion_factor.z, 2.f)));
	
	text_print(hdc, 10, 80, "theta %f", angles.y);
	text_print(hdc, 10, 100, "phi %f", angles.x);
	text_print(hdc, 10, 120, "z %f", dist);

	SelectObject(hdc, oldpen);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool buttons[1024];
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case 1:
				break;

            default:
                return DefWindowProcA(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_SIZE:
		//GetClientRect(hWnd, &rect);
		//if (rect.bottom == 0)
		//	rect.bottom = 1;

		//projection = glm::perspective(45.f, rect.right / (float)rect.bottom, 0.0001f, 1000.f);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_MOUSEWHEEL: {
		SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta < 0)
			dist -= 0.5f;
		else dist += 0.5f;

		//view = glm::translate(mat_identity, glm::vec3(0, 0, dist));
		//view = glm::rotate(view, glm::radians(angles.x), glm::vec3(1, 0, 0));
		//view = glm::rotate(view, glm::radians(angles.y), glm::vec3(0, 1, 0));
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_PAINT: {
		PAINTSTRUCT ps;
		GetClientRect(h_wnd, &rect);
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC cdc = CreateCompatibleDC(hdc);
		HBITMAP h_bitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		SelectObject(cdc, h_bitmap);
		SetBkMode(cdc, TRANSPARENT);
		paint_scene(cdc, hWnd);
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, cdc, 0, 0, SRCCOPY);
		DeleteObject(h_bitmap);
		DeleteDC(cdc);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_MOUSEMOVE:
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			printf("MOUSEMOVE\n");

			curr_cursor.x = LOWORD(lParam);
			curr_cursor.y = HIWORD(lParam);

			POINT diffpt;
			diffpt.y = curr_cursor.x - last_cursor.x;
			diffpt.x = curr_cursor.y - last_cursor.y;
			if (diffpt.x || diffpt.y) {
				angles.x += diffpt.x * -0.1f;
				angles.y += diffpt.y * 0.1f;
				printf("angle %f %f\n", angles.x, angles.y);			
			}
			last_cursor = curr_cursor;

			// setup matrix
			//view = glm::translate(mat_identity, glm::vec3(0, 0, dist));
			//view = glm::rotate(view, glm::radians(angles.x), glm::vec3(1, 0, 0));
			//view = glm::rotate(view, glm::radians(angles.y), glm::vec3(0, 1, 0));
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
		curr_cursor.x = LOWORD(lParam);
		curr_cursor.y = HIWORD(lParam);
		last_cursor = curr_cursor;
		break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	case WM_KEYDOWN:
		buttons[wParam] = true;
		break;

	case WM_KEYUP:
		buttons[wParam] = false;
		break;

	case WM_ERASEBKGND:
		return 1;

    default:
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    return 0;
}