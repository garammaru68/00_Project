#pragma once
#include "SNode.h"
#include "SMap.h"

class SQuadTree
{
public:
	SMap*			m_pMap;
	SNode*			m_pRootNode;
	vector<SNode*>	m_leafList;
public:
	SNode*	CreateNode(SNode* pParentNode, UINT TopLeft, UINT TopRight, UINT BottomLeft, UINT BottomRight);
	bool	BuildTree();
	bool	CreateQuadtree(SMap* pMap);
	bool	DivideNode(SNode* pNode);
	bool    BuildQuadTree(SNode* pNode);

	bool    Render(ID3D11DeviceContext* pd3dContext);
	void    Draw(SNode* pNode, ID3D11DeviceContext* pd3dContext);

};

