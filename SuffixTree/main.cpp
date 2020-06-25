#pragma once
#include"SuffixTree.h"



int main()
{
	SuffixTree* st = new SuffixTree();
	cout << "*******************************************************************************************************" << endl;
	cout << "*                                                                                                     *" << endl;
	cout << "* 请输入命令：                                                                                        *" << endl;
	cout << "*            输入create,创建后缀树                                                                    *" << endl;
	cout << "*            输入find,进行字符串的查询                                                                *" << endl;
	cout << "*            输入count,进行字符串的重复次数查询                                                       *" << endl;
	cout << "*            输入maxsub,进行最长重复子串的查询                                                        *" << endl;
	cout << "*            输入maxpub,进行两字符串最长公共子串的查询                                                *" << endl;
	cout << "*            输入quit,退出                                                                            *" << endl;
	cout << "*            注意：                                                                                   *" << endl;
	cout << "*            maxpub命令测试顺序放在其他命令之后                                                       *" << endl;
	cout << "*                                                                                                     *" << endl;
	cout << "*******************************************************************************************************" << endl;
	cout << "请输入命令:" << endl;
	string ope;
	cin >> ope;
	while (ope != "quit")
	{

		string word;
		if (ope == "create")
		{
			cout << "请输入字符串:" << endl;

			cin >> word;
			st->init();
			st->CreateSuffixTree(word + "#");
			cout << "后缀树创建成功！" << endl;
		}
		else if (ope == "find")
		{
			string find;
			cout << "请输入想要查找的字符串:" << endl;
			cin >> find;
			if (st->searchSubstring(find))
			{
				cout << "存在子串" << endl;
			}
			else
			{
				cout << "不存在子串" << endl;
			}
		}
		else if (ope == "count")
		{
			string find;
			cout << "请输入想要查找的字符串:" << endl;
			cin >> find;
			cout << "子串重复个数为：";
			int cnt = st->cntSubstring(find, st->getRoot());
			if (cnt == -1)
				cout << "不存在此子串" << endl;
			else
				cout << cnt << endl;
		}
		else if (ope == "maxsub")
		{
			string res = st->maxRepeatSubstr(st->getRoot());
			if (res == "")
			{
				cout << "字符串中没有重复子串" << endl;;
			}
			else
			{
				cout << "最长重复子串为";
				cout << res << endl;
			}
		}
		else if (ope == "maxpub")
		{
			string newword;
			cout << "请输入另一个字符串:";
			cin >> newword;
			string word = st->getWord();
			st->init();
			st->CreateSuffixTree(word.substr(0, word.length() - 1) + "$" + newword + "#");
			DWORD start_time = GetTickCount();
			string mps = st->searchPub(st->getRoot());
			DWORD end_time = GetTickCount();
			cout << "The run time is:" << (end_time - start_time) << "ms!" << endl;
			if (mps == "")
			{
				cout << "不存在公共字符串" << endl;
			}
			else
			{
				cout << mps << endl;
			}
		}
		else
		{
			cout << "命令形式错误，请重新输入" << endl;
		}
		cout << "请输入命令:" << endl;
		cin >> ope;
	}
	cout << "成功退出程序！" << endl;
	return 0;
}