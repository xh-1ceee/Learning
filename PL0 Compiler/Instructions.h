#pragma once
#include<string>
#include<cstring>
using namespace std;
#define ADD 1
#define SUB 2
#define MULT 3
#define DIV 4
#define NOT 5
//假定a不同的值1，2，3，4分别对应加减乘除

//关系运算
#define EQL 6//相等
#define UEQ 7//不等
#define LES 8//小于
#define LESOREQL 9//小于等于
#define GRT 10//大于
#define GRTOREQL 11//大于等于


#define R 12
#define W 13

struct Instructions
{//此类定义目标指令的格式
	
	string ins;//功能码
	int l;//层次差
	int a;//位移量

	Instructions(string i, int _l, int _a)
	{
		ins = i;
		l = _l;
		a = _a;
	}
	Instructions(const Instructions& instr)
	{
		ins = instr.ins;
		l = instr.l;
		a = instr.a;
	}
};