#pragma once
#include <Windows.h>

/* WINDOWS COMMON CONTROLS */
#include <CommCtrl.h>
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")

namespace ctls {
	void set_default_font();
	void set_font(HFONT h_font);
	HFONT get_font();

	/* CONTROL HANDLE (FOR SERVICE USE) */
	class control_handle {
		HWND h_control;
	public:
		control_handle() {}
		~control_handle() {}

		inline void set_handle(HWND handle) { h_control = handle; }
		inline HWND get_handle() { return h_control; }
		inline bool is_window() { return IsWindow(h_control); }
		inline bool is_enabled() { return IsWindowEnabled(h_control); }
		inline void disable() { EnableWindow(h_control, FALSE); }
		inline void enable() { EnableWindow(h_control, TRUE); }

		operator bool() { return h_control != NULL; }
		operator HWND() { return h_control; }
	};

	/* CHECKBOX */
	class checkbox : public control_handle {
	public:
		checkbox();
		checkbox(HWND parent, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex, bool init_state = false);
		~checkbox();

		bool is_checked();
		bool set_check(bool state);
	};

	/* TOGGLE BUTTON */
	class toggle_button : public control_handle {
	public:
		toggle_button();
		toggle_button(HWND parent, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex, bool init_state = false);
		~toggle_button();

		bool is_checked();
		bool set_check(bool state);
	};

	/* STATIC */
	class static_label : public control_handle {
	public:
		static_label();
		static_label(HWND parent, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex);
		~static_label();

		void set_text(const char *p_text);
		void set_textf(const char *p_format, ...);
		void get_text(char *p_dst, size_t dstlen);
	};

	/* TRACKBAR */
	class trackbar : public control_handle {
	public:
		trackbar();
		trackbar(HWND parent, const char *p_label, int x, int y, int width, int height, int rmin = 0, int rmax = 1, int pos = 0, DWORD dw_style_ex = 0, DWORD dw_style = 0);
		~trackbar();

		void set_minmax(int min, int max);
		int get_min();
		int get_max();

		void set_pos(int pos);
		int get_pos();
	};

	/* EDITBOX */
	class editbox : public control_handle {
	public:
		editbox();
		editbox(HWND parent, const char *p_text, int x, int y, int width, int height, DWORD dw_style_ex = 0, DWORD dw_style = 0);
		~editbox();

		void set_text(const char *p_text);
		void get_text(char *p_dst, size_t dstlen);
	};

	class logbox : public control_handle {
		logbox();
		logbox(HWND parent, const char *p_text, int x, int y, int width, int height, DWORD dw_style_ex = 0, DWORD dw_style = 0);
		~logbox();
	
	};
};