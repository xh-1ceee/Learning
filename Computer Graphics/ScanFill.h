#pragma once
#include"Bresenham.h"
bool isFirstSF = true;
GLint k = 0;


typedef struct edgeTable {
	float x;
	float dx;
	float ymax;
	edgeTable* next;
}AET, NET;
void drawLine(int x0, int x1, int y)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_LINES);
	/*for (int i = x0; i <= x1; i++)
	{
		glColor3f(0.0f,i/(x1 - x0), i / (x1 - x0));
		glVertex2f(i, y);
	}*/

	glColor3f(0.0f, 0.9f, 1.0f);
	glVertex2f(x0, y);
	glColor3f(0.5f, 1.0f, 0.0f);
	glVertex2f(x1, y);
	glEnd();
	glFlush();
}
void scanFill()
{
	GLint MAX_Y = -1024, MIN_Y = 1024;
	for (int i = 0; i < inputEdgeNum; i++)
	{
		MAX_Y = (MAX_Y < pos[i].y) ? pos[i].y : MAX_Y;
		MIN_Y = (MIN_Y > pos[i].y) ? pos[i].y : MIN_Y;
	}
	
	NET* net[1024];
	for (int i = MIN_Y; i <= MAX_Y; i++)
	{
		net[i] = new NET;
		net[i]->next = NULL;
	}
	//建立NET
	
	for (int i = 0; i < k; i++)
	{
		GLint ymin = (edges[i].s.y < edges[i].e.y) ? edges[i].s.y : edges[i].e.y;
		GLint ymax = (edges[i].s.y < edges[i].e.y) ? edges[i].e.y : edges[i].s.y;
		NET* tmp = new NET;
		tmp->x = (edges[i].s.y < edges[i].e.y) ? edges[i].s.x : edges[i].e.x;
		tmp->dx = (float) (edges[i].s.x - edges[i].e.x) / (edges[i].s.y - edges[i].e.y);
		tmp->ymax = ymax;
		tmp->next = NULL;
		NET* p = net[ymin];
		while(p->next!=NULL)
			p = p->next;
		p->next = tmp;
	}

	/*for (int i = MIN_Y; i <= MAX_Y; i++)
	{
		NET* n = net[i]->next;
		int cnt = 0;
		while (n != NULL)
		{
			cout<<" "<<cnt<<" " << n->x << " " << n->ymax << " " <<n->dx<< endl;
			cnt++;
			n = n->next;
		}
	}*/
	//Initialize AET
	AET* aet = new AET;
	aet->next = NULL;
	for (int i = MIN_Y; i <= MAX_Y; i++)
	{
		//从AET表中删除ymax == i的结点,ymax > i 递增dx
		AET* del = aet;
		AET* delNext = aet->next;
		while (delNext != NULL)
		{
			if (delNext->ymax == i)
			{
				del->next = delNext->next;
				delete delNext;
				delNext = del->next;
			}
			else
			{
				del = del->next;
				delNext = delNext->next;
			}
		}
		AET* add = aet->next;
		while (add != NULL)
		{
			if (add->ymax > i)
			{
				add->x = add->x + add->dx;
				add = add->next;
			}
		}
		AET* q = aet;
		NET* p = NULL;	
		//将NET中的点增加到AET并按照x递增排序
		p = net[i]->next;
		while (p != NULL)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET* n = p->next;
			p->next = q->next;
			q->next = p;
			p = n;
			q = aet;
		}
		//改写像素颜色值
		p = aet->next;
		while (p && p->next)
		{
			//cout << p->x << " " << p->next->x - 1 << endl;
			drawLine(p->x + 1, p->next->x, i);
			p = p->next->next;
		}
		
			
	}
}

void mouseHitSF(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{

		if (button == GLUT_LEFT_BUTTON)
		{
			y = HEIGHT - y;
			//cout << k << " " << x << " " << y << endl;
			drawPixel(x, y);
			if (k == inputEdgeNum) isEnough = true;
			if (isFirstSF)
			{
				pos[0].x = x, pos[0].y = y;
				isFirstSF = false;
				k++;
			}
			else if (!isEnough)
			{
				pos[k].x = x, pos[k].y = y;
				integerBresenhamLine(pos[k -1].x, pos[k - 1].y, pos[k].x, pos[k].y);
				edges[k - 1].s = Point(pos[k].x, pos[k].y);
				edges[k - 1].e = Point(pos[k - 1].x, pos[k - 1].y);
				k++;
				if (k == inputEdgeNum)
				{
					isEnough = true;
					integerBresenhamLine(pos[k - 1].x, pos[k - 1].y, pos[0].x, pos[0].y);
					edges[k - 1].s = Point(pos[0].x, pos[0].y);
					edges[k - 1].e = Point(pos[k - 1].x, pos[k - 1].y);
				}
			}
			
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			scanFill();
		}
	}
		
}
void displayScanFill()
{
	glViewport(0, 0, WIDTH, HEIGHT);
}