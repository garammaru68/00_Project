#pragma once
#include "SMap.h"
struct SNode
{
	DWORD   dwDepth;
	BOOL	isLeaf;
	UINT    iEdgeIndex[4];
	SNode*  pChild[4];
	std::vector<DWORD>      m_IndexList;
	ComPtr<ID3D11Buffer>	pIndexBuffer;
	SNode()
	{
		pChild[0] = nullptr;
		pChild[1] = nullptr;
		pChild[2] = nullptr;
		pChild[3] = nullptr;
	}
	~SNode()
	{
		if (pChild[0] != nullptr)
		{
			for (int iChild = 0; iChild < 4; iChild++)
			{
				delete pChild[iChild];
			}
		}
	}
};
class SQuadtree
{
	SNode* m_pRootNode;
	std::vector<SNode*>  m_leafList;
	std::vector<SNode*>  m_visibleList;
public:
	SMap*  m_pMap;
	bool   CreateQuadtree(SMap* pMap);
	bool   BuildTree();
	SNode*  CreateNode(SNode* pParentNode, UINT tr, UINT tl, UINT br, UINT bl);
	bool    BuildQuadtree(SNode* pNode);
	bool    SpliteChild(SNode* pNode);
	bool    Render(ID3D11DeviceContext* pContext);
	bool	Release();
	void    Draw(SNode* pNode, ID3D11DeviceContext* pd3dContext);
};

