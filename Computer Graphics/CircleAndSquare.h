#pragma once
#include"GlobalV.h"
const int n = 100;
const GLfloat PI = 3.1415926;


void displayCircleAndquare()
{
	GLfloat r = 1.0f;
	glClear(GL_COLOR_BUFFER_BIT);
	for (int k = 0; k < 10; k++)
	{

		glBegin(GL_LINE_LOOP);
		if (k % 2 == 0)
		{
			for (int i = 1; i <= 4; i++)
			{
				glVertex2f(r * cos(2 * PI / 4 * i), r * sin(2 * PI / 4 * i));
			}
		}
		else
		{
			for (int i = 1; i <= 4; i++)
			{
				glVertex2f(r * cos((i - 1) * PI / 2 + PI / 4), r * sin((i - 1) * PI / 2 + PI / 4));  //45 135 225 315
			}
		}
		glEnd();
		r /= 1.414f;
		glBegin(GL_LINE_LOOP);
		for (int i = 1; i <= 100; i++)
		{
			glVertex2f(r * cos(2 * PI / 100 * i), r * sin(2 * PI / 100 * i));

		}
		glEnd();


	}
	glFlush();
}