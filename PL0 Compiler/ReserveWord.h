#pragma once
#include<cstring>
#include<string>
using namespace std;
struct Word {
	string rw;//关键字
	string SYM;//对应关键字类型
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