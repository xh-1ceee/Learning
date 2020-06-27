#include<iostream>
#include"PL0.h"
using namespace std;
int main()
{
	PL0 test("test.txt");
	//PL0 test("PL0_code.in");
	test.getSYM();
	test.getSYMF("wordres.txt");
	test.BLOCK();
	test.printTable();
	test.printTableF("tableres.txt");
	test.printCode();
	test.printCodeF("coderes.txt");
}