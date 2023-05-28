#pragma comment(lib, "glut32.lib")
#include <stdio.h>
#include "glm/glm.hpp"
#include "GL/glut.h"
//#include "rabdata.h"

using namespace glm;

const float sensitivity = 0.1f;
vec2 rotation = { 0.f, 0.f };

struct vec2i
{
public:
	vec2i() : x(0), y(0) {}
	vec2i(int X, int Y) : x(X), y(Y) {}

	int x, y;
};

float dist = -40.f;
int prevMouseY = 0;
vec2i prevMousePosRotation;
bool isPressedLeft = false;
bool isPressedRight = false;
vec2i screenSize;

// Положение камеры
float camX = 0.0f;
float camY = 0.0f;
float camZ = 5.0f;

// Углы поворота камеры
float camYaw = 0.0f;
float camPitch = 0.0f;

GLfloat houseVertices[] = {
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f,  0.5f, 0.5f,
	-0.5f,  0.5f, 0.5f,

	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,

	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, 0.5f,

	-0.5f, -0.5f, 0.5f,
	-0.5f,  0.5f, 0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, 0.5f,
	0.5f,  0.5f, 0.5f,
	0.0f,  0.8f, 0.0f,

	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);

	glViewport(0, 0, screenSize.x, screenSize.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, screenSize.x / (double)screenSize.y, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();

	//gluLookAt()

	

	glVertexPointer(3, GL_FLOAT, 0, houseVertices);
	glDrawElements(GL_TRIANGLES, sizeof(houseIndices) / sizeof(houseIndices[0]), GL_UNSIGNED_INT, houseIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glutSwapBuffers();
}

void mouseMotion(int x, int y)
{
	if (isPressedRight && prevMouseY != y)
	{
		dist += (y - prevMouseY) * sensitivity;
		prevMouseY = y;
		printf("Mouse y: %d  PrevMouseY: %d  Dist: %f\n", y, prevMouseY, dist);
	}
	glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y)
{
	prevMouseY = y;
	isPressedLeft = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
	isPressedRight = (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN);
	glutPostRedisplay();
}

void windowResize(int width, int height)
{
	screenSize = vec2i(width, height);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL Demo Application");

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutDisplayFunc(display);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseClick);
	glutReshapeFunc(windowResize);
	glutMainLoop();
	return 0;
}