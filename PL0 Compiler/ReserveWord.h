#pragma once
#include<cstring>
#include<string>
using namespace std;
struct Word {
	string rw;//�ؼ���
	string SYM;//��Ӧ�ؼ�������
	int id;
	Word(string r)
	{
		rw = r;
		SYM = r + "SYM";
	}
	Word(string r, string s)
	{
		rw = r;
		SYM = s;
	}
	Word()
	{
		rw = "";
		SYM = "";
	}
};