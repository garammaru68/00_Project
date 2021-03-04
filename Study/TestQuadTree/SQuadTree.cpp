#include "SQuadTree.h"

Node * SQuadTree::CreateNode(Node * pParentNode, float TopLeft, float TopRight, float BottomLeft, float BottomRight)
{
	Node* pNode;
	SAFE_NEW(pNode, Node);
	pNode->m_vCornerList.reserve(4);

	pNode->m_vCornerList.push_back(TopLeft);
	pNode->m_vCornerList.push_back(TopRight);
	pNode->m_vCornerList.push_back(BottomLeft);
	pNode->m_vCornerList.push_back(BottomRight);

	if (pParentNode)
	{
		pNode->m_dwDepth = pParentNode->m_dwDepth + 1;
	}
	return nullptr;
}
