#pragma once
#include <iostream>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

template <class MT>
struct MNode
{
	MT* m_Data;
	MNode* n_Next;
	MNode* n_Prev;
};

template <class MT>
class LinkedList
{
public:
	MNode<MT>* n_Head = 0;
	MNode<MT>* n_Tail = 0;
public:
	LinkedList();
	~LinkedList();
public:
	void LinkNode(MNode<MT>* prevNode, MNode<MT>* newNode);
	void DelNode(MNode<MT>* delNode);
	void AllDelNode();
	bool FindSameNum(int FNum);
	bool AscendingNumber(MNode<MT>* PrevNode, MNode<MT>* NowNode);
	bool AscendingGrade(MNode<MT>* PrevNode, MNode<MT>* NowNode);
	MNode<MT>* SortLink(MT* lkLoad);
};
