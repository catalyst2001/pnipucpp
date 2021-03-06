#include <stdio.h>
#include "gl_shapes.h"
#include "glut/glut.h"
#include <gl/GLU.h>
#include <limits>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")

class vec2 {
public:
	vec2() {}
	~vec2() {}
	vec2(int xx, int yy) : x(xx), y(yy) {}
	int x, y;
};

class bbox {
public:
	bbox() {}
	~bbox() {}
	bbox(int xmin, int ymin, int xmax, int ymax) : vmin(vec2(xmin, ymin)), vmax(vec2(xmax, ymax)) {}
	bool pt_in_bbox(vec2 &pt) { return (vmin.x < pt.x && vmin.y < pt.y && vmax.x > pt.x && vmax.y > pt.y); }

	vec2 vmin;
	vec2 vmax;
};

class node {
	double value;
	node *p_parent_node;
	node *p_left_node;
	node *p_right_node;
public:
	node() {}
	~node() {}

	int level;
	vec2 pos; //для расчетов визуализации

	void set_value(double val) { value = val; }
	double get_value() { return value; }
	node *get_left_node() { return p_left_node; }
	void set_left_node(node *p_leftnode) {
		p_left_node = p_leftnode;
		p_left_node->set_level(level + 1);
	}
	node *get_right_node() { return p_right_node; }

	void set_right_node(node *p_rightnode) {
		p_right_node = p_rightnode;
		p_left_node->set_level(level + 1);
	}
	node *get_parent_node() { return p_parent_node; }
	void set_parent_node(node *p_parent) { p_parent_node = p_parent; }
	void set_level(int l) { level = l; }
	int get_level() { return level; }
};

#define NEW_NODE() ((node *)calloc(1, sizeof(node)))
inline node *new_node(node *p_parent, double value)
{
	node *p_node = NEW_NODE();
	p_node->set_value(value);
	p_node->set_parent_node(p_parent);
	if(p_parent)
		p_node->set_level(p_parent->get_level() + 1);

	return p_node;
}

#define NODE_CIRCLE_RADUIS 30

int width = 600;
int height = 600;
node *p_root_node;
double tree_min_value = DBL_MAX;
vec2 mouse;

double get_random_float(double min_val, double max_val)
{
	srand((unsigned int)__rdtsc());
	return (double)(rand() % (int)(max_val - min_val + 1.)) + min_val;
}

#define MIN_VAL 1.
#define MAX_VAL 100.

//
// build_tree
// 
// построить дерево, возвращаемое значение - адрес корня дерева
//
node *build_tree()
{
	node *p_root_node = new_node(nullptr, get_random_float(MIN_VAL, MAX_VAL));
	p_root_node->set_level(1);
	
	node *p_branch1 = new_node(p_root_node, get_random_float(MIN_VAL, MAX_VAL));
	node *p_sub1branch1 = new_node(p_branch1, get_random_float(MIN_VAL, MAX_VAL));
	p_sub1branch1->set_left_node(new_node(p_sub1branch1, get_random_float(MIN_VAL, MAX_VAL)));
	p_sub1branch1->set_right_node(new_node(p_sub1branch1, get_random_float(MIN_VAL, MAX_VAL)));
	p_branch1->set_left_node(p_sub1branch1);

	node *p_sub2branch1 = new_node(p_branch1, get_random_float(MIN_VAL, MAX_VAL));
	p_sub2branch1->set_left_node(new_node(p_sub2branch1, get_random_float(MIN_VAL, MAX_VAL)));
	p_sub2branch1->set_right_node(new_node(p_sub2branch1, get_random_float(MIN_VAL, MAX_VAL)));
	p_branch1->set_right_node(p_sub2branch1);
	p_root_node->set_left_node(p_branch1);

	node *p_branch2 = new_node(p_root_node, get_random_float(MIN_VAL, MAX_VAL));
	node *p_sub1branch2 = new_node(p_branch2, get_random_float(MIN_VAL, MAX_VAL));
	p_sub1branch2->set_left_node(new_node(p_sub1branch2, get_random_float(MIN_VAL, MAX_VAL)));
	p_sub1branch2->set_right_node(new_node(p_sub1branch2, get_random_float(MIN_VAL, MAX_VAL)));
	p_branch2->set_left_node(p_sub1branch2);

	node *p_sub2branch2 = new_node(p_branch2, get_random_float(MIN_VAL, MAX_VAL));
	p_sub2branch2->set_left_node(new_node(p_sub2branch2, get_random_float(MIN_VAL, MAX_VAL)));
	p_sub2branch2->set_right_node(new_node(p_sub2branch2, get_random_float(MIN_VAL, MAX_VAL)));
	p_root_node->set_right_node(p_branch2);
	p_branch2->set_right_node(p_sub2branch2);

	return p_root_node;
}

int text_width(void *p_font, const char *p_string)
{
	int w = 0;
	while (*p_string) {
		w += glutBitmapWidth(p_font, *p_string);
		p_string++;
	}
	return w;
}

void draw_string(void *p_font, int x, int y, const char *p_format, ...)
{
	char string[512];
	va_list argptr;
	va_start(argptr, p_format);
	vsprintf_s(string, sizeof(string), p_format, argptr);
	va_end(argptr);
	for (int i = 0; string[i]; i++) {
		glRasterPos2i(x, y);
		glutBitmapCharacter(p_font, string[i]);
		x += glutBitmapWidth(p_font, string[i]);
	}
}

// отображает форматированную строку
void draw_string_centred(void *p_font, int x, int y, const char *p_format, ...)
{
	char string[512];
	va_list argptr;
	va_start(argptr, p_format);
	vsprintf_s(string, sizeof(string), p_format, argptr);
	va_end(argptr);

	x -= (text_width(p_font, string) / 4);
	y += 8;
	draw_string(p_font, x, y, string);
}

int node_hovered(node *p_node, const vec2 &pt)
{
	return ((p_node->pos.x - pt.x) * (p_node->pos.x - pt.x) + (p_node->pos.y - pt.y) * (p_node->pos.y - pt.y)) <= NODE_CIRCLE_RADUIS * NODE_CIRCLE_RADUIS;
}

//
// draw_node
// 
// выполняет отрисовку ноды. 
// если нодой является корень дерева, то все дерево будет отрисовано целиком 
//
void draw_node(node *p_node, bool dobuleDraw = false)
{
	if (!p_node)
		return;

	// получаем родителя
	node *p_parent = p_node->get_parent_node();

	//рисуем линию только в случае если текущая нода не является родительской
	//(т.к нет позиции начала линии - потому что нет родителя)
	if (p_parent && !dobuleDraw) {
		draw_line(p_parent->pos.x, p_parent->pos.y, p_node->pos.x, p_node->pos.y);
		draw_node(p_parent, true);
	}
	//поверх линии рисуем круг ноды
	glPushAttrib(GL_CURRENT_BIT);
	glColor3ub(255, 255, 255);
	draw_circle(p_node->pos.x, p_node->pos.y, FILLED, 80, NODE_CIRCLE_RADUIS);

	static unsigned char colors[][3] = {
		{0, 0, 0},
		{0, 0, 255}
	};
	glColor3ubv(colors[node_hovered(p_node, mouse)]);
	draw_circle(p_node->pos.x, p_node->pos.y, UNFILLED, 80, NODE_CIRCLE_RADUIS);
	draw_string_centred(GLUT_BITMAP_TIMES_ROMAN_24, p_node->pos.x - 10, p_node->pos.y, "%.1lf", p_node->get_value());
	glPopAttrib();

	if (!dobuleDraw) {
		// идем рисовать левую ветвь
		draw_node(p_node->get_left_node());

		// затем рисуем правую
		draw_node(p_node->get_right_node());
	}
}

// 
// tree_height
// 
// Вычисляет и возвращает высоту дерева
// 
int tree_height(node *p_node)
{
	if (!p_node)
		return 0;

	int left_height = tree_height(p_node->get_left_node());
	int right_height = tree_height(p_node->get_right_node());
	return max(left_height, right_height) + 1;
}

enum node_position {
	ROOT_NODE = 0,
	LEFT_NODE,
	RIGHT_NODE
};

// расчитывает позиции нод исходя их предыдущих делений отступов по высоте и ширине
void process_tree_node(node *p_node, int w, node_position position)
{
	if (!p_node)
		return;

	int div = (int)pow(2, (double)p_node->get_level());
	int paddingx = w / div;
	node *p_parent = p_node->get_parent_node();
	switch (position) {
	case ROOT_NODE:
		p_node->pos.x = paddingx;
		p_node->pos.y = (p_node->get_level() * NODE_CIRCLE_RADUIS * 2) + NODE_CIRCLE_RADUIS / 2;
		break;

	case LEFT_NODE:
		p_node->pos.x = p_parent->pos.x - paddingx;
		p_node->pos.y = (p_node->get_level() * NODE_CIRCLE_RADUIS * 2) + NODE_CIRCLE_RADUIS / 2;
		break;

	case RIGHT_NODE:
		p_node->pos.x = p_parent->pos.x + paddingx;
		p_node->pos.y = (p_node->get_level() * NODE_CIRCLE_RADUIS * 2) + NODE_CIRCLE_RADUIS / 2;
		break;

	default:
		printf("process_tree_node() -> unknown type\n");
	}
	//printf("x: %d y: %d  node: %d  (%s)  level: %d\n", p_node->pos.x, p_node->pos.y, position, !p_node->get_parent_node() ? "Root" : "Child", p_node->get_level());
	process_tree_node(p_node->get_left_node(), w, LEFT_NODE);
	process_tree_node(p_node->get_right_node(), w, RIGHT_NODE);
}

void tree_min_val(double *p_minval, node *p_node)
{
	if (!p_node)
		return;

	if (*p_minval > p_node->get_value())
		*p_minval = p_node->get_value();

	tree_min_val(p_minval, p_node->get_left_node());
	tree_min_val(p_minval, p_node->get_right_node());
}

void resize_window(int iwidth, int iheight)
{
	width = iwidth;
	height = iheight;
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLfloat)width, (GLfloat)height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();

	process_tree_node(p_root_node, width, ROOT_NODE);
}

void update(int key, int x, int y)
{
	printf("update: %d\n", key);
	if (key == 27)
		exit(0);

	glutPostRedisplay();
}

void key_event(unsigned char key, int x, int y)
{
	printf("key_event: %d\n", key);
	update(key, x, y);
	mouse.x = x;
	mouse.y = y;
}

void idle_func()
{
	//printf("idle_func\n");
	glutPostRedisplay();
}

void draw_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //очистка буфера цвета
	glLoadIdentity(); //сброс матрицы модели вида

	glViewport(0, 0, width, height);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.f, (float)width, (float)height, 0.f);
	glColor3ub(0, 0, 0);
	draw_node(p_root_node);
	glColor3ub(0, 0, 0);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, 10, height - 20, "Tree minimum value: %.2lf", tree_min_value);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();
}

void mouse_movement(int button, int state, int x, int y)
{
	mouse.x = x;
	mouse.y = y;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutCreateWindow("Бинарные деревья");
	glutReshapeFunc(resize_window);
	glutKeyboardFunc(key_event);
	glutSpecialFunc(update);
	glutDisplayFunc(draw_scene);
	glutIdleFunc(idle_func);
	glutMouseFunc(mouse_movement);

	p_root_node = build_tree();
	process_tree_node(p_root_node, width, ROOT_NODE);
	tree_min_val(&tree_min_value, p_root_node);

	glDisable(GL_DEPTH_TEST); //отключаем буфер глубины, он нам не потребуется
	glClearColor(1.f, 1.f, 1.f, 1.f); //цвет очистки буфера цвета (белый)
	glLineWidth(1.5);

	glutMainLoop();
	return 0;
}