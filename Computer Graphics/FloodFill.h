#pragma once
#include"Bresenham.h"

int dx[4] = {0,0,1,-1};
int dy[4] = {1,-1,0,0};
GLint startx, starty;
bool isEnough = false;
bool isFirstFF = true;
int inputEdgeNum;
GLint cnt = 0;
void drawPixelFF(int x, int y)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(2.0);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

void floodFill(int x, int y)
{
	queue<pair<int, int> >q;
	q.push(make_pair(x,y));
	vis[x][y] = 1;
	while (!q.empty())
	{
		pair<int,int> tmp = q.front();
		if(!q.empty())q.pop();
		for (int i = 0; i <= 3; i++)
		{
			int nx = tmp.first + dx[i];
			int ny = tmp.second + dy[i];
			if (vis[nx][ny]==0)
			{
				pair<int, int> next;
				next.first = nx;
				next.second = ny;
				q.push(next);
				drawPixelFF(nx, ny);
				vis[nx][ny] = 1;
			}
		}
	}
}
void mouseHitFF(int button, int state, int x, int y)
{
	
	if (state == GLUT_DOWN)
	{

		if (button == GLUT_LEFT_BUTTON)
		{
			y = HEIGHT - y;
			drawPixel(x, y);
			if (cnt == inputEdgeNum) isEnough = true;
			if (isFirstFF)
			{
				hit_pos_x0 = x;
				hit_pos_y0 = y;
				startx = x;
				starty = y;
				isFirstFF = false;
				cnt++;
			}
			else if(!isEnough)
			{
				hit_pos_x1 = x, hit_pos_y1 = y;
				cnt++;
				if (cnt == inputEdgeNum) isEnough = true;
				integerBresenhamLine(hit_pos_x0, hit_pos_y0, hit_pos_x1, hit_pos_y1);
				hit_pos_x0 = hit_pos_x1, hit_pos_y0 = hit_pos_y1;
			}
			if(isEnough)integerBresenhamLine(hit_pos_x0, hit_pos_y0, startx, starty);
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			y = HEIGHT - y;
			drawPixelFF(x, y);
			floodFill(x, y);
		}
	}
}



void displayFloodFill()
{
	//glViewport(0, 0, 500, 500);
}






