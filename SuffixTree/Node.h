#pragma once
#include<vector>
using namespace std;

class Node;
//��ʾ�ߵ���
class Edge
{
public:
	//ÿ���ߵĿռ临�Ӷ�Ϊ O(1)����ֻ��������ָ�룬�������������ַ������޹�
	int startIndex;
	int endIndex;
	Node* startNode;
	Node* endNode;

	Edge(int _startIndex, int _endIndex, Node* _startNode, Node* _endNode) {
		this->startIndex = _startIndex;
		this->endIndex = _endIndex;
		this->startNode = _startNode;
		this->endNode = _endNode;
	}
	Edge() {}

	//������ֹ�ַ�����
	int returnEndIndex(int CURPOS);
};

class Node
{
public:
	Node* suffixLink;
	vector<Edge> edges;
	bool isLeaf;
	int number;
	Node(int _number, bool _isLeaf)
	{
		this->isLeaf = _isLeaf;
		this->number = _number;
		suffixLink = NULL;
	}
	Node() {}


	//ɾ����
	void RemoveEdge(Edge* e)
	{
		int index = -1;
		int ns = 0;
		if (e->startNode != NULL)
		{
			ns = e->startNode->number;
		}
		int ne = 0;
		if (e->endNode != NULL)
		{
			ne = e->endNode->number;
		}
		for (vector<Edge>::iterator it = edges.begin(); it != edges.end(); ++it)
		{
			Edge* edgeTmp = &(*it);
			int ss = 0;
			if (edgeTmp->startNode != NULL)
			{
				ss = edgeTmp->startNode->number;
			}
			int se = 0;
			if (edgeTmp->endNode != NULL)
			{
				se = edgeTmp->endNode->number;
			}
			if (ns == ss && ne == se)
			{
				edges.erase(it);
				break;
			}
		}
	}
	
};


