#pragma once
#include<string>
#include<cstring>
using namespace std;
#define ADD 1
#define SUB 2
#define MULT 3
#define DIV 4
#define NOT 5
//�ٶ�a��ͬ��ֵ1��2��3��4�ֱ��Ӧ�Ӽ��˳�

//��ϵ����
#define EQL 6//���
#define UEQ 7//����
#define LES 8//С��
#define LESOREQL 9//С�ڵ���
#define GRT 10//����
#define GRTOREQL 11//���ڵ���


#define R 12
#define W 13

struct Instructions
{//���ඨ��Ŀ��ָ��ĸ�ʽ
	
	string ins;//������
	int l;//��β�
	int a;//λ����

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