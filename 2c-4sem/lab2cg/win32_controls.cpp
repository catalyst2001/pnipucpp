#include "win32_controls.h"

HFONT h_global_font;

void ctls::set_default_font()
{
	h_global_font = (HFONT)GetStockObject(ANSI_VAR_FONT);
}

void ctls::set_font(HFONT h_font)
{
	h_global_font = h_font;
}

HFONT ctls::get_font()
{
	return h_global_font;
}

void ctls::uncheck_except_me(const control_handle *p_cbs, int num_cb, const control_handle &self, int active_id)
{
	for (int i = 0; i < num_cb; i++) {
		if (p_cbs[i].get_handle() != self.get_handle()) {
			if (SendMessageA(p_cbs[i].get_handle(), BM_GETCHECK, (WPARAM)0, (LPARAM)0) == BST_CHECKED) {
				SendMessageA(p_cbs[i].get_handle(), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
			}
		}
	}
}

ctls::checkbox::checkbox()
{
}

ctls::checkbox::checkbox(HWND parent, int id, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex, bool init_state)
{
	init_handle(CreateWindowExA(0, WC_BUTTONA, p_label, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, x, y, width, height, parent, (HMENU)id, NULL, NULL));
	SendMessageA(get_handle(), BM_SETCHECK, (WPARAM)(init_state) ? BST_CHECKED : BST_UNCHECKED, (LPARAM)0);
}

ctls::checkbox::~checkbox()
{
}

bool ctls::checkbox::is_checked()
{
	return SendMessageA(get_handle(), BM_GETCHECK, (WPARAM)0, (LPARAM)0) == BST_CHECKED;
}

void ctls::checkbox::set_check(bool state)
{
	SendMessageA(get_handle(), BM_SETCHECK, (WPARAM)(state) ? BST_CHECKED : BST_UNCHECKED, (LPARAM)0);
}

ctls::toggle_button::toggle_button()
{
}

ctls::toggle_button::toggle_button(HWND parent, int id, const char * p_label, int x, int y, int width, int height, DWORD dw_style_ex, bool init_state)
{
	init_handle(CreateWindowExA(0, WC_BUTTONA, p_label, WS_VISIBLE|WS_CHILD|BS_AUTOCHECKBOX|BS_PUSHLIKE, x, y, width, height, parent, (HMENU)id, NULL, NULL));
	SendMessageA(get_handle(), BM_SETCHECK, (WPARAM)(init_state) ? BST_CHECKED : BST_UNCHECKED, (LPARAM)0);
}

ctls::toggle_button::~toggle_button()
{
}

bool ctls::toggle_button::is_checked()
{
	return SendMessageA(get_handle(), BM_GETCHECK, (WPARAM)0, (LPARAM)0) == BST_CHECKED;
}

void ctls::toggle_button::set_check(bool state)
{
	SendMessageA(get_handle(), BM_SETCHECK, (WPARAM)(state) ? BST_CHECKED : BST_UNCHECKED, (LPARAM)0);
}

ctls::static_label::static_label()
{
}

ctls::static_label::static_label(HWND parent, int id, const char * p_label, int x, int y, int width, int height, DWORD dw_style_ex)
{
}

ctls::static_label::~static_label()
{
}

void ctls::static_label::set_text(const char *p_text)
{
}

void ctls::static_label::set_textf(const char *p_format, ...)
{
}

void ctls::static_label::get_text(char *p_dst, size_t dstlen)
{
}

ctls::trackbar::trackbar()
{
}

ctls::trackbar::trackbar(HWND parent, int id, const char *p_description, int x, int y, int width, int height, int def_text_height, int rmin, int rmax, int pos, DWORD dw_style_ex, DWORD dw_style)
{
	RECT track_rect;
	HWND h_static_description = NULL;
	track_rect.left = x;
	track_rect.top = y;
	track_rect.right = width;
	track_rect.bottom = height;
	if (p_description) {
		h_static_description = CreateWindowExA(0, WC_STATICA, p_description, WS_CHILD | WS_VISIBLE,
			track_rect.left, track_rect.top,
			track_rect.right, def_text_height, parent, (HMENU)0, NULL, NULL);

		assert(h_static_description);
		SendMessageA(h_static_description, WM_SETFONT, (WPARAM)h_global_font, (LPARAM)TRUE);
	}

	init_handle(CreateWindowExA(0, TRACKBAR_CLASSA, "", WS_CHILD | WS_VISIBLE, track_rect.left, track_rect.top + def_text_height,
		track_rect.right, (track_rect.bottom - def_text_height), parent, (HMENU)id, NULL, NULL));

	if(h_static_description)
		SetWindowLongPtrA(get_handle(), GWL_USERDATA, (LONG_PTR)h_static_description);
}

ctls::trackbar::~trackbar()
{
}

void ctls::trackbar::set_minmax(int _min, int _max)
{
	SendMessageA(get_handle(), TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)_min);
	SendMessageA(get_handle(), TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)_max);
}

int ctls::trackbar::get_min()
{
	return SendMessageA(get_handle(), TBM_GETRANGEMIN, (WPARAM)0, (LPARAM)0);
}

int ctls::trackbar::get_max()
{
	return SendMessageA(get_handle(), TBM_GETRANGEMAX, (WPARAM)0, (LPARAM)0);
}

void ctls::trackbar::set_pos(int pos)
{
	SendMessageA(get_handle(), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)pos);
}

int ctls::trackbar::get_pos()
{
	return SendMessageA(get_handle(), TBM_GETPOS, (WPARAM)0, (LPARAM)0);
}

ctls::editbox::editbox()
{
}

ctls::editbox::editbox(HWND parent, int id, const char *p_text, int x, int y, int width, int height, DWORD dw_style_ex, DWORD dw_style)
{
}

ctls::editbox::~editbox()
{
}

void ctls::editbox::set_text(const char *p_text)
{
}

void ctls::editbox::get_text(char *p_dst, size_t dstlen)
{
}

ctls::logbox::~logbox()
{
}

void ctls::logbox::log_messagef(const char *p_format, ...)
{
}

void ctls::logbox::clear()
{
}

inline void ctls::control_handle::init_handle(HWND handle)
{
	if (handle)
		SendMessageA(handle, WM_SETFONT, (WPARAM)h_global_font, (LPARAM)TRUE);
	
	set_handle(handle);
}

ctls::treeview::treeview()
{
}

ctls::treeview::treeview(HWND parent, int id, int x, int y, int width, int height, DWORD dw_style_ex, DWORD dw_style)
{
	init_handle(CreateWindowExA(dw_style_ex, WC_TREEVIEWA, "", WS_CHILD|WS_VISIBLE|WS_BORDER|TVS_HASLINES, x, y, width, height, parent, (HMENU)id, NULL, NULL));
}

ctls::treeview::~treeview()
{
}

HTREEITEM ctls::treeview::insert_text_item(HTREEITEM h_parent_item, const char *p_name, int value)
{
	/* TVI_FIRST, TVI_ROOT, TVI_LAST */
	TVINSERTSTRUCTA tvins;
	tvins.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvins.item.pszText = _strdup(p_name);
	tvins.item.cchTextMax = sizeof(tvins.item.pszText) / sizeof(tvins.item.pszText[0]);
	tvins.item.lParam = value;
	if (!h_parent_item) {
		tvins.hInsertAfter = TVI_ROOT;
		tvins.hParent = NULL;
	}
	else {
		tvins.hInsertAfter = TVI_LAST;
		tvins.hParent = h_parent_item;
	}
	return (HTREEITEM)SendMessageA(get_handle(), TVM_INSERTITEMA, (WPARAM)0, (LPARAM)&tvins);
}
