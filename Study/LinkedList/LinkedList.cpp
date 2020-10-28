#include "LinkedList.h"

template <class MT>
LinkedList<MT>::LinkedList()
{
	n_Head = new MNode<MT>;
	memset(n_Head, 0, sizeof(MNode<MT>));
	n_Tail = new MNode<MT>;
	memset(n_Tail, 0, sizeof(MNode<MT>));
	n_Head->n_Next = n_Tail;
	n_Tail->n_Prev = n_Head;
}

template <class MT>
LinkedList<MT>::~LinkedList()
{
	AllDelNode();
	delete n_Head;
	n_Head = 0;
	delete n_Tail;
	n_Tail = 0;
}

template<class MT>
void LinkedList<MT>::LinkNode(MNode<MT>* prevNode, MNode<MT>* newNode)
{
	MNode<MT>* tempNode = prevNode->n_Next;
	prevNode->n_Next = newNode;
	newNode->n_Next = tempNode;
	tempNode->n_Prev = newNode;
	newNode->n_Prev = prevNode;
}

template<class MT>
void LinkedList<MT>::DelNode(MNode<MT>* delNode)
{
	MNode<MT>* DPrev = delNode->n_Prev;
	MNode<MT>* DNext = delNode->n_Next;
	DPrev->n_Next = DNext;
	DNext->n_Prev = DPrev;
	delete delNode->m_Data;
	delNode->m_Data = 0;
	delete delNode;
	delNode = 0;
}

template<class MT>
void LinkedList<MT>::AllDelNode()
{
	MNode<MT>* ADelete = n_Head->n_Next;
	while (ADelete != n_Tail)
	{
		DelNode(ADelete);
		ADelete = n_Head->n_Next;
	}
}

template<class MT>
bool LinkedList<MT>::FindSameNum(int fNum)
{
	for (MNode<MT>* CheckNode = n_Head->n_Next; CheckNode != n_Tail; CheckNode = CheckNode->n_Next)
	{
		MT* FNum = CheckNode->m_Data;
		if (FNum->m_Num == fNum)
		{
			return true;
		}
	}
	return false;
}

template<class MT>
bool LinkedList<MT>::AscendingNumber(MNode<MT>* PrevNode, MNode<MT>* NowNode)
{
	if (PrevNode->m_Data->m_Num > NowNode->m_Data->m_Num)
	{
		return true;
	}
	else false;
}

template<class MT>
bool LinkedList<MT>::AscendingGrade(MNode<MT>* PrevNode, MNode<MT>* NowNode)
{
	if (PrevNode->m_Data->m_fAverage > NowNode->m_Data->m_fAverage)
	{
		return true;
	}
	else false;
}

template<class MT>
MNode<MT>* LinkedList<MT>::SortLink(MT* lkLoad)
{
	MNode<MT>* LoadNodeLink;
	LoadNodeLink = new MNode<MT>;
	memset(LoadNodeLink, 0, sizeof(MNode<MT>));
	LoadNodeLink->m_Data = lkLoad;

	MNode<MT>* SortResult = n_Head;
	for (MNode<MT>* ForSort = n_Head->n_Next; ForSort != n_Tail; ForSort = ForSort->n_Next)
	{
		if (AscendingGrade(ForSort, LoadNodeLink) == true)
		{
			return SortResult;
		}
		SortResult = ForSort;
	}
	return SortResult;
}
