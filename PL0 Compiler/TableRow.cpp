#include"TableRow.h"
TableRow::TableRow()
{
	NAME = "";
	KIND = "";
	VAL = -1;
	LEV = 0;
	ADR = 0;
}
TableRow::TableRow(const TableRow& t)
{
	NAME = t.NAME;
	KIND = t.KIND;
	VAL = t.VAL;
	LEV = t.LEV;
	ADR = t.ADR;
}
TableRow::TableRow(string name, string kind, int val, int lev, int adr)
{
	NAME = name;
	KIND = kind;
	VAL = val;
	LEV = lev;
	ADR = adr;
}