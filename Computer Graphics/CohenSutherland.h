#pragma once
#include"Bresenham.h"
#define INNER 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define LEFT_RIGHT 3
#define LEFT_BOTTOM 5
#define RIGHT_BOTTOM 6
Point vertex[3];
Point line[2];
GLint cntCS = 0;
bool isEnoughCS = false;
bool isFirstCS = true;
bool startLine = false;
bool isLeftPoint(Point s, Point e, Point p)
{
	if(int(((s.x - p.x) * (e.y - p.y)) - ((e.x - p.x) * (s.y - p.y))) <= 0)
	{
		return true;
	}
	return false;
}
int getArea(double x, double y)
{
	Point tmp(x, y);
	if (isLeftPoint(vertex[2], vertex[0], tmp) && isLeftPoint(vertex[1], vertex[2], tmp) && isLeftPoint(vertex[0], vertex[1], tmp))
	{
		return INNER;
	}
	else if (!isLeftPoint(vertex[0], vertex[1], tmp) && !isLeftPoint(vertex[2], vertex[0], tmp)&&isLeftPoint(vertex[1], vertex[2], tmp)) return LEFT_RIGHT;
	else if (!isLeftPoint(vertex[2], vertex[0], tmp) && isLeftPoint(vertex[1], vertex[2], tmp) && isLeftPoint(vertex[0], vertex[1], tmp))
	{
		return LEFT;
	}
	else if (!isLeftPoint(vertex[0], vertex[1], tmp) && isLeftPoint(vertex[1], vertex[2], tmp) && isLeftPoint(vertex[2], vertex[0], tmp))
	{
		return RIGHT;
	}
	else if (!isLeftPoint(vertex[2], vertex[0], tmp) && !isLeftPoint(vertex[1], vertex[2], tmp)&& isLeftPoint(vertex[0], vertex[1], tmp)) return LEFT_BOTTOM;
	else if (!isLeftPoint(vertex[0], vertex[1], tmp) && !isLeftPoint(vertex[1], vertex[2], tmp)&& isLeftPoint(vertex[2], vertex[0], tmp)) return RIGHT_BOTTOM;
	else if (isLeftPoint(vertex[0], vertex[1], tmp) && !isLeftPoint(vertex[1], vertex[2], tmp) && isLeftPoint(vertex[2], vertex[0], tmp))
	{
		return BOTTOM;
	}

}
int encode(float x, float y)
{
	int c = 0;
	if (getArea(x, y) == INNER)
	{
		return 0;
	}
	if (getArea(x, y) == LEFT|| getArea(x, y) == LEFT_BOTTOM||getArea(x,y)==LEFT_RIGHT)
	{
		c |= LEFT;
	}
	if (getArea(x, y) == RIGHT || getArea(x, y) == RIGHT_BOTTOM || getArea(x, y) == LEFT_RIGHT)
	{
		c |= RIGHT;
	}
	if (getArea(x, y) == BOTTOM|| getArea(x, y) == LEFT_BOTTOM || getArea(x, y) == RIGHT_BOTTOM)
	{
		c |= BOTTOM;
	}
	return c;
}

Point IntersectPoint(Point s, Point e, Point n1, Point n2)
{
	double a0 = s.y - e.y;
	double b0 = e.x - s.x;
	double c0 = s.x * e.y - s.y * e.x;
	double a1 = n1.y - n2.y;
	double b1 = n2.x - n1.x;
	double c1 = n1.x*n2.y - n1.y*n2.x;
	double d = a0 * b1 - a1 * b0;
	double x = (b0 * c1 - b1 * c0)*1.0 / d;
	double y = (c0 * a1 - c1 * a0)*1.0 / d;
	return Point(x, y);
}
void displayCohenSutherland(){}
void cohenSutherland(double x1,double y1,double x2,double y2)
{
	int code1, code2, code;
	double nx1 = x1, ny1 = y1, nx2 = x2, ny2 = y2;
	code1 = encode(nx1, ny1);
	code2 = encode(nx2, ny2);
	while (code1 != 0 || code2 != 0)
	{
		if ((code1 & code2) != 0)
		{
			return;
		}
		if (code1 != 0) code = code1;
		else code = code2;
		Point tmp;
		if ((LEFT & code) != 0)
		{
			tmp = IntersectPoint(vertex[2],vertex[0], Point(nx1, ny1), Point(nx2, ny2));
		}
		else if ((RIGHT & code) != 0)
		{
			tmp = IntersectPoint(vertex[1], vertex[0], Point(nx1, ny1), Point(nx2, ny2));
		}
		else if ((BOTTOM & code) != 0)
		{
			tmp = IntersectPoint(vertex[2], vertex[1], Point(nx1,ny1), Point(nx2,ny2));
		}
		if(code == code1)
		{
			nx1 = tmp.x;
			ny1 = tmp.y;
			code1 = encode(nx1, ny1);
		}
		else
		{
			nx2 = tmp.x;
			ny2 = tmp.y;
			code2 = encode(nx2, ny2);
		}
	}
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	integerBresenhamLine(vertex[0].x,vertex[0].y, vertex[1].x, vertex[1].y);
	integerBresenhamLine(vertex[1].x, vertex[1].y, vertex[2].x, vertex[2].y);
	integerBresenhamLine(vertex[2].x, vertex[2].y, vertex[0].x, vertex[0].y);
	integerBresenhamLine(nx1, ny1, nx2, ny2);
}
void mouseHitCS(int button, int state, int x, int y)
{
	int inputEdgeNum = 3;
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if (startLine == false)
			{
				y = HEIGHT - y;
				drawPixel(x, y);
				if (cntCS == inputEdgeNum) isEnoughCS = true;
				if (isFirstCS)
				{
					hit_pos_x0 = x;
					hit_pos_y0 = y;
					vertex[0].x = x;
					vertex[0].y = y;
					isFirstCS = false;
					cntCS++;
				}
				else if (!isEnoughCS)
				{
					hit_pos_x1 = x, hit_pos_y1 = y;
					vertex[cntCS].x = x;
					vertex[cntCS].y = y;
					cntCS++;
					if (cntCS == inputEdgeNum) isEnoughCS = true;
					integerBresenhamLine(hit_pos_x0, hit_pos_y0, hit_pos_x1, hit_pos_y1);
					hit_pos_x0 = hit_pos_x1, hit_pos_y0 = hit_pos_y1;
				}
				if (isEnoughCS)
				{
					integerBresenhamLine(hit_pos_x0, hit_pos_y0, vertex[0].x, vertex[0].y);
					startLine = true;
					isFirstCS = true;
				}
			}
			else
			{
				y = HEIGHT - y;
				drawPixel(x, y);
				if (isFirst)
				{
					isFirst = false;
					line[0].x = x, line[0].y = y;
				}
				else
				{

					line[1].x = x, line[1].y = y;
					integerBresenhamLine(line[0].x, line[0].y,line[1].x,line[1].y);
					isFirst = true;
				}
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			cohenSutherland(line[0].x, line[0].y, line[1].x, line[1].y);
		}
	}
}


