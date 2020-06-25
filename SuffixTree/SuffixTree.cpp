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
	//若A在B中，A肯定是B的一个后缀的前缀
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
{//针对所有非叶节点,采用递归
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
	//给定的字符串A在字符串B中重复出现的次数。也就是子串重复的次数。
	//用B#的形式构建后缀树，查找A节点下的叶子节点的个数即为重复次数， 如果A在B中重复了3次，那么B肯定有三个后缀的以A为前缀。
	int index = 0;
	int cnt = 0;
	for (int i = 0; i < node->edges.size(); i++)
	{
		Edge* tmp = &node->edges.at(i);
		string s1 = word.substr(tmp->startIndex, tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1);
		string s2 = sub.substr(0, tmp->returnEndIndex(CURPOS) - tmp->startIndex + 1);
		if (s1.length() > sub.length())
		{//如果边上的长度大于要找的
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
	//最深的非叶子节点所经历的字符串连接起来就是最长的重复子串，因为如果是重复的叶子节点必须大于1
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
		{//如果叶子节点的入边含有#
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
	//用A#B$作为字符串压入后缀树，找到最深的非叶子节点，并且该节点的叶子节点即有$也有#即可。
	//最深的叶子节点是指从root所经历过的字符的个数
	//递归统计深度（最大公共子串长度）
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