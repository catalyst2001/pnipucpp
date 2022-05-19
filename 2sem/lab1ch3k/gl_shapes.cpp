#include "gl_shapes.h"

void draw_line(int fromx, int fromy, int tox, int toy)
{
	glBegin(GL_LINES);
	glVertex2i(fromx, fromy);
	glVertex2i(tox, toy);
	glEnd();
}

void draw_circle(int x, int y, int mode, int nsegs, int radius)
{
	glBegin((!mode) ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i <= nsegs; i++) {
		float angle = 2 * FLOAT_PI * i / (float)nsegs;
		float xofs = cos(angle) * (double)radius;
		float yofs = sin(angle) * (double)radius;
		glVertex2i(x + (int)xofs, y + (int)yofs);
	}
	glEnd();
}
