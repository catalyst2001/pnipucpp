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

ctls::checkbox::checkbox()
{
}

ctls::checkbox::checkbox(HWND parent, const char * p_label, int x, int y, int width, int height, DWORD dw_style_ex, bool init_state)
{
}

ctls::checkbox::~checkbox()
{
}

bool ctls::checkbox::is_checked()
{
	return false;
}

bool ctls::checkbox::set_check(bool state)
{
	return false;
}

ctls::toggle_button::toggle_button()
{
}

ctls::toggle_button::toggle_button(HWND parent, const char * p_label, int x, int y, int width, int height, DWORD dw_style_ex, bool init_state)
{
}

ctls::toggle_button::~toggle_button()
{
}

bool ctls::toggle_button::is_checked()
{
	return false;
}

bool ctls::toggle_button::set_check(bool state)
{
	return false;
}

ctls::static_label::static_label()
{
}

ctls::static_label::static_label(HWND parent, const char * p_label, int x, int y, int width, int height, DWORD dw_style_ex)
{
}

ctls::static_label::~static_label()
{
}

void ctls::static_label::set_text(const char * p_text)
{
}

void ctls::static_label::set_textf(const char * p_format, ...)
{
}

void ctls::static_label::get_text(char * p_dst, size_t dstlen)
{
}

ctls::trackbar::trackbar()
{
}

ctls::trackbar::trackbar(HWND parent, const char * p_label, int x, int y, int width, int height, int rmin, int rmax, int pos, DWORD dw_style_ex, DWORD dw_style)
{
}

ctls::trackbar::~trackbar()
{
}

void ctls::trackbar::set_minmax(int min, int max)
{
}

int ctls::trackbar::get_min()
{
	return 0;
}

int ctls::trackbar::get_max()
{
	return 0;
}

void ctls::trackbar::set_pos(int pos)
{
}

int ctls::trackbar::get_pos()
{
	return 0;
}

ctls::editbox::editbox()
{
}

ctls::editbox::editbox(HWND parent, const char * p_text, int x, int y, int width, int height, DWORD dw_style_ex, DWORD dw_style)
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
