#pragma once
#include "SMap.h"
struct SNode
{
	DWORD					m_dwDepth;
	BOOL					m_blLeaf;
	UINT					m_iCornerIndex[4];
	SNode*					m_pChild[4];
	std::vector<DWORD>      m_vIndexList;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
};
class SQuadtree
{
	SNode* m_pRooSNode;
	std::vector<SNode*>  m_leafList;
public:
	SMap*	m_pMap;
	bool	CreateQuadtree(SMap* pMap);
	bool	BuildTree();
	SNode*  CreateNode(SNode* pParenSNode, UINT tr, UINT tl, UINT br, UINT bl);
	bool    BuildQuadtree(SNode* pNode);
	bool    SpliteChild(SNode* pNode);
	bool    Render(ID3D11DeviceContext* pContext);
	void    Draw(SNode* pNode, ID3D11DeviceContext* pd3dContext);
};

