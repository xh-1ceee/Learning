#pragma once
#include<GL/glut.h>
#include<cstdio>
#include<iostream>
#include<map>
#include<utility>
#include<vector>
#include<cstring>
#include<queue>
#include<cmath>
const int WIDTH = 800;
const int HEIGHT = 800;

GLint modeSelection = 1;
struct Point {
	GLdouble x;
	GLdouble y;
	Point()
	{
		x = 0;
		y = 0;
	}
	Point(double a, double b)
	{
		x = a;
		y = b;
	}
	bool operator == (const Point& b) const
	{
		return (x == b.x) && (y == b.y);
	}
	bool operator<(const Point& p) const
	{
		return (x < p.x) || (x == p.x && y < p.y);
	}
}pos[1024];
struct Edge {
	Point s;
	Point e;
}edges[1024];




