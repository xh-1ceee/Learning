#pragma once
#include"GlobalV.h"

GLint hit_pos_x0 = 0, hit_pos_y0 = 0;
GLint hit_pos_x1 = 0, hit_pos_y1 = 0;
bool isFirst = true;

using namespace std;

map<int, map<int, int> >vis;
map<int, map<int, int> >visSH;

void drawPixel(int x, int y)
{
	glColor3f(0.0f, 1.0f, 1.0f);
	glPointSize(1.0);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	vis[x][y] = 1;
	glEnd();
	glFlush();
}
void drawNewPixel(int x, int y)
{
	
	glPointSize(1.0);
	glBegin(GL_POINTS);
	visSH[x][y]++;
	if (visSH[x][y] % 2 != 0)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2i(x, y);
	}
	else
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2i(x, y);
	}
	glEnd();
	glFlush();
}
void swap(int& a, int& b)
{
	int t = a;
	a = b;
	b = t;
}
void integerBresenhamLine(int x0, int y0, int x1, int y1)
{
	int x, y, dx, dy, e;
	dx = abs(x1 - x0), dy = abs(y1 - y0);
	x = x0, y = y0;
	int swapFlag = 0;
	if (dx < dy) swap(dx, dy), swapFlag = 1;
	e = -dx;
	for (int i = 0; i <= dx; i++)
	{
		drawPixel(x, y);
		if (swapFlag == 0 && x1 > x0)x++;
		else if (swapFlag == 0 && x1 <= x0) x--;
		else if (swapFlag == 1 && y1 > y0) y++;
		else if (swapFlag == 1 && y1 <= y0) y--;
		e += 2 * dy;
		if (e >= 0)
		{
			if (swapFlag == 0 && y1 > y0)y++;
			else if (swapFlag == 0 && y1 <= y0) y--;
			else if (swapFlag == 1 && x1 > x0) x++;
			else if (swapFlag == 1 && x1 <= x0) x--;
			e -= 2 * dx;
		}
	}
}
void integerBresenhamLineSH(int x0, int y0, int x1, int y1)
{
	int x, y, dx, dy, e;
	dx = abs(x1 - x0), dy = abs(y1 - y0);
	x = x0, y = y0;
	int swapFlag = 0;
	if (dx < dy) swap(dx, dy), swapFlag = 1;
	e = -dx;
	for (int i = 0; i <= dx; i++)
	{
		drawNewPixel(x, y);
		if (swapFlag == 0 && x1 > x0)x++;
		else if (swapFlag == 0 && x1 <= x0) x--;
		else if (swapFlag == 1 && y1 > y0) y++;
		else if (swapFlag == 1 && y1 <= y0) y--;
		e += 2 * dy;
		if (e >= 0)
		{
			if (swapFlag == 0 && y1 > y0)y++;
			else if (swapFlag == 0 && y1 <= y0) y--;
			else if (swapFlag == 1 && x1 > x0) x++;
			else if (swapFlag == 1 && x1 <= x0) x--;
			e -= 2 * dx;
		}
	}
}
void midCircle(int x0, int y0)
{
	int x, y;
	int r = 100;
	GLfloat d;
	x = 0, y = r, d = 1.25 - r;
	drawPixel(x, y);
	while (x <= y)
	{
		if (d < 0)
			d += 2 * x + 3;
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		drawPixel(x + x0, y + y0);
		drawPixel(y + y0, x + x0);
		drawPixel(x0 - x, y + y0);
		drawPixel(y + y0, x0 - x);
		drawPixel(x + x0, y0 - y);
		drawPixel(y0 - y, x + x0);
		drawPixel(x0 - x, y0 - y);
		drawPixel(y0 - y, x0 - x);
	}
}

void displayCircle()
{
	int x, y;
	cout << "Please gives the x and y value of origin point" << endl;
	cout << "with one space between them (like: 'x y')" << endl;
	cin >> x >> y;
	glClear(GL_COLOR_BUFFER_BIT);
	midCircle(x, y);
	glEnd();
	glFlush();
}

void mouseHitBH(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			y = HEIGHT - y;
			drawPixel(x, y);
			if (isFirst)
			{
				isFirst = false;
				hit_pos_x0 = x, hit_pos_y0 = y;
			}
			else
			{

				hit_pos_x1 = x, hit_pos_y1 = y;
				integerBresenhamLine(hit_pos_x0, hit_pos_y0, hit_pos_x1, hit_pos_y1);
				isFirst = true;
			}
		}
	}
}

void displayBresenham() {}