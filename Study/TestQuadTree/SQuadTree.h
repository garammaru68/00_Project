#pragma once
#include "Node.h"
class SQuadTree : public Node
{
public:
	Node*			m_pRootNode;
	vector<Node*>	m_vpLeafList;

public:
	Node* CreateNode(Node* pParentNode, float TopLeft, float TopRight, float BottomLeft, float BottomRight);

};

