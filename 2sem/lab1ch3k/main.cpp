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
		glOrtho((double)0, (double)w, (double)0, (double)h, -1.0, 1.0);
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

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	while (p_window->IsWindowOpened()) {
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		
		glPushAttrib(GL_CURRENT_BIT);
		glColor3ub(0, 0, 0);
		glLineWidth(1.5f);
		//p_window->Draw2DCircle(p_window->GetWindowWidth() >> 1, p_window->GetWindowHeight() >> 1, UNFILLED, 100, 200);

		p_window->PrintText(200, 200, "1234567890");

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