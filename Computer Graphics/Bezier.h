#pragma once
#include"GlobalV.h"
#include"Bresenham.h"
#define STEP 0.01
GLint inputVertexNum = 0;
GLint tmpV = -1;
Point v[500];
vector<Point> res;
Point p[1000][1000];
void drawLine(Point p1, Point p2) 
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
	glFlush();
}
void Bezier()
{
	memset(p, 0, sizeof(p));
	for (int i = 0; i <= tmpV; i++)
	{
		p[0][i] = v[i];
	}
	for (double t = 0; t <= 1; t += STEP)
	{
		for (int i = 1; i <= tmpV; i++)
		{
			for (int j = 0; j <= tmpV - i; j++)
			{
				p[i][j].x = (1 - t) * p[i - 1][j].x + t * p[i - 1][j + 1].x;
				p[i][j].y = (1 - t) * p[i - 1][j].y + t * p[i - 1][j + 1].y;
				
			}	
		}
		res.push_back(p[tmpV][0]);
	}
	for (int i = 0; i < res.size()-1; i++)
	{
		drawLine(res[i], res[i + 1]);
	}
}
void displayBezier(){}
void mouseHitBZ(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			y = HEIGHT - y;
			drawPixel(x, y);
			if (tmpV == inputVertexNum-1) isEnough = true;
			else if (!isEnough)
			{
				
				tmpV++;
				v[tmpV].x = x, v[tmpV].y = y;
				if (tmpV >= 1)integerBresenhamLine(v[tmpV - 1].x, v[tmpV - 1].y, v[tmpV].x, v[tmpV].y);
				if (tmpV == inputVertexNum)
				{
					isEnough = true;
				}
			}

		}
		else if (button == GLUT_RIGHT_BUTTON && tmpV == inputVertexNum-1)
		{
			Bezier();
		}
		else if (button == GLUT_MIDDLE_BUTTON)
		{
			tmpV = -1;
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
}
