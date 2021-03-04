#pragma once
#include "SMap.h"
struct SNode
{
	DWORD					m_dwDepth;
	BOOL					m_blLeaf;
	UINT					m_iCornerIndex[4];
	SNode*					m_pChild[4];
	vector<DWORD>			m_IndexList;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
};
class SQuadTree
{
	SNode* m_pRootNode;
	vector<SNode*>	m_leafList;
	vector<SNode*>	m_visibleList;
public:
	SMap* m_pMap;
	SNode* CreateNode(SNode* pParentNode, UINT TopLeft, UINT TopRight, UINT BottomLeft, UINT BottomRight);
	bool DivideNode(SNode* pNode);
};

