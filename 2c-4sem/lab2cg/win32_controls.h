#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>

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
	
	protected:
		control_handle() {}
		control_handle(HWND handle) { h_control = handle; }
		~control_handle() {}

		inline void init_handle(HWND handle);
	public:
		inline void set_handle(HWND handle) { h_control = handle; }
		inline HWND get_handle() const { return h_control; }
		inline bool is_window() const { return IsWindow(h_control); }
		inline bool is_enabled() const { return IsWindowEnabled(h_control); }
		inline void disable() { EnableWindow(h_control, FALSE); }
		inline void enable() { EnableWindow(h_control, TRUE); }
		inline int get_id() const { return (int)GetMenu(h_control); }

		operator bool() { return h_control != NULL; }
		operator HWND() { return h_control; }
		control_handle &operator=(HWND handle) { 
			set_handle(handle);
			return *this;
		}

		bool operator==(control_handle &h_control) { return get_handle() == h_control.get_handle(); }
		bool operator!=(control_handle &h_control) { return get_handle() != h_control.get_handle(); }
	};

	void uncheck_except_me(const control_handle *p_cbs, int num_cb, const control_handle &self, int active_id);

	/* CHECKBOX */
	class checkbox : public control_handle {
	public:
		checkbox();
		checkbox(HWND parent, int id, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex = 0, bool init_state = false);
		checkbox(HWND parent, int id, int padding, const char *p_label, int x, int *p_y, int width, int height, DWORD dw_style_ex = 0, bool init_state = false);
		~checkbox();

		bool is_checked();
		void set_check(bool state);
	};

	/* TOGGLE BUTTON */
	class toggle_button : public control_handle {
	public:
		toggle_button();
		toggle_button(HWND parent, int id, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex = 0, bool init_state = false);
		toggle_button(HWND parent, int id, int padding, const char *p_label, int x, int *p_y, int width, int height, DWORD dw_style_ex = 0, bool init_state = false);
		~toggle_button();

		bool is_checked();
		void set_check(bool state);
	};

	/* STATIC */
	class static_label : public control_handle {
	public:
		static_label();
		static_label(HWND parent, int id, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex = 0);
		static_label(HWND parent, int id, int padding, const char *p_label, int x, int *p_y, int width, int height, DWORD dw_style_ex = 0);
		~static_label();

		void set_text(const char *p_text);
		void set_textf(const char *p_format, ...);
		void get_text(char *p_dst, size_t dstlen);
	};

	/* TRACKBAR */
	class trackbar : public control_handle {
		void baseinit(HWND parent, int id, const char * p_description, int x, int y, int width, int height, int def_text_height, int rmin, int rmax, int pos, DWORD dw_style_ex, DWORD dw_style);
	public:
		trackbar();
		trackbar(HWND parent, int id, const char *p_description, int x, int y, int width, int height, int def_text_height = 15, int rmin = 0, int rmax = 1, int pos = 0, DWORD dw_style_ex = 0, DWORD dw_style = 0);
		trackbar(HWND parent, int id, int padding, const char *p_description, int x, int *p_y, int width, int height, int def_text_height = 15, int rmin = 0, int rmax = 1, int pos = 0, DWORD dw_style_ex = 0, DWORD dw_style = 0);
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
		editbox(HWND parent, int id, const char *p_text, int x, int y, int width, int height, DWORD dw_style_ex = 0, DWORD dw_style = 0);
		editbox(HWND parent, int id, int padding, const char *p_text, int x, int *p_y, int width, int height, DWORD dw_style_ex = 0, DWORD dw_style = 0);
		~editbox();

		void set_text(const char *p_text);
		void get_text(char *p_dst, size_t dstlen);
	};

	class logbox : public control_handle {
		logbox();
		logbox(HWND parent, int id, const char *p_text, int x, int y, int width, int height, DWORD dw_style_ex = 0, DWORD dw_style = 0);
		logbox(HWND parent, int id, int padding, const char *p_text, int x, int *p_y, int width, int height, DWORD dw_style_ex = 0, DWORD dw_style = 0);
		~logbox();
		
		void log_messagef(const char *p_format, ...);
		void clear();
	};

	class treeview : public control_handle {
	public:
		treeview();
		treeview(HWND parent, int id, int x, int y, int width, int height, DWORD dw_style_ex = 0, DWORD dw_style = 0);
		treeview(HWND parent, int id, int x, int *p_y, int width, int height, DWORD dw_style_ex = 0, DWORD dw_style = 0, int padding = 2);
		~treeview();

		HTREEITEM insert_text_item(HTREEITEM h_parent_item, const char *p_name, int value);
		bool expand(HTREEITEM h_item);
		bool collapse(HTREEITEM h_item);
	};
};