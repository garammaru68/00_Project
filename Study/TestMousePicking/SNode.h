#pragma once
#include "SFrustum.h"

class SNode
{
public:
	DWORD					m_dwDepth;
	BOOL					m_blLeaf;
	UINT					m_iCornerIndex[4];
	SNode*					m_pChild[4];
	std::vector<DWORD>		m_vIndexList;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:
	SNode();
	~SNode();
};