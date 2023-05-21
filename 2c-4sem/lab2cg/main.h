#pragma once

#define WC_LAB2 "cgLab2Window"
//#define WC_LAB2_NOTIFY "cgLab2NotifyWindow"
#define WC_LAB2_SCENE "cgLab2SceneWindow"
#define WC_LAB2_CONTROLPANEL "cgLab2ControlPanel"
#define WC_LAB2_ANIMPANEL "cgLab2AnimPanel"

#define MARGIN_PX 2
#define CONTROL_PANEL_WIDTH 300
#define ANIM_CONTROL_PANEL_HEIGHT 100

/* CONTROLS ID's */
enum CONTROLS_IDS {

	/* CONTROL PANEL ID's */
	IDC_WIREFRAME = 100,
	IDC_SHADE_MODEL_FLAT,
	IDC_SHADE_MODEL_SMOOTH,
	IDC_HIERARCHY_TREE,
	IDC_APRONMOVE_TRACK,
	IDC_SUPPORTMOVE_TRACK,
	IDC_SUPPORT_HEAD_MOVE_TRACK,
	IDC_SUPPORT_OFFSET_TRACK,
	IDC_SUPPORT_HEAD_BOLTS,
	IDC_SCREW_DRIVE,
	IDC_TAILSTOCKMOVE_TRACK,
	IDC_SPINDLEROTATION_TRACK,
	IDC_SPINDLEJAWSMOVE_TRACK,

	/* ANIM CONTROL PANEL ID's */
	IDC_IMPORT_ANIM,
	IDC_EXPORT_ANIM,
	IDC_KEYFRAME_SET,
	IDC_KEYFRAME_UNDO,
	IDC_KEYFRAMES_CLEAR,
	IDC_KEYFRAME_SET_AS_ENDFRAME,
	IDC_KEYFRAME_SWITCH,
	IDC_PLAYING_FPS_TRACK,
	IDC_HAS_LOOP_PLAYING,
	IDC_ANIM_PLAY,
	IDC_ANIM_PAUSE,
	IDC_ANIM_STOP
};

#define CNT(a)             (sizeof(a) / sizeof(a[0]))
#define ABS(x)             (((x) < 0) ? -x : x)
#define CLAMP(x, mn, mx)   (((x) < mn) ? mn : ((x) > mx) ? mx : (x))
#define CLAMPMIN(x, mn)    (((x) < mn) ? mn : x)
#define CLAMPMAX(x, mx)    (((x) > mx) ? mx : x)
#define SLERP(last_val, next_val, time) (last_val + (next_val - last_val) * time)

/* ANIM FLAGS */
#define AF_ANIMATE (1 << 0)
#define AF_LOOP (1 << 1)

#define ANIM_VERSION 1