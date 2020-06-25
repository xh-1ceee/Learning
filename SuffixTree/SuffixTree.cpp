#pragma once
#include"SuffixTree.h"

int Edge::returnEndIndex(int CURPOS)
{
	if (endNode->isLeaf == 1)
	{
		return CURPOS;
	}
	else
	{
		return endIndex;
	}
}
bool SuffixTree::searchSubstring(string sub)
{
	//��A��B�У�A�϶���B��һ����׺��ǰ׺
	Node* curNode = root;
	bool flag = false;
	bool whileflag = false;
	int len = sub.length();
	while (curNode->isLeaf == false && !whileflag)
	{
		if (len == 0)
		{
			flag = true;
			break;
		}
		for (int i = 0; i < curNode->edges.size(); i++)
		{
			Edge* tmp = &curNode->edges.at(i);
			string s1 = word.substr(tmp->startIndex, tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1);
			string s2 = sub.substr(0, tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1);
			if (tmp->startNode == root && s1.length() > sub.length())
			{
				if (s1.find(sub) != string::npos)
				{
					return true;
				}
			}
			if (tmp->endNode->isLeaf == true)
			{
				if (s1 == s2)
				{
					flag = true;
					len = 0;
				}
				if (i == curNode->edges.size() - 1)
				{
					whileflag = true;
				}
			}
			else if (s1 == s2)
			{
				sub = sub.substr(0 + tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1, sub.length() - tmp->returnEndIndex(CURPOS) + tmp->startIndex - 1);
				curNode = tmp->endNode;
				len -= tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1;
				if (len < 0)
				{
					len = 0;
					flag = true;
				}
				break;
			}

		}
	}
	if (len > 0 || flag == false)return false;
	else return true;
}

int SuffixTree::getleaf(Node* sn)
{//������з�Ҷ�ڵ�,���õݹ�
	if (sn->isLeaf == true) return 1;
	int leaf = 0;
	for (int i = 0; i < sn->edges.size(); i++)
	{
		Edge* e = &sn->edges.at(i);
		if (e->endNode->isLeaf == true)
		{
			leaf++;
		}
		else
		{
			leaf += getleaf(e->endNode);
		}
	}
	return leaf;
}
int SuffixTree::cntSubstring(string sub, Node* node)
{
	if (node->isLeaf == true)
	{
		return 1;
	}
	//�������ַ���A���ַ���B���ظ����ֵĴ�����Ҳ�����Ӵ��ظ��Ĵ�����
	//��B#����ʽ������׺��������A�ڵ��µ�Ҷ�ӽڵ�ĸ�����Ϊ�ظ������� ���A��B���ظ���3�Σ���ôB�϶���������׺����AΪǰ׺��
	int index = 0;
	int cnt = 0;
	for (int i = 0; i < node->edges.size(); i++)
	{
		Edge* tmp = &node->edges.at(i);
		string s1 = word.substr(tmp->startIndex, tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1);
		string s2 = sub.substr(0, tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1);
		if (s1.length() > sub.length())
		{//������ϵĳ��ȴ���Ҫ�ҵ�
			if (s1.substr(0, sub.length()) == sub)
			{
				cnt = getleaf(tmp->endNode);
				return cnt;
			}
		}
		else
		{
			if (s1 == s2)
			{
				index += tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1;
				sub = sub.substr(index, sub.length() - (tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1));
				if (sub.length() == 0)
				{
					return getleaf(tmp->endNode);
				}
				cnt += cntSubstring(sub, tmp->endNode);
			}

		}
	}
	return cnt;
}
string SuffixTree::maxRepeatSubstr(Node* node)
{
	//����ķ�Ҷ�ӽڵ����������ַ�������������������ظ��Ӵ�����Ϊ������ظ���Ҷ�ӽڵ�������1
	string maxstr = "";
	string tmpstr = "";
	int maxlen = 0;
	bool flag = false;
	for (int i = 0; i < node->edges.size(); i++)
	{
		Edge* tmp = &node->edges.at(i);
		string restr = "";
		tmpstr = word.substr(tmp->startIndex, tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1);
		if (tmp->endNode->isLeaf == false)
		{
			flag = true;
			restr = tmpstr + maxRepeatSubstr(tmp->endNode);
		}
		if (restr.length() > maxlen)
		{
			maxlen = restr.length();
			maxstr = restr;
		}

	}
	if (flag)
	{
		return maxstr;
	}
	else return "";
}

bool SuffixTree::judge(Node* n)
{
	bool flag1 = false;
	bool flag2 = false;
	for (int i = 0; i < n->edges.size(); i++)
	{
		Edge* tmp = &n->edges.at(i);
		string tmpstr = word.substr(tmp->startIndex, tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1);
		if (tmp->endNode->isLeaf && tmpstr.find("$") != string::npos && tmpstr.find("#") != string::npos)
		{//���Ҷ�ӽڵ����ߺ���#
			flag1 = true;
		}
		else if (tmp->endNode->isLeaf && tmpstr.find("$") == string::npos && tmpstr.find("#") != string::npos)
		{
			flag2 = true;
		}

	}
	if (flag1 && flag2) return true;
	else return false;
}

string SuffixTree::searchPub(Node* n)
{
	//��A#B$��Ϊ�ַ���ѹ���׺�����ҵ�����ķ�Ҷ�ӽڵ㣬���Ҹýڵ��Ҷ�ӽڵ㼴��$Ҳ��#���ɡ�
	//�����Ҷ�ӽڵ���ָ��root�����������ַ��ĸ���
	//�ݹ�ͳ����ȣ���󹫹��Ӵ����ȣ�
	string maxstr = "";
	string tmpstr = "";
	int maxlen = 0;
	bool flag = false;
	for (int i = 0; i < n->edges.size(); i++)
	{
		Edge* tmp = &n->edges.at(i);
		string restr = "";
		tmpstr = word.substr(tmp->startIndex, tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1);
		if (judge(tmp->endNode))
		{
			flag = true;
			restr = tmpstr;
		}
		if (tmp->endNode->isLeaf == false)
		{
			restr = max(tmpstr + searchPub(tmp->endNode), restr);
		}
		if (restr.length() > maxlen)
		{
			maxlen = restr.length();
			maxstr = restr;
		}

	}
	if (flag)
	{
		return maxstr;
	}
	else return "";
}