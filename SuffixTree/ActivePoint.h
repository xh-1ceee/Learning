#pragma once
#include"Node.h"
//�����Ԫ��
class ActivePoint
{
public:
	Node* activeNode;
	Edge* activeEdge;
	int length;

	ActivePoint(Node* _activeNode, int _length) 
	{
		this->activeNode = _activeNode;
		this->length = _length;
		activeEdge = NULL;
	}
	ActivePoint() {}
};