#define DBG_ALLOW_IN_RELEASE

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windowsx.h>
#include <intrin.h>
#include "gl_exts.h"
#include "../cgcommon/gl_viewport.h"
#include "main.h"
#include "dbg.h"
#include "model.h"
#include "trackball.h"
#include "win32_controls.h"

#define FLT_PI   (3.14159265358979323846f)
#define RAD2DEG  (57.2957795)
#define DEG2RAD  (0.0174532925f)
#define ROTBYR(r) ((sinf(r) * RAD2DEG) * 2.0f)

HINSTANCE g_instance;
HWND h_main_window;
HWND h_control_panel;
GLVIEWPORT gl_viewport;

// For trackball.
float prevMouseX, prevMouseY;
float cam_curr_quat[4];
float cam_prev_quat[4];
float cam_eye[3], cam_lookat[3], cam_up[3];
const float initial_cam_pos[3] = { 0.0, 0.0, 3.0f };  // World coordinates.

typedef BOOL(WINAPI *wglSwapIntervalEXTPfn)(int interval);

struct mesh_hierarchy {
	const char *p_name;
	int parent;
};

enum MESHES {
	MESH_MAINFRAME = 0,
	MESH_APRON,
	MESH_APRON_TAP,
	MESH_SCENE_SCREW_DRIVE,
	MESH_SUPPORT_TOP,
	MESH_SUPPORT_MOVE_TAP,
	MESH_SUPPORT_OFFSET_TAP,

	MESH_HEADSTOCK,
	MESH_SPINDLE,
	MESH_SPINDLE_JAW1,
	MESH_SPINDLE_JAW2,
	MESH_SPINDLE_JAW3,

	MESH_TAILSTOCK,
	MESH_TAILSTOCK_TAP,

	MESH_SWITCH_BOTTOM1,
	MESH_SWITCH_BOTTOM2,
	MESH_SWITCH_TOP1,
	MESH_SWITCH_TOP2
};

#define DECL_HIERARCHY_PARAM(myid, name, parent) { name, parent }

mesh_hierarchy object_hierarchy[] = {
	DECL_HIERARCHY_PARAM(MESH_MAINFRAME, "mainframe", -1), //parent NONE

	DECL_HIERARCHY_PARAM(MESH_APRON, "apron", MESH_MAINFRAME), //parent "mainframe"
	DECL_HIERARCHY_PARAM(MESH_APRON_TAP, "apron_tap", MESH_APRON), //parent "apron"
	DECL_HIERARCHY_PARAM(MESH_SCENE_SCREW_DRIVE, "apron_screw_drive", MESH_MAINFRAME), //parent "mainframe"

	DECL_HIERARCHY_PARAM(MESH_SUPPORT_TOP, "support_top", MESH_APRON), //parent "apron"
	DECL_HIERARCHY_PARAM(MESH_SUPPORT_MOVE_TAP, "support_move_tap", MESH_APRON), //parent "apron"
	DECL_HIERARCHY_PARAM(MESH_SUPPORT_OFFSET_TAP, "support_offset_tap", MESH_SUPPORT_TOP), //parent "support_top"

	DECL_HIERARCHY_PARAM(MESH_HEADSTOCK, "headstock", MESH_MAINFRAME), //parent "mainframe"
	DECL_HIERARCHY_PARAM(MESH_SPINDLE, "spindle", MESH_HEADSTOCK), //parent "headstock"
	DECL_HIERARCHY_PARAM(MESH_SPINDLE_JAW1, "spindle_jaw1", MESH_SPINDLE), //parent "spindle"
	DECL_HIERARCHY_PARAM(MESH_SPINDLE_JAW2, "spindle_jaw2", MESH_SPINDLE), //parent "spindle"
	DECL_HIERARCHY_PARAM(MESH_SPINDLE_JAW3, "spindle_jaw3", MESH_SPINDLE), //parent "spindle"

	DECL_HIERARCHY_PARAM(MESH_TAILSTOCK, "tailstock", 0), //parent "mainframe"
	DECL_HIERARCHY_PARAM(MESH_TAILSTOCK_TAP, "tailstock_tap", MESH_TAILSTOCK), //parent "tailstock"

	DECL_HIERARCHY_PARAM(MESH_SWITCH_BOTTOM1, "switch_bottom1", MESH_MAINFRAME), //parent "mainframe"
	DECL_HIERARCHY_PARAM(MESH_SWITCH_BOTTOM2, "switch_bottom2", MESH_MAINFRAME), //parent "mainframe"
	DECL_HIERARCHY_PARAM(MESH_SWITCH_TOP1, "switch_top1", MESH_HEADSTOCK), //parent "headstock"
	DECL_HIERARCHY_PARAM(MESH_SWITCH_TOP2, "switch_top2", MESH_HEADSTOCK) //parent "headstock"
};

#define CNT(a) (sizeof(a) / sizeof(a[0]))

template<typename _type>
void my_swap(_type &a, _type &b)
{
	_type temp = a;
	a = b;
	b = temp;
}

void sort_meshes_by_hierarchy(model &mdl, const mesh_hierarchy *p_hierarchy, size_t count)
{
	mesh_s *p_mesh;
	std::vector<mesh_s *> &meshes = mdl.get_meshes();

	/* HIERARCHY DATA */
	for (size_t i = 0; i < count; i++) {

		/* EACH MESH */
		for (size_t j = i; j < meshes.size(); j++) {
			if (!strcmp(p_hierarchy[i].p_name, meshes[j]->name)) {
				my_swap<mesh_s *>(meshes[j], meshes[i]);
				goto __leave_cycle;
			}
		}
	__leave_cycle:;
	}
}

#if defined(_DEBUG) || defined(DBG_ALLOW_IN_RELEASE)
void print_meshes(model &mdl)
{
	for (size_t i = 0; i < mdl.get_meshes_count(); i++) {
		mesh_s *p_mesh = mdl.get_mesh_by_index(i);
		DBG("Mesh %d : (%s)", i, (p_mesh) ? p_mesh->name : "NULL");
	}
}
#endif

struct mesh_idxs_s {
	size_t frame; // mainframe
	size_t headstock_idx; //headstock

	size_t apron_idx; //apron X
	size_t apron_tap_idx; //apron_tap X
	size_t apron_screw_drive_idx; //apron_screw_drive

	size_t support_idx; //support_top
	size_t support_tap_idx; //support_move_tap XZ
	size_t support_offset_tap_idx; //support_offset_tap XZ

	size_t spindle_idx; //spindle rX
	size_t spindle_jaw1_idx; //spindle_jaw1 rX tYZ
	size_t spindle_jaw2_idx; //spindle_jaw2 rX tYZ
	size_t spindle_jaw3_idx; //spindle_jaw3 rX tYZ

	size_t tailstock_idx; //tailstock tX
	size_t tailstock_tap_idx; //tailstock_tap rX tX

	size_t switch_bottom1_idx; //switch_bottom1
	size_t switch_bottom2_idx; //switch_bottom2
	size_t switch_top1_idx; //switch_top1
	size_t switch_top2_idx; //switch_top2
};

ctls::toggle_button flat_shaded;
ctls::toggle_button smooth_shaded;
ctls::checkbox wireframe_check;
ctls::treeview meshes_hierarchy;
ctls::trackbar apron_move_track_x;
ctls::trackbar support_move_track_z;
ctls::trackbar tailstock_z;
ctls::trackbar spindle_rotation;
ctls::trackbar spindle_jaws_move;

float rotation_sensitivity = 1.0f;
float translation_sensitivity = 15.0f;

model scene_model;
mesh_idxs_s model_meshes_indices;

double last_time = 1.0;
double curr_time = 1.0;

glm::vec3 center_of_jaws;

//bool resolve_model_meshes_indices(mesh_idxs_s &dst_meshes_idxs, model &mdl)
//{
//	return (mdl.find_mesh_by_name(&dst_meshes_idxs.frame, "mainframe") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.apron_idx, "apron") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.apron_tap_idx, "apron_tap") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.apron_screw_drive_idx, "apron_screw_drive") &&
//
//		mdl.find_mesh_by_name(&dst_meshes_idxs.support_idx, "support_top") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.support_tap_idx, "support_move_tap") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.support_offset_tap_idx, "support_offset_tap") &&
//
//		mdl.find_mesh_by_name(&dst_meshes_idxs.spindle_idx, "spindle") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.spindle_jaw1_idx, "spindle_jaw1") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.spindle_jaw2_idx, "spindle_jaw2") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.spindle_jaw3_idx, "spindle_jaw3") &&
//
//		mdl.find_mesh_by_name(&dst_meshes_idxs.tailstock_idx, "tailstock") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.tailstock_tap_idx, "tailstock_tap") &&
//
//		mdl.find_mesh_by_name(&dst_meshes_idxs.switch_bottom1_idx, "switch_bottom1") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.switch_bottom2_idx, "switch_bottom2") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.switch_top1_idx, "switch_top1") &&
//		mdl.find_mesh_by_name(&dst_meshes_idxs.switch_top2_idx, "switch_top2"));
//}

void get_time_milliseconds(double *p_dst)
{
	LARGE_INTEGER counter, frequency;
	QueryPerformanceCounter(&counter);
	QueryPerformanceFrequency(&frequency);
	(*p_dst) = counter.QuadPart / (double)frequency.QuadPart;
}

void compute_positions_of_parents(model &mdl, const mesh_hierarchy *p_hierarchy, size_t count)
{
	std::vector<mesh_s *> &meshes = mdl.get_meshes();
	for (size_t i = 0; i < meshes.size() && i < count; i++) {
		if (p_hierarchy[i].parent == -1) {
			meshes[i]->pos_of_parent = glm::vec3(0.f, 0.f, 0.f);
			continue;
		}

		meshes[i]->pos_of_parent = meshes[i]->position - meshes[p_hierarchy[i].parent]->position;
		meshes[i]->pos_of_parent_curr = meshes[i]->pos_of_parent;

#if defined(_DEBUG) || defined(DBG_ALLOW_IN_RELEASE)
		glm::vec3 *p_pos_of_parent = &meshes[i]->pos_of_parent;
		glm::vec3 *p_parent_pos = &meshes[p_hierarchy[i].parent]->position;
		glm::vec3 *p_my_pos = &meshes[i]->position;
		DBG("(%f %f %f) - (%f %f %f) = ( %f %f %f )", p_parent_pos->x, p_parent_pos->y, p_parent_pos->z,
			p_my_pos->x, p_my_pos->y, p_my_pos->z,
			p_pos_of_parent->x, p_pos_of_parent->y, p_pos_of_parent->z);
#endif
	}
}

bool prepare_childs_indices(model &mdl, const mesh_hierarchy *p_hierarchy, size_t count)
{
	std::vector<mesh_s *> &meshes = mdl.get_meshes();

	/* PARENT */
	for (size_t i = 0; i < meshes.size() && i < count; i++) {
		mesh_s *p_parent = meshes[i];
		p_parent->num_childs = 0;

		/* CHILDS REFS TO PARENT (ME) */
		for (size_t j = 0; j < meshes.size() && i < count; j++) {
			if (p_hierarchy[j].parent == i) {
				if (p_parent->num_childs == MESH_MAX_CHILDS) {
					DBG("Mesh (%s) childs array overflowed (%d/%d)", p_parent->name, p_parent->num_childs, MESH_MAX_CHILDS);
					return false;
				}
				p_parent->childs_idxs[p_parent->num_childs] = (int)j;
				p_parent->num_childs++;
			}
		}

#if defined(_DEBUG) || defined(DBG_ALLOW_IN_RELEASE)
		if (p_parent->num_childs) {
			DBG("");
			DBG("------ (%s) CHILDS ------", p_parent->name);
			for (int i = 0; i < p_parent->num_childs; i++)
				DBG("  %d: (%s) child", i, meshes[p_parent->childs_idxs[i]]->name);
		}
#endif
	}
	return true;
}

void error_msg(const char *p_format, ...)
{
	va_list argptr;
	char buffer[1024];
	va_start(argptr, p_format);
	vsprintf_s(buffer, sizeof(buffer), p_format, argptr);
	va_end(argptr);

	MessageBoxA(HWND_DESKTOP, buffer, "CRITICAL ERROR", MB_OK | MB_ICONERROR);
	exit(1);
}

void center_of_screen(LPRECT p_dst_rect, int width, int height)
{
	p_dst_rect->right = width;
	p_dst_rect->bottom = height;
	p_dst_rect->left = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
	p_dst_rect->top = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);
}

void resize_ui(const LPRECT p_rect)
{
	RECT rect;
	SIZE clientrect_width;
	int controls_panel_width;

	clientrect_width.cx = p_rect->right - (MARGIN_PX * 2);
	clientrect_width.cy = p_rect->bottom - (MARGIN_PX * 2);
	controls_panel_width = CONTROL_PANEL_WIDTH;

	/* RECOMPUTE CONTROL WINDOW SIZE */
	rect.left = clientrect_width.cx - controls_panel_width + MARGIN_PX;
	rect.top = p_rect->top + MARGIN_PX;
	rect.right = CONTROL_PANEL_WIDTH;
	rect.bottom = clientrect_width.cy;
	MoveWindow(h_control_panel, rect.left, rect.top, rect.right, rect.bottom, TRUE);

	/* RECOMPUTE SCENE WINDOW SIZE */
	rect.left = p_rect->left + MARGIN_PX;
	rect.top = p_rect->top + MARGIN_PX;
	rect.right = clientrect_width.cx - controls_panel_width - MARGIN_PX;
	rect.bottom = clientrect_width.cy;
	MoveWindow(gl_viewport.h_viewport, rect.left, rect.top, rect.right, rect.bottom, FALSE);
	SetFocus(gl_viewport.h_viewport);
}

LRESULT CALLBACK wnd_proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK scene_wnd_proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK controlpanel_wnd_proc(HWND, UINT, WPARAM, LPARAM);

bool register_classes(HINSTANCE h_instance)
{
	/* REGISTER MAIN WINDOW CLASS */
	WNDCLASSEXA wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW|CS_VREDRAW;
	wcex.hInstance = h_instance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpfnWndProc = wnd_proc;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszClassName = WC_LAB2;
	if (!RegisterClassExA(&wcex))
		return false;

	/* CONTROL PANEL */
	wcex.lpfnWndProc = controlpanel_wnd_proc;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszClassName = WC_LAB2_CONTROLPANEL;
	if (!RegisterClassExA(&wcex))
		return false;

	/* REGISTER VIEWPORT WINDOW CLASS */
	wcex.lpfnWndProc = scene_wnd_proc;
	wcex.style = CS_OWNDC;
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszClassName = WC_LAB2_SCENE;
	if (!RegisterClassExA(&wcex))
		return false;

	return true;
}

void build_tree_node(ctls::treeview &tree, HTREEITEM h_parent_item, std::vector<mesh_s *> &meshes, mesh_s *p_mesh)
{
	h_parent_item = tree.insert_text_item(h_parent_item, p_mesh->name, 0);
	for (int i = 0; i < p_mesh->num_childs; i++)
		build_tree_node(tree, h_parent_item, meshes, meshes[p_mesh->childs_idxs[i]]);

	tree.expand(h_parent_item);
}

void build_hierarchy_tree(ctls::treeview &tree, model &mdl)
{
	std::vector<mesh_s *> &meshes = mdl.get_meshes();
	build_tree_node(tree, NULL, meshes, meshes[0]);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	RECT rect;
	int posx, posy;
	INITCOMMONCONTROLSEX iccex;

	DBG_INIT();

	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC = ICC_WIN95_CLASSES;
	if (!InitCommonControlsEx(&iccex)) {
		DWORD error = GetLastError();
		error_msg("Failed to initialize new version on windows common controls!\r\nError: %d (0x%x)", error, error);
		return 1;
	}

	g_instance = hInstance;
	if (!register_classes(hInstance)) {
		error_msg("Failed to create window class!");
		return 1;
	}

	/* CREATE MAIN WINDOW */
	center_of_screen(&rect, 800, 600);
	h_main_window = CreateWindowExA(0, WC_LAB2, "Lab2", WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right, rect.bottom, NULL, NULL, hInstance, NULL);
	if (!h_main_window) {
		error_msg("Failed to create window");
		return 2;
	}

	if (gl_viewport_init(&gl_viewport, WS_CHILD | WS_VISIBLE, MARGIN_PX, MARGIN_PX, 1, 1, h_main_window, WC_LAB2_SCENE, 24, 32) != VRES_OK) {
		error_msg("Failed to init OpenGL viewport");
		return 3;
	}
	gl_load_extensions();

	wglSwapIntervalEXTPfn wglSwapIntervalEXT = (wglSwapIntervalEXTPfn)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(60);

	h_control_panel = CreateWindowExA(WS_EX_DLGMODALFRAME, WC_LAB2_CONTROLPANEL, "", WS_CHILD | WS_VISIBLE, 1, 1, 1, 1, h_main_window, (HMENU)0, NULL, NULL);

	posx = posy = 2;
	ctls::set_default_font();
	wireframe_check = ctls::checkbox(h_control_panel, IDC_WIREFRAME, MARGIN_PX, "Wireframe", posx, &posy, 400, 30, 0, false);
	flat_shaded = ctls::toggle_button(h_control_panel, IDC_SHADE_MODEL_FLAT, "Flat", posx, posy, 50, 20, 0, true);
	smooth_shaded = ctls::toggle_button(h_control_panel, IDC_SHADE_MODEL_SMOOTH, MARGIN_PX, "Smooth", posx+50+MARGIN_PX, &posy, 50, 20);
	meshes_hierarchy = ctls::treeview(h_control_panel, IDC_HIERARCHY_TREE, posx, &posy, 300, 300, MARGIN_PX);
	apron_move_track_x = ctls::trackbar(h_control_panel, IDC_APRONMOVE_TRACK, MARGIN_PX, "Apron move (x)", posx, &posy, 300, 50);
	support_move_track_z = ctls::trackbar(h_control_panel, IDC_SUPPORTMOVE_TRACK, MARGIN_PX, "Support move (z)", posx, &posy, 300, 50);
	tailstock_z = ctls::trackbar(h_control_panel, IDC_TAILSTOCKMOVE_TRACK, MARGIN_PX, "Tailstock move (z)", posx, &posy, 300, 50);
	spindle_rotation = ctls::trackbar(h_control_panel, IDC_SPINDLEROTATION_TRACK, MARGIN_PX, "Spindle rotation", posx, &posy, 300, 50);
	spindle_jaws_move = ctls::trackbar(h_control_panel, IDC_SPINDLEJAWSMOVE_TRACK, MARGIN_PX, "Sinndle jaws move", posx, &posy, 300, 50);
	
	apron_move_track_x.set_pos(0);
	apron_move_track_x.set_minmax(-100, 100);
	spindle_rotation.set_minmax(0, 720);
	spindle_jaws_move.set_minmax(0, 20);

	// Initialization for trackball.
	trackball(cam_curr_quat, 0, 0, 0, 0);
	cam_eye[0] = initial_cam_pos[0];
	cam_eye[1] = initial_cam_pos[1];
	cam_eye[2] = initial_cam_pos[2];
	cam_lookat[0] = 0.0f;
	cam_lookat[1] = 0.0f;
	cam_lookat[2] = 0.0f;
	cam_up[0] = 0.0f;
	cam_up[1] = 1.0f;
	cam_up[2] = 0.0f;

	ShowWindow(h_main_window, SW_SHOW);
	UpdateWindow(h_main_window);

	if (!scene_model.load_model("models/machine_done2.obj")) {
		printf("Failed to load model!\n");
		return 1;
	}

	printf("---------- PRE MESHES ------------\n");
	print_meshes(scene_model);
	sort_meshes_by_hierarchy(scene_model, object_hierarchy, CNT(object_hierarchy));
	printf("---------- POST MESHES ------------\n");
	print_meshes(scene_model);

	compute_positions_of_parents(scene_model, object_hierarchy, CNT(object_hierarchy));
	if (!prepare_childs_indices(scene_model, object_hierarchy, CNT(object_hierarchy))) {
		return 1;
	}

	build_hierarchy_tree(meshes_hierarchy, scene_model);

	mesh_s *p_spindle = scene_model.get_mesh_by_index(MESH_SPINDLE);
	mesh_s *p_spindle_jaw1 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW1);
	mesh_s *p_spindle_jaw2 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW2);
	mesh_s *p_spindle_jaw3 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW3);

	center_of_jaws.x = (p_spindle_jaw1->pos_of_parent.x + p_spindle_jaw2->pos_of_parent.x + p_spindle_jaw3->pos_of_parent.x) / 3.f;
	center_of_jaws.y = (p_spindle_jaw1->pos_of_parent.y + p_spindle_jaw2->pos_of_parent.y + p_spindle_jaw3->pos_of_parent.y) / 3.f;
	center_of_jaws.z = (p_spindle_jaw1->pos_of_parent.z + p_spindle_jaw2->pos_of_parent.z + p_spindle_jaw3->pos_of_parent.z) / 3.f;

	//SuspendThread(GetCurrentThread());

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	GetClientRect(gl_viewport.h_viewport, &rect);
	glViewport(0, 0, rect.right, rect.bottom);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, rect.right / (double)rect.bottom, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat g_LighPos[] = { 10.0f, 100.0f, 10.0f, 1.0f };
	GLfloat g_LightAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat g_LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat g_LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat g_LighAttenuation0 = 1.0f;
	GLfloat g_LighAttenuation1 = 0.0f;
	GLfloat g_LighAttenuation2 = 0.0f;
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, g_LighPos);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_LightSpecular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, g_LighAttenuation0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, g_LighAttenuation1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, g_LighAttenuation2);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClearColor(0.5f, 0.5f, 0.5f, 1.5f);
	glShadeModel(GL_FLAT);

    MSG msg;
	while (1) {
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		/* DRAW SCENE */
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		// View transformation matrix.
		glm::mat4 viewMat = glm::lookAt(glm::vec3(cam_eye[0], cam_eye[1], cam_eye[2]),
			glm::vec3(cam_lookat[0], cam_lookat[1], cam_lookat[2]),
			glm::vec3(cam_up[0], cam_up[1], cam_up[2]));

		// Get the additional camera rotation matrix introduced by trackball.  
		GLfloat rot[4][4];
		build_rotmatrix(rot, cam_curr_quat);
		glm::mat4 camRotMat = glm::mat4(rot[0][0], rot[0][1], rot[0][2], rot[0][3],
			rot[1][0], rot[1][1], rot[1][2], rot[1][3],
			rot[2][0], rot[2][1], rot[2][2], rot[2][3],
			rot[3][0], rot[3][1], rot[3][2], rot[3][3]);

		// The final view transformation has the additional rotation from trackball.
		viewMat = viewMat * camRotMat;
		glMultMatrixf(glm::value_ptr(viewMat));

		//get_time_milliseconds(&curr_time);
		scene_model.draw_model();
		//last_time = curr_time;

		SwapBuffers(gl_viewport.h_device_context);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

    return 0;
}

#define MBUTTON_LEFT (1 << 0)
#define MBUTTON_RIGHT (1 << 1)
#define MBUTTON_CENTER (1 << 2)

void build_rotmatrix(glm::mat4 &dst_mat, const glm::quat &rot_quat)
{
	float rot_mat[4][4];
	build_rotmatrix(rot_mat, glm::value_ptr(rot_quat));
	dst_mat = glm::mat4(rot_mat[0][0], rot_mat[0][1], rot_mat[0][2], rot_mat[0][3],
		rot_mat[1][0], rot_mat[1][1], rot_mat[1][2], rot_mat[1][3],
		rot_mat[2][0], rot_mat[2][1], rot_mat[2][2], rot_mat[2][3],
		rot_mat[3][0], rot_mat[3][1], rot_mat[3][2], rot_mat[3][3]);
}

void norm_mouse_position(glm::vec2 &dst_mouse_pt, HWND h_wnd, int x, int y)
{
	RECT rect;
	GetClientRect(h_wnd, &rect);
	dst_mouse_pt.x = x / (float)rect.right;
	dst_mouse_pt.y = y / (float)rect.bottom;
}

#define ROTATION_SENSITIVITY (-0.01f)

LRESULT CALLBACK scene_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	SIZE size;
	static int mbflags = 0;
	
	switch (message) {

	case WM_SIZE: {
		size.cx = LOWORD(lParam);
		size.cy = HIWORD(lParam);
		if (!size.cy)
			size.cy = 1;

		glViewport(0, 0, size.cx, size.cy);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.f, size.cx / (double)size.cy, 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;
	}

	case WM_ERASEBKGND:
		return 1;

	case WM_KEYDOWN: {
		if (wParam == VK_F1) {
			static bool has_wireframe = true;
			glPolygonMode(GL_FRONT_AND_BACK, (has_wireframe) ? GL_LINE : GL_FILL);
			has_wireframe = !has_wireframe;
		}
		
		break;
	}

	case WM_LBUTTONDOWN:
		mbflags |= MBUTTON_LEFT;
		break;

	case WM_LBUTTONUP:
		mbflags &= ~MBUTTON_LEFT;
		trackball(cam_prev_quat, 0.0, 0.0, 0.0, 0.0);
		break;

	case WM_RBUTTONDOWN:
		mbflags |= MBUTTON_RIGHT;
		break;

	case WM_RBUTTONUP:
		mbflags &= ~MBUTTON_RIGHT;
		break;

	case WM_MBUTTONDOWN:
		mbflags |= MBUTTON_CENTER;
		break;

	case WM_MBUTTONUP:
		mbflags &= ~MBUTTON_CENTER;
		break;

	case WM_MOUSEMOVE: {
		GetClientRect(hWnd, &rect);

		float mouse_x = (float)GET_X_LPARAM(lParam);
		float mouse_y = (float)GET_Y_LPARAM(lParam);
		float window_width = (float)rect.right;
		float window_height = (float)rect.bottom;

		float p1x = rotation_sensitivity * (2.0f * prevMouseX - window_width) / window_width;
		float p1y = rotation_sensitivity * (window_height - 2.0f * prevMouseY) / window_height;

		float p2x = rotation_sensitivity * (2.0f * mouse_x - window_width) / window_width;
		float p2y = rotation_sensitivity * (window_height - 2.0f * mouse_y) / window_height;

		if (mbflags & MBUTTON_LEFT) {
			trackball(cam_prev_quat, p1x, p1y, p2x, p2y);
			add_quats(cam_prev_quat, cam_curr_quat, cam_curr_quat);
		}
		else if (mbflags & MBUTTON_CENTER) {
			cam_eye[0] -= translation_sensitivity * (mouse_x - prevMouseX) / window_width;
			cam_lookat[0] -= translation_sensitivity * (mouse_x - prevMouseX) / window_width;
			cam_eye[1] += translation_sensitivity * (mouse_y - prevMouseY) / window_height;
			cam_lookat[1] += translation_sensitivity * (mouse_y - prevMouseY) / window_height;
		}
		else if (mbflags & MBUTTON_RIGHT) {
			cam_eye[2] += translation_sensitivity * (mouse_y - prevMouseY) / window_height;
			cam_lookat[2] += translation_sensitivity * (mouse_y - prevMouseY) / window_height;
		}

		// Update mouse point
		prevMouseX = mouse_x;
		prevMouseY = mouse_y;
		break;
	}

	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
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

	case WM_SIZE: {
		GetClientRect(hWnd, &rect);
		resize_ui(&rect);
		break;
	}

	case WM_ACTIVATE:
		SetFocus(gl_viewport.h_viewport);
		break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	case WM_LBUTTONDOWN:
		break;

	case WM_MOUSEMOVE: {
	}

	case WM_LBUTTONUP:
		break;

    default:
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK controlpanel_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	float value;
	mesh_s *p_mesh;
	switch (message)
	{
	case WM_HSCROLL: {
		HWND h_scroll_sender = (HWND)lParam;

		/* HANDLE NO-DEFAULT SCROLLS */
		if (h_scroll_sender) {
			if (h_scroll_sender == apron_move_track_x) {
				value = (float)apron_move_track_x.get_pos();
				p_mesh = scene_model.get_mesh_by_index(MESH_APRON);
				p_mesh->pos_of_parent_curr.z = p_mesh->pos_of_parent.z + value;

				mesh_s *p_apron_tap = scene_model.get_mesh_by_index(MESH_APRON_TAP);
				p_apron_tap->rotation.x = ROTBYR(value);
				break;
			}

			if (h_scroll_sender == tailstock_z) {
				value = (float)tailstock_z.get_pos();
				p_mesh = scene_model.get_mesh_by_index(MESH_TAILSTOCK);
				p_mesh->pos_of_parent_curr.z = p_mesh->pos_of_parent.z + value;

				mesh_s *p_tailstock_tap = scene_model.get_mesh_by_index(MESH_TAILSTOCK_TAP);
				p_tailstock_tap->rotation.z = ROTBYR(value);
				break;
			}

			if (h_scroll_sender == support_move_track_z) {
				value = (float)support_move_track_z.get_pos();
				p_mesh = scene_model.get_mesh_by_index(MESH_SUPPORT_TOP);
				p_mesh->pos_of_parent_curr.x = p_mesh->pos_of_parent.x + (value * 0.03f);

				mesh_s *p_support_move_tap = scene_model.get_mesh_by_index(MESH_SUPPORT_MOVE_TAP);
				p_support_move_tap->rotation.x = ROTBYR(value);
				break;
			}

			if (h_scroll_sender == spindle_rotation) {
				scene_model.get_mesh_by_index(MESH_SPINDLE)->rotation.z = (float)spindle_rotation.get_pos();
				break;
			}

			//if (h_scroll_sender == spindle_jaws_move) {
			//	value = (float)spindle_jaws_move.get_pos();
			//	mesh_s *p_spindle = scene_model.get_mesh_by_index(MESH_SPINDLE);
			//	mesh_s *p_spindle_jaw1 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW1);
			//	mesh_s *p_spindle_jaw2 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW2);
			//	mesh_s *p_spindle_jaw3 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW3);

			//	//glm::vec3 diff = p_spindle_jaw1->pos_of_parent_curr - p_spindle->position;
			//	//float factor = value / diff.length();
			//	//diff *= factor;

			//	p_spindle_jaw1->pos_of_parent_curr.x = center_of_jaws.x + value;
			//	p_spindle_jaw1->pos_of_parent_curr.y = center_of_jaws.y + value;
			//										
			//	p_spindle_jaw2->pos_of_parent_curr.x = center_of_jaws.x + value;
			//	p_spindle_jaw2->pos_of_parent_curr.y = center_of_jaws.y + value;
			//										
			//	p_spindle_jaw3->pos_of_parent_curr.x = center_of_jaws.x + value;
			//	p_spindle_jaw3->pos_of_parent_curr.y = center_of_jaws.y + value;
			//	break;
			//}
		}
		break;
	}

	case WM_COMMAND:
	{
		SetFocus(gl_viewport.h_viewport);
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDC_WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, (wireframe_check.is_checked()) ? GL_LINE : GL_FILL);
			break;

		case IDC_SHADE_MODEL_FLAT:
			glShadeModel(GL_FLAT);
			smooth_shaded.set_check(false);
			break;

		case IDC_SHADE_MODEL_SMOOTH:
			glShadeModel(GL_SMOOTH);
			flat_shaded.set_check(false);
			break;

		default:
			return DefWindowProcA(hWnd, message, wParam, lParam);
		}
	}
	break;

	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return 0;
}