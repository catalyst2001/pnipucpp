#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

#define FLOAT_PI 3.1415926535897932384f

#define FILLED 0   //���������
#define UNFILLED 1 //�� ���������

void draw_circle(int x, int y, int mode, int nsegs, int radius);
void draw_line(int fromx, int fromy, int tox, int toy);