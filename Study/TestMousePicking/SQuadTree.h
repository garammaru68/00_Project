#pragma once
#include <queue>
#include "SNode.h"
#include "SHeightMap.h"

class SQuadTree
{
public:
	SMap*			m_pMap;
	SNode*			m_pRootNode;
	vector<SNode*>	m_leafList;
	vector<SNode*>  m_visibleList;
public:
	SNode*  CreateNode(SNode* pParentNode, UINT TopLeft, UINT TopRight, UINT BottomLeft, UINT BottomRight);
	bool	BuildTree();
	bool	CreateQuadTree(SMap* pMap);
	bool    DivideNode(SNode* pNode);
	bool    BuildQuadTree(SNode* pNode);
	bool    Render(ID3D11DeviceContext* pContext);
	void    Draw(SNode* pNode, ID3D11DeviceContext* pd3dContext);
	bool	Release();
};
