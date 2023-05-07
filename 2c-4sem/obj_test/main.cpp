#include "../cgcommon/glwnd.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../cgcommon/tiny_obj_loader.h"
#include "../cgcommon/glm/glm.hpp"

using namespace tinyobj;

INT Width, Height;

void fn_window_resize(HWND hWnd, int width, int height)
{
	if (!height)
		height = 1;

	Width = width;
	Height = height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, width / (double)height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

std::vector<std::vector<glm::vec3>> draw_objects;

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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);

	attrib_t attribs;
	std::string warnings, errors;
	std::vector<shape_t> shapes;
	std::vector<material_t> materials;
	if (!tinyobj::LoadObj(&attribs, &shapes, &materials, &warnings, &errors, "cube_obj.obj", NULL, true)) {
		DBG("Failed to load OBJ model. Error: %s\n", warnings.c_str());
		return;
	}

	for (size_t ishape = 0; ishape < shapes.size(); ishape++) {
		tinyobj::shape_t *p_shape = &shapes[ishape];
		DBG("Shape: %s\n", p_shape->name.c_str());

		std::vector<glm::vec3> group_verts;
		tinyobj::mesh_t *p_mesh = &p_shape->mesh;
		for (size_t idx = 0; idx < p_mesh->indices.size() / 3; idx++) {
			tinyobj::index_t idx0 = p_shape->mesh.indices[3 * idx + 0];
			tinyobj::index_t idx1 = p_shape->mesh.indices[3 * idx + 1];
			tinyobj::index_t idx2 = p_shape->mesh.indices[3 * idx + 2];

			// Get the three vertex indexes and coordinates
			int vi[3];      // indexes
			float v[3][3];  // coordinates
			for (int k = 0; k < 3; k++) {
				vi[0] = idx0.vertex_index;
				vi[1] = idx1.vertex_index;
				vi[2] = idx2.vertex_index;
				assert(vi[0] >= 0);
				assert(vi[1] >= 0);
				assert(vi[2] >= 0);

				v[0][k] = attribs.vertices[3 * vi[0] + k];
				v[1][k] = attribs.vertices[3 * vi[1] + k];
				v[2][k] = attribs.vertices[3 * vi[2] + k];
			}

			group_verts.push_back(glm::vec3(v[0][0], v[0][1], v[0][2]));
			group_verts.push_back(glm::vec3(v[1][0], v[1][1], v[1][2]));
			group_verts.push_back(glm::vec3(v[2][0], v[2][1], v[2][2]));
		}

		draw_objects.push_back(group_verts);
	}
}

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.f, 0.f, -150.f);
	glRotatef(45.f, 1.f, 0.f, 0.f);

	for (size_t i = 0; i < draw_objects.size(); i++) {
		glVertexPointer(3, GL_FLOAT, 0, draw_objects[i].data());
		glDrawArrays(GL_TRIANGLES, 0, draw_objects[i].size());
	}
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	glm::vec2 v1 = glm::vec2(-5.3f, -3.2f);
	glm::vec2 v2 = glm::vec2(5.6f, -3.1f);
	glm::vec2 v3 = glm::vec2(0.5f, 5.2f);
	DBG(
		"v1 ( %f %f )\n"
		"v2 ( %f %f )\n"
		"v3 ( %f %f )\n",
		v1.x, v1.y,
		v2.x, v2.y,
		v3.x, v3.y);

	glm::vec2 center = (v1 + v2 + v3) / 3.f;
	DBG("center( %f %f )\n", center.x, center.y);

	glm::vec2 nv1 = glm::normalize(v1 - center);
	glm::vec2 nv2 = glm::normalize(v2 - center);
	glm::vec2 nv3 = glm::normalize(v3 - center);
	DBG(
		"\n-------- direction ----------\n"
		"nv1 ( %f %f ) length: %f\n"
		"nv2 ( %f %f ) length: %f\n"
		"nv3 ( %f %f ) length: %f\n",
		nv1.x, nv1.y, glm::length(v1),
		nv2.x, nv2.y, glm::length(v2),
		nv3.x, nv3.y, glm::length(v3));

	float scale = 15.f;
	nv1 *= scale;
	nv2 *= scale;
	nv3 *= scale;
	DBG(
		"\n-------- direction multiplied with %f ----------\n"
		"nv1 ( %f %f )\n"
		"nv2 ( %f %f )\n"
		"nv3 ( %f %f )\n",
		scale,
		nv1.x, nv1.y,
		nv2.x, nv2.y,
		nv3.x, nv3.y);





	return 0;

	create_window("Test OBJ model loading", __FILE__ __TIME__,
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