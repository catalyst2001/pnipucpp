﻿#include <stdio.h>
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
#define RAD2DEG  (57.2957795f)
#define DEG2RAD  (0.0174532925f)
#define ROTBYR(r) ((sinf(r) * RAD2DEG) * 2.0f)
#define ROTBYD(r) ((sinf(r * DEG2RAD) * RAD2DEG) * 2.0f)

typedef BOOL(WINAPI *wglSwapIntervalEXTPfn)(int interval);

HINSTANCE g_instance;
HWND h_main_window;
HWND h_control_panel;
HWND h_anim_control_panel;

GLVIEWPORT gl_viewport;
ctls::toggle_button flat_shaded;
ctls::toggle_button smooth_shaded;
ctls::checkbox wireframe_check;
ctls::treeview meshes_hierarchy;
ctls::trackbar apron_move_track_x;
ctls::trackbar support_move_track_z;
ctls::trackbar tailstock_z;
ctls::trackbar spindle_rotation;
ctls::trackbar spindle_jaws_move;
ctls::trackbar support_head_rotation;
ctls::trackbar support_head_offset;
ctls::trackbar support_head_bolts_move;
//ctls::trackbar apron_screw_drive;

// ANIM PANEL
ctls::trackbar keyframes_switch;
ctls::trackbar anim_fps_switch;
ctls::checkbox anim_loop_play;

// TRACKBALL
float prevMouseX, prevMouseY;
float cam_curr_quat[4];
float cam_prev_quat[4];
float cam_eye[3], cam_lookat[3], cam_up[3];
const float initial_cam_pos[3] = { 0.0, 0.0, 3.0f };

float rotation_sensitivity = 1.0f;
float translation_sensitivity = 15.0f;

model scene_model;

double last_time = 1.0;
double curr_time = 1.0;

float jaws_distance_of_center = 0.f;
glm::vec2 center_of_jaws;

/* ANIMATION */
int animation_flags = 0;
double anim_time_interval = 0.0;
double anim_last_time = 0.0;
int anim_current_frame = 0;
int anim_next_frame = 0;
int anim_max_frames = 0;

struct mesh_hierarchy {
	const char *p_name;
	int parent;
};

enum MESHES {
	MESH_MAINFRAME = 0,
	MESH_APRON_SCREW_DRIVE,
	MESH_APRON,
	MESH_APRON_TAP,
	MESH_SUPPORT_TOP,
	MESH_SUPPORT_MOVE_TAP,
	MESH_SUPPORT_OFFSET_TAP,
	MESH_SUPPORT_HEAD,
	MESH_SUPPORT_HEAD_KNIFE,
	MESH_SUPPORT_HEAD_BOLTS,

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

struct bone_transform {
	vec3 position;
	vec3 rotation;
};

struct keyframe_s {
private:
	bool keyframes_equals(keyframe_s &kf) {
		size_t num_bones = bones_positions.size();
		if (num_bones != kf.bones_positions.size())
			return false;

		for (size_t i = 0; i < num_bones; i++) {
			if (bones_positions[i].position != kf.bones_positions[i].position ||
				bones_positions[i].rotation != kf.bones_positions[i].rotation) {
				return false;
			}
		}
		return true;
	}

public:
	keyframe_s() {}
	~keyframe_s() {}

	std::vector<bone_transform> bones_positions;

	inline bool operator==(keyframe_s &kf) { return keyframes_equals(kf); }
	inline bool operator!=(keyframe_s &kf) { return !keyframes_equals(kf); }
};

vector<keyframe_s> recorded_keyframes;

#define DECL_HIERARCHY_PARAM(myid, name, parent) { name, parent }

mesh_hierarchy object_hierarchy[] = {
	DECL_HIERARCHY_PARAM(MESH_MAINFRAME, "mainframe", -1), //parent NONE

	DECL_HIERARCHY_PARAM(MESH_APRON_SCREW_DRIVE, "apron_screw_drive", MESH_MAINFRAME), //parent "mainframe"
	DECL_HIERARCHY_PARAM(MESH_APRON, "apron", MESH_MAINFRAME), //parent "mainframe"
	DECL_HIERARCHY_PARAM(MESH_APRON_TAP, "apron_tap", MESH_APRON), //parent "apron"

	DECL_HIERARCHY_PARAM(MESH_SUPPORT_TOP, "support_top", MESH_APRON), //parent "apron"
	DECL_HIERARCHY_PARAM(MESH_SUPPORT_MOVE_TAP, "support_move_tap", MESH_APRON), //parent "apron"
	DECL_HIERARCHY_PARAM(MESH_SUPPORT_OFFSET_TAP, "support_offset_tap", MESH_SUPPORT_TOP), //parent "support_top"
	DECL_HIERARCHY_PARAM(MESH_SUPPORT_HEAD, "support_head", MESH_SUPPORT_TOP), // parent "support_top"
	DECL_HIERARCHY_PARAM(MESH_SUPPORT_HEAD_KNIFE, "knife", MESH_SUPPORT_HEAD), // parent "support_head"
	DECL_HIERARCHY_PARAM(MESH_SUPPORT_HEAD_BOLTS, "support_head_bolts", MESH_SUPPORT_HEAD), // parent "support_head"

	DECL_HIERARCHY_PARAM(MESH_HEADSTOCK, "headstock", MESH_MAINFRAME), //parent "mainframe"
	DECL_HIERARCHY_PARAM(MESH_SPINDLE, "spindle", MESH_HEADSTOCK), //parent "headstock"
	DECL_HIERARCHY_PARAM(MESH_SPINDLE_JAW1, "spindle_jaw1", MESH_SPINDLE), //parent "spindle"
	DECL_HIERARCHY_PARAM(MESH_SPINDLE_JAW2, "spindle_jaw2", MESH_SPINDLE), //parent "spindle"
	DECL_HIERARCHY_PARAM(MESH_SPINDLE_JAW3, "spindle_jaw3", MESH_SPINDLE), //parent "spindle"

	DECL_HIERARCHY_PARAM(MESH_TAILSTOCK, "tailstock", MESH_MAINFRAME), //parent "mainframe"
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
	char buffer[512];
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
		sprintf_s(buffer, sizeof(buffer), "Mesh group %s not found in loaded OBJ model!", p_hierarchy[i].p_name);
		MessageBoxA(HWND_DESKTOP, buffer, "Hierarchy sort warning!", MB_OK | MB_ICONWARNING);

	__leave_cycle:
		(void)0;
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
	rect.bottom = CLAMPMIN(clientrect_width.cy - ANIM_CONTROL_PANEL_HEIGHT - MARGIN_PX, 0);
	MoveWindow(gl_viewport.h_viewport, rect.left, rect.top, rect.right, rect.bottom, FALSE);
	SetFocus(gl_viewport.h_viewport);


	/* RECOMPUTE ANIM CONTROL WINDOW SIZE */
	rect.left = p_rect->left + MARGIN_PX;
	rect.top = rect.bottom + MARGIN_PX;
	//rect.right = clientrect_width.cx - controls_panel_width - MARGIN_PX;
	rect.bottom = rect.top + ANIM_CONTROL_PANEL_HEIGHT - MARGIN_PX;
	MoveWindow(h_anim_control_panel, rect.left, rect.top, rect.right, rect.bottom, TRUE);
	MoveWindow(keyframes_switch, MARGIN_PX, 20+40+MARGIN_PX, rect.right - MARGIN_PX, 40, TRUE);
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
	wcex.lpszClassName = WC_LAB2_CONTROLPANEL;
	if (!RegisterClassExA(&wcex))
		return false;
	
	/* ANIM CONTROL PANEL */
	wcex.lpfnWndProc = controlpanel_wnd_proc;
	wcex.lpszClassName = WC_LAB2_ANIMPANEL;
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

inline void prepare_keyframe_track(ctls::trackbar &h_track, int pos, int num_frames)
{
	h_track.set_minmax(0, num_frames);
	h_track.set_pos(pos);
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

	ctls::set_default_font();

	/* ANIM PANEL */
	posx = MARGIN_PX;
	posy = 20;
	h_anim_control_panel = CreateWindowExA(WS_EX_DLGMODALFRAME, WC_LAB2_ANIMPANEL, "", WS_VISIBLE | WS_CHILD, 0, 0, 1, 1, h_main_window, (HMENU)0, NULL, NULL);
	ctls::button(h_anim_control_panel, IDC_IMPORT_ANIM, "Import ANM", posx, posy, 80, 20);
	ctls::button(h_anim_control_panel, IDC_EXPORT_ANIM, "Export ANM", posx += 80 + MARGIN_PX, posy, 80, 20);
	ctls::button(h_anim_control_panel, IDC_KEYFRAME_SET, "Set KF", posx += 100 + MARGIN_PX, posy, 80, 20);
	ctls::button(h_anim_control_panel, IDC_KEYFRAME_UNDO, "Undo KF", posx += 80 + MARGIN_PX, posy, 80, 20);
	ctls::button(h_anim_control_panel, IDC_KEYFRAMES_CLEAR, "Clear KFs", posx += 80 + MARGIN_PX, posy, 80, 20);
	ctls::button(h_anim_control_panel, IDC_KEYFRAME_SET_AS_ENDFRAME, "Set KF as END", posx += 80 + MARGIN_PX, posy, 80, 20);
	anim_loop_play = ctls::checkbox(h_anim_control_panel, IDC_HAS_LOOP_PLAYING, "Loop playing?", posx += 100 + MARGIN_PX, posy, 80, 20, 0, true);
	anim_fps_switch = ctls::trackbar(h_anim_control_panel, IDC_PLAYING_FPS_TRACK, "Anim FPS", posx += 80 + MARGIN_PX, posy-20, 80, 40, 0, -100, 100, 0, 0, WS_VISIBLE|WS_CHILD|TBS_AUTOTICKS);
	ctls::button(h_anim_control_panel, IDC_ANIM_PLAY, "Play", posx += 80 + MARGIN_PX, posy, 50, 20);
	ctls::button(h_anim_control_panel, IDC_ANIM_PAUSE, "Pause", posx += 50 + MARGIN_PX, posy, 50, 20);
	ctls::button(h_anim_control_panel, IDC_ANIM_STOP, "Stop", posx += 50 + MARGIN_PX, posy, 50, 20);
	posx = MARGIN_PX;
	posy += 40 + MARGIN_PX;
	keyframes_switch = ctls::trackbar(h_anim_control_panel, IDC_KEYFRAME_SWITCH, NULL, posx, posy, 1, 40, 0, 0, 0, 0, 0, WS_VISIBLE|WS_CHILD|TBS_TOP|TBS_AUTOTICKS);
	keyframes_switch.set_minmax(0, 0);
	keyframes_switch.set_tick_freq(1);
	anim_fps_switch.set_tick_freq(10);
	anim_fps_switch.set_minmax(-100, 100);
	anim_fps_switch.set_pos(24);
	anim_time_interval = 1.0 / ABS(anim_fps_switch.get_pos());

	/* CREATE CONTROL PANEL */
	h_control_panel = CreateWindowExA(WS_EX_DLGMODALFRAME, WC_LAB2_CONTROLPANEL, "", WS_CHILD | WS_VISIBLE, 1, 1, 1, 1, h_main_window, (HMENU)0, NULL, NULL);

	posx = posy = 2;
	wireframe_check = ctls::checkbox(h_control_panel, IDC_WIREFRAME, MARGIN_PX, "Wireframe", posx, &posy, 400, 30, 0, false);
	flat_shaded = ctls::toggle_button(h_control_panel, IDC_SHADE_MODEL_FLAT, "Flat", posx, posy, 50, 20, 0, true);
	smooth_shaded = ctls::toggle_button(h_control_panel, IDC_SHADE_MODEL_SMOOTH, MARGIN_PX, "Smooth", posx+50+MARGIN_PX, &posy, 50, 20);
	meshes_hierarchy = ctls::treeview(h_control_panel, IDC_HIERARCHY_TREE, posx, &posy, 300, 300, MARGIN_PX);
	apron_move_track_x = ctls::trackbar(h_control_panel, IDC_APRONMOVE_TRACK, MARGIN_PX, "Apron move", posx, &posy, 300, 50);
	support_move_track_z = ctls::trackbar(h_control_panel, IDC_SUPPORTMOVE_TRACK, MARGIN_PX, "Support move", posx, &posy, 300, 50);
	tailstock_z = ctls::trackbar(h_control_panel, IDC_TAILSTOCKMOVE_TRACK, MARGIN_PX, "Tailstock move", posx, &posy, 300, 50);
	spindle_rotation = ctls::trackbar(h_control_panel, IDC_SPINDLEROTATION_TRACK, MARGIN_PX, "Spindle rotation", posx, &posy, 300, 50);
	spindle_jaws_move = ctls::trackbar(h_control_panel, IDC_SPINDLEJAWSMOVE_TRACK, MARGIN_PX, "Spindle jaws move", posx, &posy, 300, 50);
	support_head_rotation = ctls::trackbar(h_control_panel, IDC_SUPPORT_HEAD_MOVE_TRACK, MARGIN_PX, "Support head rotation", posx, &posy, 300, 50);
	support_head_offset = ctls::trackbar(h_control_panel, IDC_SUPPORT_OFFSET_TRACK, MARGIN_PX, "Support head offset", posx, &posy, 300, 50);
	support_head_bolts_move = ctls::trackbar(h_control_panel, IDC_SUPPORT_HEAD_BOLTS, MARGIN_PX, "Support head unscrew/screw bolts", posx, &posy, 300, 50);
	//apron_screw_drive = ctls::trackbar(h_control_panel, IDC_SUPPORT_OFFSET_TRACK, MARGIN_PX, "Apron screw drive move", posx, &posy, 300, 50);
	
	apron_move_track_x.set_minmax(0, 78);
	apron_move_track_x.set_pos(0);
	tailstock_z.set_minmax(0, 85);
	tailstock_z.set_pos(0);
	spindle_rotation.set_minmax(0, 720);
	spindle_jaws_move.set_minmax(0, 20);
	support_head_rotation.set_minmax(-45, 45);
	support_head_rotation.set_pos(0);
	support_head_offset.set_minmax(0, 100);
	support_head_bolts_move.set_minmax(0, 10);
	support_head_bolts_move.set_pos(0);
	//apron_screw_drive.set_minmax(0, 720);
	//apron_screw_drive.set_pos(0);

	// TRACKBALL INIT
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

	if (!scene_model.load_model("models/machine_done3.obj")) {
		error_msg("Failed to load model!");
		return 1;
	}

	ShowWindow(h_main_window, SW_SHOWMAXIMIZED);

	DBG("---------- PRE MESHES ------------\n");
	print_meshes(scene_model);
	sort_meshes_by_hierarchy(scene_model, object_hierarchy, CNT(object_hierarchy));
	DBG("---------- POST MESHES ------------\n");
	print_meshes(scene_model);

	compute_positions_of_parents(scene_model, object_hierarchy, CNT(object_hierarchy));
	if (!prepare_childs_indices(scene_model, object_hierarchy, CNT(object_hierarchy))) {
		return 1;
	}

	build_hierarchy_tree(meshes_hierarchy, scene_model);

	mesh_s *p_spindle_jaw1 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW1);
	mesh_s *p_spindle_jaw2 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW2);
	mesh_s *p_spindle_jaw3 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW3);
	center_of_jaws.x = (p_spindle_jaw1->pos_of_parent.x + p_spindle_jaw2->pos_of_parent.x + p_spindle_jaw3->pos_of_parent.x) / 3.f;
	center_of_jaws.y = (p_spindle_jaw1->pos_of_parent.y + p_spindle_jaw2->pos_of_parent.y + p_spindle_jaw3->pos_of_parent.y) / 3.f;
	jaws_distance_of_center = glm::length(glm::vec2(p_spindle_jaw1->pos_of_parent.x, p_spindle_jaw1->pos_of_parent.y) - center_of_jaws);
	//DBG("Distance of center: %f\n", jaws_distance_of_center);

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
		get_time_milliseconds(&curr_time);
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		/* DRAW SCENE */
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glm::mat4 viewMat = glm::lookAt(glm::vec3(cam_eye[0], cam_eye[1], cam_eye[2]),
			glm::vec3(cam_lookat[0], cam_lookat[1], cam_lookat[2]),
			glm::vec3(cam_up[0], cam_up[1], cam_up[2]));
 
		GLfloat rot[4][4];
		build_rotmatrix(rot, cam_curr_quat);
		glm::mat4 camRotMat = glm::mat4(rot[0][0], rot[0][1], rot[0][2], rot[0][3],
			rot[1][0], rot[1][1], rot[1][2], rot[1][3],
			rot[2][0], rot[2][1], rot[2][2], rot[2][3],
			rot[3][0], rot[3][1], rot[3][2], rot[3][3]);

		viewMat = viewMat * camRotMat;
		glMultMatrixf(glm::value_ptr(viewMat));

		/* HANDLE ANIM IF ENABLED */
		if (animation_flags & AF_ANIMATE) {
			double delta_time = curr_time - last_time;
			anim_last_time += delta_time;
			if (anim_last_time >= anim_time_interval) {
				

				printf(
					"curr_time: %lf\n"
					"last_time: %lf\n"
					"delta_time: %lf"
					"anim_last_time: %lf\n"
					"anim_time_interval: %lf\n\n"
					"anim_current_frame: %d\n"
					"anim_next_frame: %d\n"
					"anim_max_frames: %d\n",
					curr_time, last_time, delta_time, anim_last_time, anim_time_interval,
					anim_current_frame, anim_next_frame, anim_max_frames
				);

				anim_last_time = 0.0;
				anim_next_frame++;
				anim_current_frame++;

				if (anim_current_frame >= anim_max_frames) {
					anim_current_frame = 0;
					if (!(animation_flags & AF_LOOP)) {
						keyframes_switch.set_pos(0);
						animation_flags &= ~AF_ANIMATE; // stop animation
					}
				}

				if (anim_next_frame >= anim_max_frames)
					anim_next_frame = 0;

				keyframes_switch.set_pos(anim_current_frame);
			}

			// interpolate frames
			if (animation_flags & AF_ANIMATE) {
				float interp = (float)delta_time * anim_time_interval;
				keyframe_s &current_kf = recorded_keyframes[anim_current_frame];
				keyframe_s &next_kf = recorded_keyframes[anim_next_frame];
				for (size_t i = 0; i < scene_model.get_meshes_count(); i++) {
					mesh_s *p_mesh = scene_model.get_mesh_by_index(i);
					bone_transform &curr_kf_bone_transform = current_kf.bones_positions[i];
					bone_transform &next_kf_bone_transform = next_kf.bones_positions[i];
					p_mesh->pos_of_parent_curr.x = SLERP(curr_kf_bone_transform.position.x, next_kf_bone_transform.position.x, interp);
					p_mesh->pos_of_parent_curr.y = SLERP(curr_kf_bone_transform.position.y, next_kf_bone_transform.position.y, interp);
					p_mesh->pos_of_parent_curr.z = SLERP(curr_kf_bone_transform.position.z, next_kf_bone_transform.position.z, interp);

					p_mesh->rotation.x = SLERP(curr_kf_bone_transform.rotation.x, next_kf_bone_transform.rotation.x, interp);
					p_mesh->rotation.y = SLERP(curr_kf_bone_transform.rotation.y, next_kf_bone_transform.rotation.y, interp);
					p_mesh->rotation.z = SLERP(curr_kf_bone_transform.rotation.z, next_kf_bone_transform.rotation.z, interp);
				}
			}
		}

		scene_model.draw_model();
		SwapBuffers(gl_viewport.h_device_context);
		last_time = curr_time;
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

bool keyframe_is_new(model &mdl, keyframe_s &new_keyframe)
{
	size_t size = recorded_keyframes.size();
	if (!size)
		return true;

	keyframe_s &last_keyframe = recorded_keyframes[size - 1];
	return last_keyframe != new_keyframe;
}

void keyframe_grab(keyframe_s &kf, model &mdl)
{
	mesh_s *p_mesh;
	bone_transform transform;
	kf.bones_positions.reserve(mdl.get_meshes_count());
	for (size_t i = 0; i < mdl.get_meshes_count(); i++) {
		p_mesh = mdl.get_mesh_by_index(i);
		transform.position = p_mesh->pos_of_parent_curr;
		transform.rotation = p_mesh->rotation;
		kf.bones_positions.push_back(transform);
	}
}

void keyframe_put(model &mdl, keyframe_s &kf)
{
	mesh_s *p_mesh;
	for (size_t i = 0; i < mdl.get_meshes_count(); i++) {
		p_mesh = mdl.get_mesh_by_index(i);
		p_mesh->pos_of_parent_curr = kf.bones_positions[i].position;
		p_mesh->rotation = kf.bones_positions[i].rotation;
	}
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
		case 0:
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
	char buffer[128];
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

			if (h_scroll_sender == spindle_jaws_move) {
				value = ((float)spindle_jaws_move.get_pos() * 0.08f) + jaws_distance_of_center;
				mesh_s *p_spindle_jaw1 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW1);
				mesh_s *p_spindle_jaw2 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW2);
				mesh_s *p_spindle_jaw3 = scene_model.get_mesh_by_index(MESH_SPINDLE_JAW3);
				glm::vec2 spindle_jaw1_pos = glm::vec2(p_spindle_jaw1->pos_of_parent.x, p_spindle_jaw1->pos_of_parent.y);
				glm::vec2 spindle_jaw2_pos = glm::vec2(p_spindle_jaw2->pos_of_parent.x, p_spindle_jaw2->pos_of_parent.y);
				glm::vec2 spindle_jaw3_pos = glm::vec2(p_spindle_jaw3->pos_of_parent.x, p_spindle_jaw3->pos_of_parent.y);
				glm::vec2 center = (spindle_jaw1_pos + spindle_jaw2_pos + spindle_jaw3_pos) / 3.f;
				spindle_jaw1_pos = glm::normalize(spindle_jaw1_pos - center) * value;
				spindle_jaw2_pos = glm::normalize(spindle_jaw2_pos - center) * value;
				spindle_jaw3_pos = glm::normalize(spindle_jaw3_pos - center) * value;

				p_spindle_jaw1->pos_of_parent_curr = glm::vec3(spindle_jaw1_pos.x, spindle_jaw1_pos.y, p_spindle_jaw1->pos_of_parent.z);
				p_spindle_jaw2->pos_of_parent_curr = glm::vec3(spindle_jaw2_pos.x, spindle_jaw2_pos.y, p_spindle_jaw2->pos_of_parent.z);
				p_spindle_jaw3->pos_of_parent_curr = glm::vec3(spindle_jaw3_pos.x, spindle_jaw3_pos.y, p_spindle_jaw3->pos_of_parent.z);
				break;
			}

			if (h_scroll_sender == support_head_rotation) {
				p_mesh = scene_model.get_mesh_by_index(MESH_SUPPORT_HEAD);
				p_mesh->rotation.y = (float)support_head_rotation.get_pos();
				break;
			}

			if (h_scroll_sender == support_head_offset) {
				value = (float)support_head_offset.get_pos();
				mesh_s *p_support_offset_tap = scene_model.get_mesh_by_index(MESH_SUPPORT_OFFSET_TAP);
				p_mesh = scene_model.get_mesh_by_index(MESH_SUPPORT_HEAD);
				p_mesh->pos_of_parent_curr.z = p_mesh->pos_of_parent.z + value * 0.01f;
				p_support_offset_tap->rotation.z = ROTBYR(value);
				break;
			}
			
			if (h_scroll_sender == support_head_bolts_move) {
				p_mesh = scene_model.get_mesh_by_index(MESH_SUPPORT_HEAD_BOLTS);
				p_mesh->pos_of_parent_curr.y = p_mesh->pos_of_parent.y + (float)support_head_bolts_move.get_pos() * 0.1f;
				break;
			}

			//if (h_scroll_sender == apron_screw_drive) {
			//	value = (float)apron_screw_drive.get_pos();
			//	mesh_s *p_apron_screw_drive = scene_model.get_mesh_by_index(MESH_APRON_SCREW_DRIVE);
			//	p_apron_screw_drive->rotation.z = value;
			//	printf("%f\n", apron_screw_drive.get_pos() * 1.f);
			//	break;
			//}

			if (h_scroll_sender == keyframes_switch && recorded_keyframes.size()) {
				int keyframe_idx = keyframes_switch.get_pos();
				keyframe_put(scene_model, recorded_keyframes[keyframe_idx]);
				DBG("Keyframe %d is apply to model", keyframe_idx);
				break;
			}

			/* ANIM FPS */
			if (h_scroll_sender == anim_fps_switch) {
				int curr_fps = anim_fps_switch.get_pos();
				anim_time_interval = 1.0 / ABS(curr_fps);
				if (curr_fps >= 0) {
					
				}
				else {
					//INVERSE PLAYING
				}
				break;
			}
		}
		break;
	}

	case WM_COMMAND:
	{
		SetFocus(gl_viewport.h_viewport);
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		/* CONTROL PANEL */
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

		/* ANIMATION */
		case IDC_IMPORT_ANIM:
			break;

		case IDC_EXPORT_ANIM:
			break;

		case IDC_KEYFRAME_SET: {
			//__asm int 3h;
			keyframe_s kf;
			int num_of_frames;
			int current_keyframe;
			keyframe_grab(kf, scene_model);
			num_of_frames = (int)recorded_keyframes.size();
			current_keyframe = keyframes_switch.get_pos();
			//if (keyframe_is_new(scene_model, kf)) {
				if (keyframes_switch.get_max() == current_keyframe) {
					keyframes_switch.set_max(num_of_frames);
					keyframes_switch.set_pos(num_of_frames);
					recorded_keyframes.push_back(kf);
					DBG("created new keyframe");
				}
				else {
					recorded_keyframes[current_keyframe] = kf;
					keyframes_switch.set_pos(current_keyframe + 1);
					DBG("overwrited exists keyframe");
				}
			//}
			anim_max_frames = (int)recorded_keyframes.size(); // update animation max frames
			DBG("IDC_KEYFRAME_SET: num_frames: %d", anim_max_frames);
			break;
		}

		case IDC_KEYFRAME_UNDO:
			size_t size;
			if(recorded_keyframes.size())
				recorded_keyframes.pop_back();
			
			size = recorded_keyframes.size();
			anim_max_frames = (int)size;
			if (size)
				keyframe_put(scene_model, recorded_keyframes[size - 1]);

			break;

		case IDC_KEYFRAMES_CLEAR:
			recorded_keyframes.clear();
			keyframes_switch.set_max(0);
			keyframes_switch.set_pos(0);
			anim_max_frames = (int)recorded_keyframes.size(); // update animation max frames
			break;

		case IDC_KEYFRAME_SET_AS_ENDFRAME:
			int keyframe_idx;
			keyframe_idx = keyframes_switch.get_pos();
			recorded_keyframes.resize(keyframe_idx);
			keyframes_switch.set_max(keyframe_idx);
			anim_max_frames = (int)recorded_keyframes.size(); // update animation max frames
			break;

		case IDC_HAS_LOOP_PLAYING:
			if (anim_loop_play.is_checked()) {
				animation_flags |= AF_LOOP;
			}
			else {
				animation_flags &= ~AF_LOOP;
			}
			break;

		case IDC_ANIM_PLAY:
			if(recorded_keyframes.size() > 2)
				animation_flags |= AF_ANIMATE;

			break;

		case IDC_ANIM_PAUSE:
			animation_flags &= ~AF_ANIMATE;
			break;

		case IDC_ANIM_STOP:
			anim_current_frame = 0;
			anim_next_frame = 1;
			animation_flags &= ~AF_ANIMATE;
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