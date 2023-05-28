#include <Windows.h>
#include "../cgcommon/glwnd.h"
#include "../cgcommon/euler_camera.h"
#include "glm/gtc/type_ptr.hpp"

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

glm::vec3 projected_verts[sizeof(houseVertices) / sizeof(houseVertices[0])];

INT Width, Height;
const float sensitivity = 1.0f;
Camera camera(true, glm::vec3(0.f, 0.f, 0.f));
float eyes_distance = 1.1f;

void scale_model(float size)
{
	for (size_t i = 0; i < sizeof(houseVertices) / sizeof(houseVertices[0]); i++) {
		houseVertices[i] *= size;
		houseVertices[i] *= size;
		houseVertices[i] *= size;
	}
}

GLWINDOW *p_glwindow;

inline glm::vec4 vec3_to_vec4(glm::vec3 &vec) { return glm::vec4(vec.x, vec.y, vec.z, 1.f); }
inline glm::vec3 vec4_to_vec3(glm::vec4 &vec) {
	if (vec.w != 0.f) {
		vec.x /= vec.w;
		vec.y /= vec.w;
		vec.z /= vec.w;
	}
	return glm::vec3(vec.x, vec.y, vec.z);
}

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(0.f, 0.f, 0.f);

	camera.UpdateInput(p_glwindow->h_window);

	// LEFT EYE
	int half_width = p_glwindow->width >> 1;
	glViewport(0, 0, half_width, p_glwindow->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, half_width / (double)p_glwindow->height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glm::vec3 right_normalized = glm::normalize(camera.Right);
	glm::vec3 left_eye_pos = camera.Position + (right_normalized * -eyes_distance);
	glm::vec3 left_eye_dir = left_eye_pos + camera.Front;

	gluLookAt(left_eye_pos.x, left_eye_pos.y, left_eye_pos.z,
		left_eye_dir.x, left_eye_dir.y, left_eye_dir.z,
		camera.Up.x, camera.Up.y, camera.Up.z);

	glVertexPointer(3, GL_FLOAT, 0, houseVertices);
	glDrawElements(GL_TRIANGLES, sizeof(houseIndices) / sizeof(houseIndices[0]), GL_UNSIGNED_INT, houseIndices);

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glm::mat4x4 projection;
	glm::mat4x4 modelview;
	glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));
	glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelview));
	float half_height = p_glwindow->height / 2.f;
	for (size_t i = 0; i < sizeof(houseIndices) / sizeof(houseIndices[0]); i++) {
		glm::vec4 prep_vert = vec3_to_vec4(houseVertices[houseIndices[i]]);
		glm::vec4 projected_vert = modelview * prep_vert * projection;
		projected_verts[houseIndices[i]] = vec4_to_vec3(projected_vert);

		projected_verts[houseIndices[i]].x *= (float)half_width;
		projected_verts[houseIndices[i]].y *= (float)half_height;
		projected_verts[houseIndices[i]].x += (float)half_width;
		projected_verts[houseIndices[i]].y += (float)half_height;
	}

	gluOrtho2D((double)half_width, (double)p_glwindow->width, (double)p_glwindow->height, 0.0);
	glVertexPointer(3, GL_FLOAT, 0, projected_verts);
	glDrawElements(GL_TRIANGLES, sizeof(houseIndices) / sizeof(houseIndices[0]), GL_UNSIGNED_INT, houseIndices);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// RIGHT EYE
	glViewport(half_width, 0, half_width, p_glwindow->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, half_width / (double)p_glwindow->height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glm::vec3 right_eye_pos = camera.Position + (right_normalized * eyes_distance);
	glm::vec3 right_eye_dir = right_eye_pos + camera.Front;

	gluLookAt(right_eye_pos.x, right_eye_pos.y, right_eye_pos.z,
		right_eye_dir.x, right_eye_dir.y, right_eye_dir.z,
		camera.Up.x, camera.Up.y, camera.Up.z);

	//glVertexPointer(3, GL_FLOAT, 0, houseVertices);
	//glDrawElements(GL_TRIANGLES, sizeof(houseIndices) / sizeof(houseIndices[0]), GL_UNSIGNED_INT, houseIndices);
}

void fn_window_resize(HWND hWnd, int width, int height)
{
	if (!height)
		height = 1;

	Width = width;
	Height = height;
}

void fn_mousemove(HWND hWnd, int x, int y)
{
}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
}

void fn_charinput(HWND hWnd, char symbol)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void fn_keydown(HWND hWnd, INT state, WPARAM wparam, LPARAM lparam)
{
	INT key = (INT)wparam;
	if (state == KEY_DOWN) {
		switch (key) {
		case 27:
			exit(0); //close program
			break;
		}
	}
}

//Add this GL functions
void fn_windowcreate(HWND hWnd)
{
	RECT rct;
	GetClientRect(hWnd, &rct);
	glViewport(0, 0, (GLsizei)rct.right, (GLsizei)rct.bottom);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, rct.right / (double)rct.bottom, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	scale_model(2.f);
	p_glwindow = GetStruct();

	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glLineWidth(2.0);
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Lab3  Stereo Pair", __FILE__,
		24,					  //Colors bits
		32,					  //Depth bits
		fn_draw,			  //Draw function
		fn_window_resize,	  //Window resize function
		fn_mousemove,		  //Mouse move function
		fn_mouseclick,		  //Mouse click function
		fn_charinput,		  //Char input function
		fn_keydown,			  //Keydown function
		fn_windowcreate,	  //Window create function
		fn_windowclose,		  //Window close function
		0, 0,
		800,				  //Window width
		600);				  //Window height
	return 0;
}