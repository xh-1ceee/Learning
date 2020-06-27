#include"PL0.h"

const int MAX_WORD_LEN = 10;
using namespace std;
void PL0::getWord()
{
	ifstream fin(file);
	while (getline(fin, curLine))
	{
		cout << "Read from file: " << curLine << endl;
		curCharIndex = 0;
		while (curCharIndex < curLine.length())
		{
			string res = "";
			int lp = curCharIndex, rp = 0;
			if (curLine[curCharIndex] == ' ' || curLine[curCharIndex] == '\t')
			{
				curCharIndex++;
				continue;
			}
			char ch = curLine[curCharIndex];
			if (isalpha(ch))
			{//��ĸ��߿��Ը���ĸ��������
				while (isalpha(ch) || isdigit(ch) && curCharIndex < curLine.length() - 1)
				{
					curCharIndex++;
					ch = curLine[curCharIndex];
				}
				//����ѭ��˵��������ĸ��������
				if (!isalpha(ch) && !isdigit(ch))
				{
					curCharIndex--;
					rp = curCharIndex;
				}
				//��ȡ
				int wordLen = rp - lp + 1;
				if (wordLen > MAX_WORD_LEN)
				{
					//�˴��б���
					e.printErr(0);
					
				}
				else
				{
					res = curLine.substr(lp, wordLen);
					curCharIndex++;
				}
				//���ұ����ֱ�
				int reserveIndex = search(res);
				if (reserveIndex == -1)
				{//�����ֱ���û��,Ϊ��ʶ��
					//return Word(res, "IDENT");
					words.push_back(Word(res, "IDENT"));
					continue;
				}
				else
				{
					//Ϊ������
					words.push_back(Word(res));
					continue;
				}
			}
			else if (isdigit(ch))
			{//ʶ������
				while (isdigit(ch) && curCharIndex < curLine.length() - 1)
				{
					curCharIndex++;
					ch = curLine[curCharIndex];
				}
				if (!isdigit(ch))
				{//����ѭ������
					curCharIndex--;
					rp = curCharIndex;
				}
				int len = rp - lp + 1;
				res = curLine.substr(lp, len);
				//return Word(res, "NUMBER");
				words.push_back(Word(res, "NUMBER"));
				curCharIndex++;
				continue;
			}
			//˫�ַ��������
			else if (ch == ':' && curLine[curCharIndex + 1] == '=')
			{
				//return Word(":=");
				words.push_back(Word(":="));
				curCharIndex += 2;
				continue;
			}
			else if (ch == ':' && curLine[curCharIndex + 1] != '=')
			{
				//return Word(":");
				words.push_back(Word(":"));
				curCharIndex++;
				continue;
			}
			else if (ch == '<' && curLine[curCharIndex + 1] == '=')
			{
				//return Word("<=");
				words.push_back(Word("<="));
				curCharIndex += 2;
				continue;
			}
			else if (ch == '<' && curLine[curCharIndex + 1] != '=')
			{
				//return Word("<");
				words.push_back(Word("<"));
				curCharIndex++;
				continue;
			}
			else if (ch == '>' && curLine[curCharIndex + 1] == '=')
			{
				//return Word(">=");
				words.push_back(Word(">="));
				curCharIndex += 2;
				continue;
			}
			else if (ch == '>' && curLine[curCharIndex + 1] != '=')
			{
				//return Word(">");
				words.push_back(Word(">"));
				curCharIndex++;
				continue;
			}
			//���ַ��������
			if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')'
				|| ch == '=' || ch == '#' || ch == ',' || ch == ';' || ch == '.')
			{
				res = ch;
				//return Word(res);
				words.push_back(Word(res));
				curCharIndex++;
				continue;
			}
		}
	}
}

PL0::PL0(string input)
{
	reserveWord[0] = Word("const");
	reserveWord[1] = Word("var");
	reserveWord[2] = Word("procedure");
	reserveWord[3] = Word("begin");
	reserveWord[4] = Word("end");
	reserveWord[5] = Word("call");
	reserveWord[6] = Word("while");
	reserveWord[7] = Word("do");
	reserveWord[8] = Word("if");
	reserveWord[9] = Word("then");
	reserveWord[10] = Word("read");
	reserveWord[11] = Word("write");
	reserveWord[12] = Word("odd");
	file = input;
}
void PL0::getSYM()
{
	getWord();
	wordIdx = 0;
	curWord = words[wordIdx];
	for (int i = 0; i < words.size(); i++)
	{
		words[i].id = i;
		cout << "�ʣ�" << words[i].rw << " " << "�ࣺ" << words[i].SYM   << endl;
	}

}
void PL0::getSYMF(string f)
{
	ofstream fout;
	fout.open(f);
	getWord();
	wordIdx = 0;
	curWord = words[wordIdx];
	for (int i = 0; i < words.size(); i++)
	{
		words[i].id = i;
		fout << "�ʣ�" << words[i].rw << " " << "�ࣺ" << words[i].SYM << endl;
	}
}
void PL0::BLOCK() 
{
	curLev++;
	int dx = 3;
	int curProcedure = CODE.size();
	if(curLev==0)CODE.push_back(Instructions("JMP", 0, 0));//������
	while (curWord.SYM == "constSYM" || curWord.SYM == "varSYM"
		|| curWord.SYM == "procedureSYM")
	{
		if (curWord.SYM == "constSYM")
		{
			
			//ȡ��һ��������ʶ��
			addConst();
			while (curWord.SYM == ",SYM")
			{//����˵��������������
				addConst();
			}
			if (curWord.rw == ";")
			{//�ֺ�˵��������������
				getNextWord();
			}
			else
			{//��β���ԣ�����
				e.printErr(1);
			}
		}
		else if (curWord.SYM == "varSYM")
		{
			addVar(dx);
			dx++;
			while (curWord.SYM == ",SYM")
			{
				addVar(dx);
				dx++;
			}
			if (curWord.SYM == ";SYM") getNextWord();
			else
			{
				e.printErr(2);
			}
		}
		else if(curWord.SYM == "procedureSYM")
		{
			addProd(CODE.size());//�����ӳ����׵�ַ
			//���ú���֮��Ϊ�ֺ�
			if (curWord.SYM != ";SYM")
			{
				e.printErr(3);
			}
			else
			{
				getNextWord();
			}
			//�ݹ����BLOCK
			int lev = curLev;
			//string tmpP = curP;
			BLOCK();
			//getNextWord();
			//curP = tmpP;
			curLev = lev;
			
		}
	}
	int curCX = CODE.size();
	if (curLev == 0)CODE[curProcedure].a = curCX;//��תָ��Ŀ���ַ�޸�
	CODE.push_back(Instructions("INT", 0, dx));//����ռ�
	SENTENCE();
	CODE.push_back(Instructions("OPR", 0, 0));//�ӳ��򷵻�
}
void PL0::addConst()
{
	getNextWord();
	string constVar, constVal;
	string constKind = "CONSTANT";
	//��ǰ�Ǳ�ʶ��
	if (curWord.SYM == "IDENT")
	{
		constVar = curWord.rw;
	}
	else
	{
		e.printErr(4);
	}
	//��һ���ǵȺ�
	getNextWord();
	if (curWord.SYM != "=SYM")
	{
		e.printErr(5);
	}
	//��һ���ǳ���ֵ
	getNextWord();
	if (curWord.SYM == "NUMBER")
	{
		constVal = curWord.rw;
	}
	table.push_back(TableRow(constVar, constKind, stoi(constVal), -1, -1));
	getNextWord();//�жϵ���һ�����ţ����ܣ�
}

void PL0::addVar(int dx)
{//VARIABLE����IDENT��
	getNextWord();
	string var, val;
	string varKind = "VARIABLE";
	if (curWord.SYM != "IDENT")
	{
		e.printErr(6);
	}
	else
	{
		var =curWord.rw;
	}
	table.push_back(TableRow(var, varKind, -1, curLev, dx));
	getNextWord();
}

void PL0::addProd(int b)
{//��һ�����ӹ�����������һ���Ƿֺ�
	getNextWord();
	string prodVar;
	string prodKind = "PROCEDURE";
	if (curWord.SYM == "IDENT")
	{
		prodVar = curWord.rw;
	}
	else
	{
		e.printErr(7);
	}
	table.push_back(TableRow(prodVar, prodKind, -1, curLev, b));
	//��Ϊ�ӳ����Ŀ�ͷ
	//table.push_back(TableRow(prodVar, prodKind, -1, curLev+1, -1));
	getNextWord();
}

void PL0::printTable()
{
	for (int i = 0; i < table.size(); i++)
	{
		if (table[i].getKind() == "CONSTANT")
		{
			cout << "NAME:" << table[i].getName() << " " << "KIND:" << table[i].getKind() << " " << "VAL:" << table[i].getVal() << endl;
		}
		else if (table[i].getKind() == "VARIABLE")
		{
			cout << "NAME:" << table[i].getName() << " " << "KIND:" << table[i].getKind() << " " << "LEVEL:" << table[i].getLEV() << " " << "ADR:" << table[i].getADR() << endl;
		}
		else if (table[i].getKind() == "PROCEDURE")
		{
			cout << "NAME:" << table[i].getName() << " " << "KIND:" << table[i].getKind() << " " << "LEVEL:" << table[i].getLEV() << endl;
		}
	}
}
void PL0::printTableF(string f)
{
	ofstream fout;
	fout.open(f);
	for (int i = 0; i < table.size(); i++)
	{
		if (table[i].getKind() == "CONSTANT")
		{
			fout << "NAME:" << table[i].getName() << " " << "KIND:" << table[i].getKind() << " " << "VAL:" << table[i].getVal() << endl;
		}
		else if (table[i].getKind() == "VARIABLE")
		{
			fout << "NAME:" << table[i].getName() << " " << "KIND:" << table[i].getKind() << " " << "LEVEL:" << table[i].getLEV() << " " << "ADR:" << table[i].getADR() << endl;
		}
		else if (table[i].getKind() == "PROCEDURE")
		{
			fout << "NAME:" << table[i].getName() << " " << "KIND:" << table[i].getKind() << " " << "LEVEL:" << table[i].getLEV() << endl;
		}
	}
}
void PL0::printCode()
{
	for (int i = 0; i < CODE.size(); i++)
	{
		cout<<i<<"  "<< CODE[i].ins << " " << CODE[i].l << " " << CODE[i].a << endl;
	}
}
void PL0::printCodeF(string f)
{
	ofstream fout;
	fout.open(f);
	for (int i = 0; i < CODE.size(); i++)
	{
		fout << i << "  " << CODE[i].ins << " " << CODE[i].l << " " << CODE[i].a << endl;
	}
}
void PL0::SENTENCE()
{//��Ҫ���� ��ֵ��if,begin,while,read,write,call
	if (curWord.SYM == "IDENT")
	{//��ֵ���:�Ѷ����������ȷ�ĸ�ֵ�ţ��Ҳ��Ǳ��ʽ
		int varIndex = searchTable(curWord.rw);//�ж��Ƿ��ڷ��ű��г���
		if (varIndex == -1)
		{
			e.printErr(8);
		}
		else if (table[varIndex].getKind() != "VARIABLE")
		{
			e.printErr(9);
		}
		getNextWord();
		if (curWord.SYM != ":=SYM")
		{
			e.printErr(10);
		}
		//֮ǰ�Ķ���ȷ�������ұߵı��ʽ
		else getNextWord();
		expression();
		//�����ʽ��ֵд��
		CODE.push_back(Instructions("STO", curLev - table[varIndex].getLEV(), table[varIndex].getADR()));
		//remain
	}
	//WHILE;if֮��������
	else if (curWord.SYM == "ifSYM" || curWord.SYM == "whileSYM")
	{
		if (curWord.SYM == "ifSYM")
		{//<�������> �� if<����>then<���>
			Boolean();
			//cout << "test:" << curWord.SYM << endl;
			if (curWord.SYM == "thenSYM")
			{
				getNextWord();
			}
			else
			{
				e.printErr(11);
			}
			//jpc������ת��a�д�����ת���ĵ�ַ
			CODE.push_back(Instructions("JPC", 0, -1));
			int tmpC = CODE.size()-1;
			//Ϊ�����if֮������
			SENTENCE();
			//���÷�����ת�ĵ�ַ,����if���е����
			CODE[tmpC].a = CODE.size();
		}
		else if (curWord.SYM == "whileSYM")
		{//<����ѭ�����> �� while<����>do<���>
			int whileStart = CODE.size();
			Boolean();
			//JPC��������whileѭ��
			CODE.push_back(Instructions("JPC", 0, -1));
			int re = CODE.size() - 1;
			if (curWord.SYM == "doSYM")
			{
				getNextWord();
			}
			else
			{
				e.printErr(12);
			}
			SENTENCE();
			//DO֮���������֮��Ҫ��ת��while��ʼ��JMP��������ת��
			CODE.push_back(Instructions("JMP", 0, whileStart));
			//JPC��������whileѭ��
			CODE[re].a = CODE.size();
		}	
	}
	else if (curWord.SYM == "beginSYM")
	{//<�������> �� begin<���>{ ��<���>}<end>
		getNextWord();
		SENTENCE();
		while (curWord.SYM == ";SYM" || curWord.SYM == "beginSYM" ||
			curWord.SYM == "callSYM" || curWord.SYM == "readSYM" ||
			curWord.SYM == "writeSYM" || curWord.SYM == "ifSYM" ||
			curWord.SYM == "whileSYM"||curWord.SYM=="IDENT")
		{//���������Ҫ�зֺ�,���һ��֮ǰû�зֺ�
			if (curWord.SYM == ";SYM")
			{
				getNextWord();
			}
			else
			{
				e.printErr(13);
			}
			SENTENCE();
		}
		if (curWord.SYM == "endSYM")
		{
			getNextWord();
			if (curWord.rw == ";" || curWord.rw == ".")
			{
				getNextWord();
			}
		}
		else
		{
			e.printErr(14);
			//cout <<"end word now is "<< curWord.rw << endl;
		}
	}
	else if (curWord.SYM == "callSYM")
	{
		//<���̵������> �� call<��ʶ��>
		getNextWord();
		if (curWord.SYM != "IDENT")
		{
			//printTable();
			e.printErr(15);
			//cout << "call " << curWord.rw << endl;
		}
		else
		{
			int idx = searchTable(curWord.rw);
			if (idx == -1)
			{
				e.printErr(8);
			}
			else if(table[idx].getKind()=="PROCEDURE")
			{
				CODE.push_back(Instructions("CAL",curLev - table[idx].getLEV(),table[idx].getADR()));
			}
			else if (table[idx].getKind() != "PROCEDURE")
			{
				e.printErr(16);
			}
			getNextWord();
		}

	}
	else if (curWord.SYM == "readSYM")
	{//<�����> �� read ( <��ʶ��>{ ��<��ʶ��>} ) 
	
		getNextWord();
		if (curWord.SYM == "(SYM")
		{
			getNextWord();
			if (curWord.SYM == "IDENT")
			{
				int idx = searchTable(curWord.rw);
				if (idx == -1)
				{
					e.printErr(17);
					getNextWord();
				}
				else if (table[idx].getKind() == "CONSTANT"|| table[idx].getKind() == "PROCEDURE")
				{
					e.printErr(18);
				}
				else
				{
					CODE.push_back(Instructions("OPR", 0, R));//������
					CODE.push_back(Instructions("STO", curLev - table[idx].getLEV(), table[idx].getADR()));
					getNextWord();
				}
				while (curWord.SYM == ",SYM")
				{//�����ŷָ����������
					getNextWord();
					if (curWord.SYM == "IDENT")
					{
						int idx = searchTable(curWord.rw);
						if (idx == -1)
						{
							e.printErr(17);
							getNextWord();
						}
						else if (table[idx].getKind() == "CONSTANT" || table[idx].getKind() == "PROCEDURE")
						{
							e.printErr(18);
						}
						else
						{
							CODE.push_back(Instructions("OPR", 0, R));//������
							CODE.push_back(Instructions("STO", curLev - table[idx].getLEV(), table[idx].getADR()));
							getNextWord();
						}
					}
				}
				//����������Ž�β
				if (curWord.SYM != ")SYM")
				{
					e.printErr(20);
				}
				getNextWord();
			}
		}
		else
		{//��������
			e.printErr(19);
			getNextWord();
			cout << curWord.rw << endl;
		}
		
	}
	else if (curWord.SYM == "writeSYM")
	{//<д���> �� write( <��ʶ��>{��<��ʶ��>} )
		//ʵ��ָ�����﷨�����Ǳ�ʶ���������Ǳ��ʽ
		getNextWord();
		if (curWord.SYM == "(SYM")
		{
			getNextWord();
			expression();
			CODE.push_back(Instructions("OPR", 0, W));//д����
			while (curWord.SYM == ",SYM")
			{
				getNextWord();
				expression();
				CODE.push_back(Instructions("OPR", 0, W));//д����
			}
			if (curWord.SYM != ")SYM")
			{
				e.printErr(21);
			}
			getNextWord();

		}
		else
		{//��������
			e.printErr(22);
			getNextWord();
			cout << curWord.rw << endl;
		}
	}

}

void PL0::expression()
{
	/*
		<���ʽ> �� [+|-]<��>{<�Ӽ������><��>}
		<��> �� <����>{<�˳������><����>}
		<����> �� <��ʶ��>|<�޷�������>|(<���ʽ>)
	*/
	if (curWord.SYM != "+SYM" && curWord.SYM != "-SYM")
	{
		item();
	}
	else
	{//ǰ����������
		string op = curWord.rw;
		getNextWord();
		item();
		//����ȡ��
		if (op == "-")
		{
			CODE.push_back(Instructions("OPR", 0, NOT));
		}
	}
	int ope = 0;
	//��֮����+/-
	while (curWord.SYM == "+SYM" || curWord.SYM == "-SYM")
	{//�������Ӽ����������
		if (curWord.SYM == "+SYM")  ope = 1;
		else if (curWord.SYM == "-SYM") ope = -1;
		getNextWord();
		item();
		if (ope == 1) CODE.push_back(Instructions("OPR", 0, ADD));
		else if (ope == -1)CODE.push_back(Instructions("OPR", 0, SUB));
	}
}

void PL0::item()
{//������
	//<��> �� <����>{<�˳������><����>}
	//<����> �� <��ʶ��> | <�޷�������> | (<���ʽ>)
	factor();
	string op = "";
	while (curWord.SYM == "*SYM" || curWord.SYM == "/SYM")
	{
		op = curWord.rw;
		getNextWord();
		factor();
		if (op == "*")
		{
			//��ӳ˷�ָ��
			CODE.push_back(Instructions("OPR", 0, MULT));
		}
		else if (op == "/")
		{
			//��ӳ���ָ��
			CODE.push_back(Instructions("OPR", 0, DIV));
		}
	}
}

void PL0::factor()
{//��������,����֮���curWordΪ���ʽ֮��ĵ�һ����
	//<����> �� <��ʶ��> | <�޷�������> | (<���ʽ>)
	if (curWord.SYM == "IDENT" || curWord.SYM == "NUMBER" || curWord.SYM == "(SYM")
	{
		if (curWord.SYM == "IDENT")
		{
			int idx = searchTable(curWord.rw);
			if (idx == -1)
			{
				e.printErr(8);
			}
			else
			{
				if (table[idx].getKind() == "CONSTANT")
				{//���������LITָ��
					CODE.push_back(Instructions("LIT", 0, table[idx].getVal()));
					getNextWord();
				}
				else if (table[idx].getKind() == "VARIABLE")
				{//���������LODָ��
					CODE.push_back(Instructions("LOD", curLev - table[idx].getLEV(), table[idx].getADR()));
					getNextWord();
				}
				else if (table[idx].getKind() == "PROCEDURE")
				{
					e.printErr(23);
					getNextWord();
				}
			}
		}
		else if (curWord.SYM == "NUMBER")
		{
			int val = stoi(curWord.rw);
			CODE.push_back(Instructions("LIT", 0, val));
			getNextWord();
		}
		else if (curWord.SYM == "(SYM")
		{
			getNextWord();
			expression();
			if (curWord.SYM == ")SYM")
			{
				getNextWord();
			}
			else
			{
				e.printErr(24);
			}
		}
	}
}

void PL0::Boolean()
{
	Word relop;
	getNextWord();
	
	if (curWord.SYM == "oddSYM")
	{//odd<���ʽ>
		getNextWord();
		expression();
		//����ָ��
	}
	else
	{//<���ʽ><��ϵ�����><���ʽ>
		expression();
		relop = curWord;
		getNextWord();
		expression();
		//<��ϵ�����> �� =|#|<|<=|>|>=
		if (relop.SYM == "=SYM")
		{
			CODE.push_back(Instructions("OPR",0, EQL));
		}
		else if (relop.SYM == "#SYM")
		{
			CODE.push_back(Instructions("OPR", 0, UEQ));
		}
		else if (relop.SYM == "<SYM")
		{
			CODE.push_back(Instructions("OPR", 0, LES));
		}
		else if (relop.SYM == "<=SYM")
		{
			CODE.push_back(Instructions("OPR", 0, LESOREQL));
		}
		else if (relop.SYM == ">SYM")
		{
			CODE.push_back(Instructions("OPR", 0, GRT));
		}
		else if (relop.SYM == ">=SYM")
		{
			CODE.push_back(Instructions("OPR", 0, GRTOREQL));
		}
		else
		{
			e.printErr(25);
		}
	}
}