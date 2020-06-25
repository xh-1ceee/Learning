#pragma once
#include"ActivePoint.h"
#include<cstring>
#include <map>
#include <string>
#include<cmath>
#include<algorithm>
#include<iostream>
#include <ctime>
#include<Windows.h>
using namespace std;


//后缀树类
class SuffixTree
{
	//每个步骤的工作量是 O(1)，因为已存在的边都是依据 "#" 的挪动而自动更改的，仅需为最后一个字符添加一条新边，所以时间复杂度为 O(1)。
	//对于一个长度为 n 的 Text，共需要 O(n) 的时间构建后缀树。
public:
	SuffixTree() 
	{
		this->nodeNumber = 1;
		Node* rootNode = new Node(this->nodeNumber, false);
		root = rootNode;
		this->nodeNumber++;


		this->remainder = 0;
		this->startIndexInsert = 0;
		ActivePoint* triple = new ActivePoint(root, 0);
		this->activePoint = triple;
		previousInsertedNode = NULL;
	}
	string getWord()
	{
		return this->word;
	}
	void CreateSuffixTree(string _word)
	{
		this->word = _word;
		for (int i = 0; i < word.length(); i++)
		{
			AddCharacter(word[i]);
		}
	}
	Node* getRoot()
	{
		return root;
	}
	void init()
	{//重新初始化后缀树
		this->nodeNumber = 1;
		Node* rootNode = new Node(this->nodeNumber, false);
		root = rootNode;
		this->nodeNumber++;
		CURPOS = -1;

		this->remainder = 0;
		this->startIndexInsert = 0;
		ActivePoint* triple = new ActivePoint(root, 0);
		this->activePoint = triple;
		previousInsertedNode = NULL;


	}
	bool searchSubstring(string sub);
	int getleaf(Node* sn);
	int cntSubstring(string sub, Node* node);
	string maxRepeatSubstr(Node* node);
	bool judge(Node* n);
	string searchPub(Node* n);

private:
	Node* root;
	int nodeNumber;
	string word;
	int CURPOS = -1;
	ActivePoint* activePoint;
	int remainder; //剩余后缀数（remainder）代表着还需要插入多少个新的后缀
	//在每个步骤开始时，剩余后缀数（remainder）总是 1。意味着，每次我们要插入的新的后缀数目为 1，即最后一个字符。
	int startIndexInsert; //新的边的开始字符在原字符串的索引
	Node* previousInsertedNode; //同一步中之前插入的节点

	//逐步添加字符进行构造
	void AddCharacter(char c)
	{
		previousInsertedNode = NULL;
		CURPOS++;
		remainder++;

		while (remainder > 0)
		{

			if (activePoint->length == 0)
			{
				startIndexInsert = CURPOS;
			}
			if (NeedToInsertNewEdge(false))
			{
				//直接插入一个新的边
				AddNewEdge(activePoint->activeNode, startIndexInsert, startIndexInsert);
				if (previousInsertedNode != NULL)
				{
					previousInsertedNode->suffixLink = activePoint->activeNode;
				}
				previousInsertedNode = activePoint->activeNode;
			}
			else
			{
				activePoint->activeEdge = ChooseActiveEdge();
				int edgeLen = activePoint->activeEdge->returnEndIndex(CURPOS) - activePoint->activeEdge->startIndex + 1;
				if (activePoint->length >= edgeLen)
				{//如果ap的长度大，将活动点更新到相应子节点
					startIndexInsert += edgeLen;
					activePoint->length -= edgeLen;
					activePoint->activeNode = activePoint->activeEdge->endNode;
					activePoint->activeEdge = ChooseActiveEdge();
					continue;
				}
				if (!NeedToInsertNewEdge(true))
				{//需要更新AP，但是没有出现新的后缀的
					activePoint->length++;
					if (previousInsertedNode != NULL)
					{
						previousInsertedNode->suffixLink = activePoint->activeNode;
					}
					previousInsertedNode = activePoint->activeNode;
					break;
				}

				int splitIndex = activePoint->length + activePoint->activeEdge->startIndex;
				//需要分裂边
				AddNewInternalEdge(splitIndex, CURPOS);
				if (previousInsertedNode != NULL)
				{
					previousInsertedNode->suffixLink = activePoint->activeEdge->endNode;
				}
				previousInsertedNode = activePoint->activeEdge->endNode;
			}

			remainder--;
			if (activePoint->activeNode->number == root->number && activePoint->length > 0)
			{
				activePoint->length--;
				startIndexInsert++;
				activePoint->activeEdge = ChooseActiveEdge();
			}
			else
			{
				if (activePoint->activeNode->suffixLink == NULL)
				{
					activePoint->activeNode = root;
				}
				else
				{
					activePoint->activeNode = activePoint->activeNode->suffixLink;
				}
			}
		}
	}

	//加边
	void AddNewEdge(Node* startNode, int startIndex, int endIndex) 
	{
		Node* leaf = new Node(nodeNumber, true);
		nodeNumber++;
		Edge* edge = new Edge(startIndex, endIndex, startNode, leaf);
		startNode->edges.push_back(*edge);
	}

	//分裂边
	void AddNewInternalEdge(int startIndex, int endIndex) 
	{
		Node* splittedEdgeNode = new Node(nodeNumber, false);
		nodeNumber++;

		Edge* splittedEdge = new Edge(activePoint->activeEdge->startIndex, startIndex - 1, activePoint->activeNode, splittedEdgeNode);

		activePoint->activeNode->edges.push_back(*splittedEdge);
		activePoint->activeEdge = ChooseActiveEdge();

		Node* leafEdgeNode = new Node(nodeNumber, true);
		nodeNumber++;
		Edge* leafEdge = new Edge(endIndex, endIndex, splittedEdgeNode, leafEdgeNode);

		splittedEdgeNode->edges.push_back(*leafEdge);

		activePoint->activeEdge->startIndex += activePoint->length;

		activePoint->activeEdge->startNode = splittedEdgeNode;
		splittedEdgeNode->edges.push_back(*(activePoint->activeEdge));

		activePoint->activeNode->RemoveEdge(activePoint->activeEdge);
		activePoint->activeEdge = splittedEdge;
	}


	//确定是否需要插入新边
	bool NeedToInsertNewEdge(bool useActiveLen)
	{//参数为是否需要活动点
		int len = 1;
		if (useActiveLen)
		{
			len += activePoint->length;
		}
		if (activePoint->activeEdge == NULL)
		{
			int edgesSize = activePoint->activeNode->edges.size();
			for (int i = 0; i < edgesSize; i++)
			{
				if (len == 1)
				{
					Edge* tmp = &activePoint->activeNode->edges.at(i);
					char s1 = word.at(tmp->startIndex);
					char s2 = word.at(startIndexInsert);
					if (s1 == s2)
					{
						return false;
					}
				}
				else
				{
					Edge* tmp = &activePoint->activeNode->edges.at(i);
					string s1 = word.substr(tmp->startIndex, len);
					string s2 = word.substr(startIndexInsert, len);
					if (s1.compare(s2) == 0)
					{
						return false;
					}
				}
			}
			return true;
		}
		else
		{
			if (len == 1)
			{
				char s1 = word.at(activePoint->activeEdge->startIndex);
				char s2 = word.at(startIndexInsert);
				if (s1 == s2)
				{
					return false;
				}
			}
			else
			{
				string s1 = word.substr(activePoint->activeEdge->startIndex, len);
				string s2 = word.substr(startIndexInsert, len);
				if (s1.compare(s2) == 0)
				{
					return false;
				}
			}
		}
		return true;
	}

	//活动点的出边确定
	Edge* ChooseActiveEdge() 
	{
		int edgeSize = activePoint->activeNode->edges.size();
		for (int i = 0; i < edgeSize; i++) 
		{
			Edge* tmp = &activePoint->activeNode->edges.at(i);
			char s1 = word.at(tmp->startIndex);
			char s2 = word.at(startIndexInsert);
			if (s1 == s2) 
			{//equals
				return tmp;
			}
		}
		return NULL;
	}

};


