#pragma once
#include"SuffixTree.h"



int main()
{
	SuffixTree* st = new SuffixTree();
	cout << "*******************************************************************************************************" << endl;
	cout << "*                                                                                                     *" << endl;
	cout << "* ���������                                                                                        *" << endl;
	cout << "*            ����create,������׺��                                                                    *" << endl;
	cout << "*            ����find,�����ַ����Ĳ�ѯ                                                                *" << endl;
	cout << "*            ����count,�����ַ������ظ�������ѯ                                                       *" << endl;
	cout << "*            ����maxsub,������ظ��Ӵ��Ĳ�ѯ                                                        *" << endl;
	cout << "*            ����maxpub,�������ַ���������Ӵ��Ĳ�ѯ                                                *" << endl;
	cout << "*            ����quit,�˳�                                                                            *" << endl;
	cout << "*            ע�⣺                                                                                   *" << endl;
	cout << "*            maxpub�������˳�������������֮��                                                       *" << endl;
	cout << "*                                                                                                     *" << endl;
	cout << "*******************************************************************************************************" << endl;
	cout << "����������:" << endl;
	string ope;
	cin >> ope;
	while (ope != "quit")
	{

		string word;
		if (ope == "create")
		{
			cout << "�������ַ���:" << endl;

			cin >> word;
			st->init();
			st->CreateSuffixTree(word + "#");
			cout << "��׺�������ɹ���" << endl;
		}
		else if (ope == "find")
		{
			string find;
			cout << "��������Ҫ���ҵ��ַ���:" << endl;
			cin >> find;
			if (st->searchSubstring(find))
			{
				cout << "�����Ӵ�" << endl;
			}
			else
			{
				cout << "�������Ӵ�" << endl;
			}
		}
		else if (ope == "count")
		{
			string find;
			cout << "��������Ҫ���ҵ��ַ���:" << endl;
			cin >> find;
			cout << "�Ӵ��ظ�����Ϊ��";
			int cnt = st->cntSubstring(find, st->getRoot());
			if (cnt == -1)
				cout << "�����ڴ��Ӵ�" << endl;
			else
				cout << cnt << endl;
		}
		else if (ope == "maxsub")
		{
			string res = st->maxRepeatSubstr(st->getRoot());
			if (res == "")
			{
				cout << "�ַ�����û���ظ��Ӵ�" << endl;;
			}
			else
			{
				cout << "��ظ��Ӵ�Ϊ";
				cout << res << endl;
			}
		}
		else if (ope == "maxpub")
		{
			string newword;
			cout << "��������һ���ַ���:";
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
				cout << "�����ڹ����ַ���" << endl;
			}
			else
			{
				cout << mps << endl;
			}
		}
		else
		{
			cout << "������ʽ��������������" << endl;
		}
		cout << "����������:" << endl;
		cin >> ope;
	}
	cout << "�ɹ��˳�����" << endl;
	return 0;
}