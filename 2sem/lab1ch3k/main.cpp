#include <stdio.h>

extern "C" {
#include "gldl.h"
};

gldl_dt_t *p_window;

int main()
{
	gldl_events_dt_t events;
	memset(&events, NULL, sizeof(events));
	events.p_window_resize_event = [](int width, int height) {
		int w = p_window->GetWindowWidth();
		int h = p_window->GetWindowHeight();
		glMatrixMode(GL_PROJECTION);
		glViewport(0, 0, w, h);
		glOrtho((double)0, (double)w, (double)h, (double)0, -1.0, 1.0);
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

	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	while (p_window->IsWindowOpened()) {
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		
		glPushAttrib(GL_CURRENT_BIT);
		glColor4ub(0, 0, 0, 255);
		glLineWidth(2.5f);
		p_window->Draw2DCircle(p_window->GetWindowWidth() >> 1, p_window->GetWindowHeight() >> 1, UNFILLED, 100, 200);
		glColor4ub(0, 0, 0, 128);
		glLineWidth(4.5f);
		p_window->Draw2DCircle(p_window->GetWindowWidth() >> 1, p_window->GetWindowHeight() >> 1, UNFILLED, 100, 201);
		

		glPointSize(2.0f);
		glBegin(GL_POINT);
		glVertex2i(500, 500);
		glEnd();
		glPointSize(1.0f);

		glPopAttrib();
	}
	gldl_shutdown(p_window);
	return 0;
}