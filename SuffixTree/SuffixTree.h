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


//��׺����
class SuffixTree
{
	//ÿ������Ĺ������� O(1)����Ϊ�Ѵ��ڵı߶������� "#" ��Ų�����Զ����ĵģ�����Ϊ���һ���ַ����һ���±ߣ�����ʱ�临�Ӷ�Ϊ O(1)��
	//����һ������Ϊ n �� Text������Ҫ O(n) ��ʱ�乹����׺����
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
	{//���³�ʼ����׺��
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
	int remainder; //ʣ���׺����remainder�������Ż���Ҫ������ٸ��µĺ�׺
	//��ÿ�����迪ʼʱ��ʣ���׺����remainder������ 1����ζ�ţ�ÿ������Ҫ������µĺ�׺��ĿΪ 1�������һ���ַ���
	int startIndexInsert; //�µıߵĿ�ʼ�ַ���ԭ�ַ���������
	Node* previousInsertedNode; //ͬһ����֮ǰ����Ľڵ�

	//������ַ����й���
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
				//ֱ�Ӳ���һ���µı�
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
				{//���ap�ĳ��ȴ󣬽������µ���Ӧ�ӽڵ�
					startIndexInsert += edgeLen;
					activePoint->length -= edgeLen;
					activePoint->activeNode = activePoint->activeEdge->endNode;
					activePoint->activeEdge = ChooseActiveEdge();
					continue;
				}
				if (!NeedToInsertNewEdge(true))
				{//��Ҫ����AP������û�г����µĺ�׺��
					activePoint->length++;
					if (previousInsertedNode != NULL)
					{
						previousInsertedNode->suffixLink = activePoint->activeNode;
					}
					previousInsertedNode = activePoint->activeNode;
					break;
				}

				int splitIndex = activePoint->length + activePoint->activeEdge->startIndex;
				//��Ҫ���ѱ�
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

	//�ӱ�
	void AddNewEdge(Node* startNode, int startIndex, int endIndex) 
	{
		Node* leaf = new Node(nodeNumber, true);
		nodeNumber++;
		Edge* edge = new Edge(startIndex, endIndex, startNode, leaf);
		startNode->edges.push_back(*edge);
	}

	//���ѱ�
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


	//ȷ���Ƿ���Ҫ�����±�
	bool NeedToInsertNewEdge(bool useActiveLen)
	{//����Ϊ�Ƿ���Ҫ���
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

	//���ĳ���ȷ��
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


