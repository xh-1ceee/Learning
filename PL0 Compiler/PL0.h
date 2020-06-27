#pragma once
#include<string>
#include<cstring>
#include<fstream>
#include<iostream>
#include<vector>
#include"ReserveWord.h"
#include"Instructions.h"
#include"TableRow.h"
#include"Error.h"
using namespace std;
class PL0
{
public:
	PL0(string input);
	void getWord();
	void getSYM();
	void getSYMF(string f);
	void BLOCK();
	void printTable();
	void printTableF(string f);
	void SENTENCE();
	void printCode();
	void printCodeF(string f);
private:
	int wordIdx = -1;
	Word curWord;
	Word reserveWord[13];
	string file;
	string curLine;//��ǰ������
	string curP = "";//��ǰ��������
	int curCharIndex = 0;//��ǰ�����ַ�����
	char curChar;//��ǰ�����ַ�
	int curLev = -1;
	std::vector<Word> words;//�ʷ��������vec
	std::vector<TableRow> table;
	std::vector<Instructions> CODE;
	Error e;
	int search(string str)
	{//�жϱ����Ƿ��Ǳ�����
		for (int i = 0; i < 13; i++)
		{
			if (reserveWord[i].rw == str) return i;
		}
		return -1;
	}
	int searchTable(string name)
	{//�жϱ����Ƿ������ɵķ��ű���
		for (int i = 0; i < table.size(); i++)
		{
			if (table[i].getName() == name && table[i].getLEV() <= curLev) return i;
		}
		return -1;
	}
	void getNextWord()
	{
		wordIdx++;
		if(wordIdx<words.size())
			curWord = words[wordIdx];
	}
	void addConst();
	void addVar(int dx);
	void addProd(int b);
	void expression();
	void item();
	void factor();
	void Boolean();
};