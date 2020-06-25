#pragma once
#include"Bresenham.h"
#define XL 100
#define XR 700
#define YT 600
#define YB 200
Point vertexSH[500];
Point vertexSHTmp[500];
GLint cntSH = 0;
GLint inputEdgeNumSH = 0;
bool isEnoughSH = false;
bool isFirstSH = true;
bool displayFlag = true;
void displaySuthlandHodgman()
{
	if (displayFlag)
	{
		integerBresenhamLine(XL, YT, XR, YT);
		integerBresenhamLine(XL, YB, XR, YB);
		integerBresenhamLine(XL, YB, XL, YT);
		integerBresenhamLine(XR, YB, XR, YT);
		displayFlag = false;
	}
}
bool inside(int edge,Point p)
{
	if (edge == 1 && p.x < 100 || edge == 3 && p.x > 700 || edge == 2 && p.y < 200 || edge == 4 && p.y>600)
	{
		return false;
	}
	return true;
}
Point getCrossPoint(int edge, Point n1, Point n2)
{
	Point p;
	if (edge == 1)//вС
	{
		p.x = 100;
		p.y = (int)(n1.y - (float)(n1.y - n2.y) * (n1.x - 100) / (n1.x - n2.x));
	}
	else if (edge == 3)//ср
	{
		p.x = 700;
		p.y = (int)(n1.y - (float)(n1.y - n2.y) * (n1.x - 700) / (n1.x - n2.x));
	}
	else if (edge == 2)//об
	{
		p.y = 200;
		p.x = (int)(n1.x - (float)(n1.x - n2.x) * (n1.y - 200) / (n1.y - n2.y));
	}
	else if (edge == 4)//ио
	{
		p.y = 600;
		p.x = (int)(n1.x - (float)(n1.x - n2.x) * (n1.y - 600) / (n1.y - n2.y));
	}
	return p;
}
void clipEdge(int edge)
{
	int indexOfVertexTemp = 0;
	bool isP1In, isP2In;
	for (int i = 0; i < cntSH; i++)
	{
		isP1In = inside(edge, vertexSH[i]);
		isP2In = inside(edge, vertexSH[(i + 1) % cntSH]);
		if (isP1In && isP2In)
			vertexSHTmp[indexOfVertexTemp++] = vertexSH[(i + 1) % cntSH];
		else if (isP1In)
			vertexSHTmp[indexOfVertexTemp++] = getCrossPoint(edge, vertexSH[i], vertexSH[(i + 1) % cntSH]);
		else if (isP2In)
		{
			vertexSHTmp[indexOfVertexTemp++] = getCrossPoint(edge, vertexSH[i], vertexSH[(i + 1) % cntSH]);
			vertexSHTmp[indexOfVertexTemp++] = vertexSH[(i + 1) % cntSH];
		}
	}
	cntSH = indexOfVertexTemp;
	for (int i = 0; i < cntSH; i++)
	{
		vertexSH[i] = vertexSHTmp[i];
	}
		
}
void sutherlandHodgman()
{
	clipEdge(1);
	clipEdge(4);
	clipEdge(3);
	clipEdge(2);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < cntSH; i++)
	{
		integerBresenhamLineSH(vertexSH[i].x, vertexSH[i].y, vertexSH[(i + 1) % cntSH].x, vertexSH[(i + 1) % cntSH].y);
	}
}

void mouseHitSH(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			y = HEIGHT - y;
			drawPixel(x, y);
			if (cntSH == inputEdgeNumSH) isEnoughSH = true;
			if (isFirstSH)
			{
				vertexSH[0].x = x, vertexSH[0].y = y;
				isFirstSH = false;
				cntSH++;
			}
			else if (!isEnoughSH)
			{
				vertexSH[cntSH].x = x, vertexSH[cntSH].y = y;
				integerBresenhamLine(vertexSH[cntSH - 1].x, vertexSH[cntSH - 1].y, vertexSH[cntSH].x, vertexSH[cntSH].y);
				cntSH++;
				if (cntSH == inputEdgeNumSH)
				{
					isEnoughSH = true;
					integerBresenhamLine(vertexSH[cntSH - 1].x, vertexSH[cntSH - 1].y, vertexSH[0].x, vertexSH[0].y);
				}
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			sutherlandHodgman();
		}
	}
}
