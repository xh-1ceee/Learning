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
			{//字母后边可以跟字母或者数字
				while (isalpha(ch) || isdigit(ch) && curCharIndex < curLine.length() - 1)
				{
					curCharIndex++;
					ch = curLine[curCharIndex];
				}
				//跳出循环说明不是字母或者数字
				if (!isalpha(ch) && !isdigit(ch))
				{
					curCharIndex--;
					rp = curCharIndex;
				}
				//截取
				int wordLen = rp - lp + 1;
				if (wordLen > MAX_WORD_LEN)
				{
					//此处有报错
					e.printErr(0);
					
				}
				else
				{
					res = curLine.substr(lp, wordLen);
					curCharIndex++;
				}
				//查找保留字表
				int reserveIndex = search(res);
				if (reserveIndex == -1)
				{//保留字表中没有,为标识符
					//return Word(res, "IDENT");
					words.push_back(Word(res, "IDENT"));
					continue;
				}
				else
				{
					//为保留字
					words.push_back(Word(res));
					continue;
				}
			}
			else if (isdigit(ch))
			{//识别整数
				while (isdigit(ch) && curCharIndex < curLine.length() - 1)
				{
					curCharIndex++;
					ch = curLine[curCharIndex];
				}
				if (!isdigit(ch))
				{//跳出循环回退
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
			//双字符的运算符
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
			//单字符的运算符
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
		cout << "词：" << words[i].rw << " " << "类：" << words[i].SYM   << endl;
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
		fout << "词：" << words[i].rw << " " << "类：" << words[i].SYM << endl;
	}
}
void PL0::BLOCK() 
{
	curLev++;
	int dx = 3;
	int curProcedure = CODE.size();
	if(curLev==0)CODE.push_back(Instructions("JMP", 0, 0));//主程序
	while (curWord.SYM == "constSYM" || curWord.SYM == "varSYM"
		|| curWord.SYM == "procedureSYM")
	{
		if (curWord.SYM == "constSYM")
		{
			
			//取下一个常量标识符
			addConst();
			while (curWord.SYM == ",SYM")
			{//逗号说明连续常量声明
				addConst();
			}
			if (curWord.rw == ";")
			{//分号说明常量声明结束
				getNextWord();
			}
			else
			{//结尾不对，报错
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
			addProd(CODE.size());//传入子程序首地址
			//调用函数之后为分号
			if (curWord.SYM != ";SYM")
			{
				e.printErr(3);
			}
			else
			{
				getNextWord();
			}
			//递归调用BLOCK
			int lev = curLev;
			//string tmpP = curP;
			BLOCK();
			//getNextWord();
			//curP = tmpP;
			curLev = lev;
			
		}
	}
	int curCX = CODE.size();
	if (curLev == 0)CODE[curProcedure].a = curCX;//跳转指令目标地址修改
	CODE.push_back(Instructions("INT", 0, dx));//分配空间
	SENTENCE();
	CODE.push_back(Instructions("OPR", 0, 0));//子程序返回
}
void PL0::addConst()
{
	getNextWord();
	string constVar, constVal;
	string constKind = "CONSTANT";
	//当前是标识符
	if (curWord.SYM == "IDENT")
	{
		constVar = curWord.rw;
	}
	else
	{
		e.printErr(4);
	}
	//下一个是等号
	getNextWord();
	if (curWord.SYM != "=SYM")
	{
		e.printErr(5);
	}
	//下一个是常量值
	getNextWord();
	if (curWord.SYM == "NUMBER")
	{
		constVal = curWord.rw;
	}
	table.push_back(TableRow(constVar, constKind, stoi(constVal), -1, -1));
	getNextWord();//判断到下一个逗号（可能）
}

void PL0::addVar(int dx)
{//VARIABLE属于IDENT类
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
{//下一个是子过程命名，下一个是分号
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
	//作为子程序表的开头
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
{//需要处理 赋值，if,begin,while,read,write,call
	if (curWord.SYM == "IDENT")
	{//赋值语句:已定义变量，正确的赋值号，右侧是表达式
		int varIndex = searchTable(curWord.rw);//判断是否在符号表中出现
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
		//之前的都正确，处理右边的表达式
		else getNextWord();
		expression();
		//将表达式的值写入
		CODE.push_back(Instructions("STO", curLev - table[varIndex].getLEV(), table[varIndex].getADR()));
		//remain
	}
	//WHILE;if之后有条件
	else if (curWord.SYM == "ifSYM" || curWord.SYM == "whileSYM")
	{
		if (curWord.SYM == "ifSYM")
		{//<条件语句> → if<条件>then<语句>
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
			//jpc非真跳转，a中储存跳转到的地址
			CODE.push_back(Instructions("JPC", 0, -1));
			int tmpC = CODE.size()-1;
			//为真继续if之后的语句
			SENTENCE();
			//设置非真跳转的地址,跳过if当中的语句
			CODE[tmpC].a = CODE.size();
		}
		else if (curWord.SYM == "whileSYM")
		{//<当型循环语句> → while<条件>do<语句>
			int whileStart = CODE.size();
			Boolean();
			//JPC非真跳过while循环
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
			//DO之后的语句完成之后要跳转到while开始（JMP无条件跳转）
			CODE.push_back(Instructions("JMP", 0, whileStart));
			//JPC非真跳过while循环
			CODE[re].a = CODE.size();
		}	
	}
	else if (curWord.SYM == "beginSYM")
	{//<复合语句> → begin<语句>{ ；<语句>}<end>
		getNextWord();
		SENTENCE();
		while (curWord.SYM == ";SYM" || curWord.SYM == "beginSYM" ||
			curWord.SYM == "callSYM" || curWord.SYM == "readSYM" ||
			curWord.SYM == "writeSYM" || curWord.SYM == "ifSYM" ||
			curWord.SYM == "whileSYM"||curWord.SYM=="IDENT")
		{//多条语句后边要有分号,最后一个之前没有分号
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
		//<过程调用语句> → call<标识符>
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
	{//<读语句> → read ( <标识符>{ ，<标识符>} ) 
	
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
					CODE.push_back(Instructions("OPR", 0, R));//读操作
					CODE.push_back(Instructions("STO", curLev - table[idx].getLEV(), table[idx].getADR()));
					getNextWord();
				}
				while (curWord.SYM == ",SYM")
				{//处理逗号分割的其他参数
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
							CODE.push_back(Instructions("OPR", 0, R));//读操作
							CODE.push_back(Instructions("STO", curLev - table[idx].getLEV(), table[idx].getADR()));
							getNextWord();
						}
					}
				}
				//最后以右括号结尾
				if (curWord.SYM != ")SYM")
				{
					e.printErr(20);
				}
				getNextWord();
			}
		}
		else
		{//非左括号
			e.printErr(19);
			getNextWord();
			cout << curWord.rw << endl;
		}
		
	}
	else if (curWord.SYM == "writeSYM")
	{//<写语句> → write( <标识符>{，<标识符>} )
		//实验指导书语法规则是标识符，样例是表达式
		getNextWord();
		if (curWord.SYM == "(SYM")
		{
			getNextWord();
			expression();
			CODE.push_back(Instructions("OPR", 0, W));//写操作
			while (curWord.SYM == ",SYM")
			{
				getNextWord();
				expression();
				CODE.push_back(Instructions("OPR", 0, W));//写操作
			}
			if (curWord.SYM != ")SYM")
			{
				e.printErr(21);
			}
			getNextWord();

		}
		else
		{//非左括号
			e.printErr(22);
			getNextWord();
			cout << curWord.rw << endl;
		}
	}

}

void PL0::expression()
{
	/*
		<表达式> → [+|-]<项>{<加减运算符><项>}
		<项> → <因子>{<乘除运算符><因子>}
		<因子> → <标识符>|<无符号整数>|(<表达式>)
	*/
	if (curWord.SYM != "+SYM" && curWord.SYM != "-SYM")
	{
		item();
	}
	else
	{//前边有正负号
		string op = curWord.rw;
		getNextWord();
		item();
		//对项取反
		if (op == "-")
		{
			CODE.push_back(Instructions("OPR", 0, NOT));
		}
	}
	int ope = 0;
	//项之后是+/-
	while (curWord.SYM == "+SYM" || curWord.SYM == "-SYM")
	{//处理多个加减运算符连接
		if (curWord.SYM == "+SYM")  ope = 1;
		else if (curWord.SYM == "-SYM") ope = -1;
		getNextWord();
		item();
		if (ope == 1) CODE.push_back(Instructions("OPR", 0, ADD));
		else if (ope == -1)CODE.push_back(Instructions("OPR", 0, SUB));
	}
}

void PL0::item()
{//处理项
	//<项> → <因子>{<乘除运算符><因子>}
	//<因子> → <标识符> | <无符号整数> | (<表达式>)
	factor();
	string op = "";
	while (curWord.SYM == "*SYM" || curWord.SYM == "/SYM")
	{
		op = curWord.rw;
		getNextWord();
		factor();
		if (op == "*")
		{
			//添加乘法指令
			CODE.push_back(Instructions("OPR", 0, MULT));
		}
		else if (op == "/")
		{
			//添加除法指令
			CODE.push_back(Instructions("OPR", 0, DIV));
		}
	}
}

void PL0::factor()
{//处理因子,处理之后的curWord为表达式之后的第一个词
	//<因子> → <标识符> | <无符号整数> | (<表达式>)
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
				{//常量，添加LIT指令
					CODE.push_back(Instructions("LIT", 0, table[idx].getVal()));
					getNextWord();
				}
				else if (table[idx].getKind() == "VARIABLE")
				{//变量，添加LOD指令
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
	{//odd<表达式>
		getNextWord();
		expression();
		//翻译指令
	}
	else
	{//<表达式><关系运算符><表达式>
		expression();
		relop = curWord;
		getNextWord();
		expression();
		//<关系运算符> → =|#|<|<=|>|>=
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