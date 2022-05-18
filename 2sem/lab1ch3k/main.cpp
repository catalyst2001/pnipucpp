#include <stdio.h>
#include "gl_shapes.h"

extern "C" {
#include "gldl.h"
};

gldl_dt_t *p_window;

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
	//bbox(int x, int y, int w, int h) {
	//	vmin.x = x;
	//	vmin.y = y;
	//	vmax.x = vmin.x + w;
	//	vmax.y = vmin.x + y;
	//}

	bool pt_in_bbox(vec2 &pt) { return (vmin.x < pt.x && vmin.y < pt.y && vmax.x > pt.x && vmax.y > pt.y); }

	vec2 vmin;
	vec2 vmax;
};

class node {
	int value;
	node *p_parent_node;
	node *p_left_node;
	node *p_right_node;
public:
	node() {}
	~node() {}

	vec2 pos; //для расчетов визуализации

	void set_value(int val) { value = val; }
	int get_value() { return value; }
	node *get_left_node() { return p_left_node; }
	void set_left_node(node *p_leftnode) { p_left_node = p_leftnode; }
	node *get_right_node() { return p_right_node; }
	void set_right_node(node *p_rightnode) { p_right_node = p_rightnode; }
	node *get_parent_node() { return p_parent_node; }
	void set_parent_node(node *p_parent) { p_parent_node = p_parent; }
};

#define NEW_NODE() ((node *)calloc(1, sizeof(node)))
inline node *new_node(node *p_parent, int value)
{
	node *p_node = NEW_NODE();
	p_node->set_value(value);
	p_node->set_parent_node(p_parent);
	return p_node;
}

//
// build_tree
// 
// построить дерево, возвращаемое значение - адрес корня дерева
//
node *build_tree()
{
	node *p_root_node = new_node(nullptr, 0);
	
	node *p_branch1 = new_node(p_root_node, 1);
	node *p_sub1branch1 = new_node(p_branch1, 2);
	p_sub1branch1->set_left_node(new_node(p_sub1branch1, 3));
	p_sub1branch1->set_right_node(new_node(p_sub1branch1, 4));
	p_branch1->set_left_node(p_sub1branch1);

	node *p_sub2branch1 = new_node(nullptr, 5);
	p_sub2branch1->set_left_node(new_node(p_sub2branch1, 6));
	p_sub2branch1->set_right_node(new_node(p_sub2branch1, 7));
	p_branch1->set_right_node(p_sub2branch1);
	p_root_node->set_left_node(p_branch1);

	node *p_branch2 = new_node(p_root_node, 8);
	node *p_sub1branch2 = new_node(p_branch2, 9);
	p_sub1branch2->set_left_node(new_node(p_sub1branch2, 10));
	p_sub1branch2->set_right_node(new_node(p_sub1branch2, 11));
	p_branch2->set_left_node(p_sub1branch2);

	node *p_sub2branch2 = new_node(p_branch2, 12);
	p_sub2branch2->set_left_node(new_node(p_sub2branch2, 13));
	p_sub2branch2->set_right_node(new_node(p_sub2branch2, 14));
	p_root_node->set_right_node(p_branch2);
	p_branch2->set_right_node(p_sub2branch2);

	return p_root_node;
}

//
// draw_node
// 
// выполняет отрисовку ноды. 
// если нодой является корень дерева, то все дерево будет отрисовано целиком 
//
void draw_node(node *p_node)
{
	if (!p_node)
		return;

	// получаем родителя
	node *p_parent = p_node->get_parent_node();

	//рисуем линию только в случае если текущая нода не является родительской
	//(т.к нет позиции начала линии - потому что нет родителя)
	if (p_parent) 
		draw_line(p_parent->pos.x, p_parent->pos.y, p_node->pos.x, p_node->pos.y);

	//поверх линии рисуем круг ноды
	draw_circle(p_node->pos.x, p_node->pos.y, UNFILLED, 80, 100);

	// идем рисовать левую ветвь
	draw_node(p_node->get_left_node());

	// затем рисуем правую
	draw_node(p_node->get_right_node());
}

//
// compute_tree_nodes
//
// выполняет расчет позиций нод относительно оконных координат
// width - ширина окна
// height - высота окна
// 
void compute_tree_nodes(int width, int height)
{
	vec2 center = vec2(width >> 1, height >> 1);

	auto compute_node = [&](node *p_node) -> void {

	};
}

node *p_root_node;

int main()
{
	gldl_events_dt_t events;
	memset(&events, NULL, sizeof(events));
	events.p_window_resize_event = [](int width, int height) {
		int w = p_window->GetWindowWidth();
		int h = p_window->GetWindowHeight();
		glMatrixMode(GL_PROJECTION);
		glViewport(0, 0, w, h);
		glOrtho((double)0, (double)w, (double)0, (double)h, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	};

	p_window = gldl_initialize(-1, -1, 800, 600, 32, 24, "Бинарные деревья", &events);
	if (!p_window) {
		char error[256];
		sprintf_s(error, sizeof(error), "Failed to create window! Error = %d", gldl_last_error_string());
		MessageBoxA(HWND_DESKTOP, error, "Critical error!", MB_ICONERROR | MB_OK);
		return 1;
	}

	p_root_node = build_tree();

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST); //отключаем буфер глубины, он нам не потребуется
	glClearColor(1.f, 1.f, 1.f, 1.f); //цвет очистки буфера цвета (белый)
	glLineWidth(1.5);
	while (p_window->IsWindowOpened()) {
		glClear(GL_COLOR_BUFFER_BIT); //очистка буфера цвета
		glLoadIdentity(); //сброс матрицы модели вида
		
		p_window->Begin2D();
		glPushAttrib(GL_CURRENT_BIT); //сохраняем текущий цвет
		glColor3ub(0, 0, 0);
		draw_node(p_root_node);
		glPopAttrib(); //восстанавливаем ранее сохраненный цвет
		p_window->End2D();
	}
	gldl_shutdown(p_window);
	return 0;
}