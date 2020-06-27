#pragma once
#include<cstring>
#include<string>
using namespace std;

class TableRow
{
private:
	string NAME, KIND;
	int VAL, LEV, ADR;
public:
	TableRow();
	TableRow(const TableRow& t);
	TableRow(string name,string kind,int val,int lev,int adr);
	void setLEV(int l)
	{
		LEV = l;
	}
	void setADR(int a)
	{
		ADR = a;
	}
	string getName()
	{
		return NAME;
	}
	string getKind()
	{
		return KIND;
	}
	int getVal()
	{
		return VAL;
	}
	int getLEV()
	{
		return LEV;
	}
	int getADR()
	{
		return ADR;
	}
};