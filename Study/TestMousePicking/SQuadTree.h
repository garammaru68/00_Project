#pragma once
#include "SNode.h"
#include "SHeightMap.h"

class SQuadTree
{
	SMap*  m_pMap;
	SNode* m_pRootNode;
	std::vector<SNode*>  m_leafList;
	std::vector<SNode*>  m_visibleList;
public:
	SNode*  CreateNode(SNode* pParentNode, UINT tr, UINT tl, UINT br, UINT bl);
	bool	BuildTree();
	bool	CreateQuadtree(SMap* pMap);
	bool    DivideNode(SNode* pNode);
	bool    BuildQuadTree(SNode* pNode);
	bool    Render(ID3D11DeviceContext* pContext);
	bool	Release();
	void    Draw(SNode* pNode, ID3D11DeviceContext* pd3dContext);
};
