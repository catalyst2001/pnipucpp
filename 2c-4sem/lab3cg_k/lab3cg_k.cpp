#include <Windows.h>
#include "../cgcommon/glwnd.h"
#include "../cgcommon/builtin_models.h"
#include "../cgcommon/euler_camera.h"

INT Width, Height;
const float sensitivity = 1.0f;
Camera camera(true, glm::vec3(0.f, 0.f, 0.f));
float eyes_distance = 5.f;

void scale_model(float size)
{
	for (size_t i = 0; i < sizeof(model1_verts) / sizeof(model1_verts[0]); i++) {
		model1_verts[i][0] *= size;
		model1_verts[i][1] *= size;
		model1_verts[i][2] *= size;
	}
}

GLWINDOW *p_glwindow;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	camera.UpdateInput(p_glwindow->h_window);

	// LEFT EYE
	int half_width = p_glwindow->width >> 1;
	glViewport(0, 0, half_width, p_glwindow->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, half_width / (double)p_glwindow->height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glm::vec3 left_eye_pos = camera.Position + glm::vec3(-eyes_distance, 0.f, 0.f);
	glm::vec3 left_eye_dir = left_eye_pos + camera.Front;

	gluLookAt(left_eye_pos.x, left_eye_pos.y, left_eye_pos.z,
		left_eye_dir.x, left_eye_dir.y, left_eye_dir.z,
		camera.Up.x, camera.Up.y, camera.Up.z);

	glVertexPointer(3, GL_FLOAT, 0, model1_verts);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(model1_verts) / sizeof(model1_verts[0]));

	// RIGHT EYE
	glViewport(half_width, 0, half_width, p_glwindow->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, half_width / (double)p_glwindow->height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glm::vec3 right_eye_pos = camera.Position + glm::vec3(eyes_distance, 0.f, 0.f);
	glm::vec3 right_eye_dir = right_eye_pos + camera.Front;

	gluLookAt(right_eye_pos.x, right_eye_pos.y, right_eye_pos.z,
		right_eye_dir.x, right_eye_dir.y, right_eye_dir.z,
		camera.Up.x, camera.Up.y, camera.Up.z);

	glVertexPointer(3, GL_FLOAT, 0, model1_verts);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(model1_verts) / sizeof(model1_verts[0]));
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

	scale_model(20.f);
	p_glwindow = GetStruct();

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Lab3", __FILE__,
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