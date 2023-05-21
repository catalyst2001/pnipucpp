#include <stdio.h>
#include "win32_controls.h"
#include "dbg.h"

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

int ctls::dialog_open_file(char *p_dst_path, int maxlen, HWND parent, const char *p_initdir, const char *p_filter)
{
	OPENFILENAMEA ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parent;
	ofn.hInstance = (HINSTANCE)GetModuleHandleW(NULL);
	ofn.lpstrFilter = p_filter;
	ofn.lpstrFile = p_dst_path;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = maxlen;
	ofn.lpstrInitialDir = p_initdir;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	if(!GetOpenFileNameA(&ofn)) {
		if (CommDlgExtendedError() != 0)
			return FD_STATUS_ERROR;

		return FD_STATUS_WINDOW_CLOSED_WITHOUT_FILE_SELECT;
	}
	return FD_STATUS_OK;
}

int ctls::dialog_save_file(char *p_dst_path, int maxlen, HWND parent, const char *p_initdir, const char *p_filter)
{
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parent;
	ofn.hInstance = (HINSTANCE)GetModuleHandleW(NULL);
	ofn.lpstrFilter = p_filter;
	ofn.lpstrFile = p_dst_path;
	ofn.nMaxFile = maxlen;
	ofn.lpstrInitialDir = p_initdir;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	DWORD error;
	if (!GetSaveFileNameA(&ofn)) {
		error = CommDlgExtendedError();
		if (error != 0) {
			DBG("CommDlgExtendedError: %d (0x%x)", error, error);
			return FD_STATUS_ERROR;
		}

		return FD_STATUS_WINDOW_CLOSED_WITHOUT_FILE_SELECT;
	}
	return FD_STATUS_OK;
}


ctls::checkbox::checkbox()
{
}

ctls::checkbox::checkbox(HWND parent, int id, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex, bool init_state)
{
	init_handle(CreateWindowExA(0, WC_BUTTONA, p_label, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, x, y, width, height, parent, (HMENU)id, NULL, NULL));
	SendMessageA(get_handle(), BM_SETCHECK, (WPARAM)(init_state) ? BST_CHECKED : BST_UNCHECKED, (LPARAM)0);

	DBG("-------------------\n");
}

ctls::checkbox::checkbox(HWND parent, int id, int padding, const char *p_label, int x, int *p_y, int width, int height, DWORD dw_style_ex, bool init_state) : control_handle()
{
	//DBG("PRE 0x%x\n", get_handle());
	*this = checkbox(parent, id, p_label, x, *p_y, width, height, dw_style_ex, init_state);
	//DBG("POST 0x%x\n", get_handle());
	(*p_y) += height + padding;
}

ctls::checkbox::~checkbox()
{
}

bool ctls::checkbox::is_checked()
{
	LRESULT res = SendMessageA(get_handle(), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
	//DBG("state: %d\n", res);
	return res == BST_CHECKED;
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

ctls::toggle_button::toggle_button(HWND parent, int id, int padding, const char *p_label, int x, int *p_y, int width, int height, DWORD dw_style_ex, bool init_state)
{
	*this = toggle_button(parent, id, p_label, x, *p_y, width, height, dw_style_ex, init_state);
	*p_y += height + padding;
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

ctls::static_label::static_label(HWND parent, int id, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex)
{
}

ctls::static_label::static_label(HWND parent, int id, int padding, const char *p_label, int x, int *p_y, int width, int height, DWORD dw_style_ex)
{
	static_label(parent, id, p_label, x, *p_y, width, height, dw_style_ex);
	*p_y += height + padding;
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

	if (!dw_style)
		dw_style = (WS_CHILD | WS_VISIBLE);

	init_handle(CreateWindowExA(dw_style_ex, TRACKBAR_CLASSA, "", dw_style, track_rect.left, track_rect.top + def_text_height,
		track_rect.right, (track_rect.bottom - def_text_height), parent, (HMENU)id, NULL, NULL));

	if(h_static_description)
		SetWindowLongPtrA(get_handle(), GWL_USERDATA, (LONG_PTR)h_static_description);
}

ctls::trackbar::trackbar(HWND parent, int id, int padding, const char *p_description, int x, int *p_y, int width, int height, int def_text_height, int rmin, int rmax, int pos, DWORD dw_style_ex, DWORD dw_style)
{
	*this = trackbar(parent, id, p_description, x, *p_y, width, height, def_text_height, rmin, rmax, pos, dw_style_ex, dw_style);
	*p_y += height + padding;
}

ctls::trackbar::~trackbar()
{
}

void ctls::trackbar::set_min(int n)
{
	SendMessageA(get_handle(), TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)n);
}

void ctls::trackbar::set_max(int n)
{
	SendMessageA(get_handle(), TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)n);
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

void ctls::trackbar::set_tick_freq(int n)
{
	SendMessageA(get_handle(), TBM_SETTICFREQ, (WPARAM)n, (LPARAM)0);
}

ctls::editbox::editbox()
{
}

ctls::editbox::editbox(HWND parent, int id, const char *p_text, int x, int y, int width, int height, DWORD dw_style_ex, DWORD dw_style)
{
}

ctls::editbox::editbox(HWND parent, int id, int padding, const char *p_text, int x, int *p_y, int width, int height, DWORD dw_style_ex, DWORD dw_style)
{
	*this = editbox(parent, id, p_text, x, *p_y, width, height, dw_style, dw_style_ex);
	*p_y += height + padding;
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

ctls::logbox::logbox()
{

}

ctls::logbox::logbox(HWND parent, int id, const char *p_text, int x, int y, int width, int height, DWORD dw_style_ex, DWORD dw_style)
{
}

ctls::logbox::logbox(HWND parent, int id, int padding, const char *p_text, int x, int *p_y, int width, int height, DWORD dw_style_ex, DWORD dw_style)
{
	*this = logbox(parent, id, p_text, x, *p_y, width, height, dw_style_ex, dw_style);
	*p_y += height + padding;
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

ctls::treeview::treeview(HWND parent, int id, int x, int *p_y, int width, int height, DWORD dw_style_ex, DWORD dw_style, int padding)
{
	*this = treeview(parent, id, x, *p_y, width, height, dw_style_ex, dw_style);
	*p_y += height + padding;
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

bool ctls::treeview::expand(HTREEITEM h_item)
{
	return !SendMessageA(get_handle(), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)h_item);
}

bool ctls::treeview::collapse(HTREEITEM h_item)
{
	return !SendMessageA(get_handle(), TVM_EXPAND, (WPARAM)TVE_COLLAPSE, (LPARAM)h_item);
}

ctls::button::button()
{
}

ctls::button::button(HWND parent, int id, const char *p_label, int x, int y, int width, int height, DWORD dw_style_ex)
{
	init_handle(CreateWindowExA(0, WC_BUTTONA, p_label, WS_VISIBLE|WS_CHILD, x, y, width, height, parent, (HMENU)id, NULL, NULL));
}

ctls::button::button(HWND parent, int id, int padding, const char * p_label, int x, int * p_y, int width, int height, DWORD dw_style_ex)
{
	*this = button(parent, id, p_label, x, *p_y, width, height, dw_style_ex);
	*p_y += height + padding;
}

ctls::button::~button()
{
}
