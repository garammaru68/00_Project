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
	SNode()
	{
		m_pChild[0] = nullptr;
		m_pChild[1] = nullptr;
		m_pChild[2] = nullptr;
		m_pChild[3] = nullptr;

		m_dwDepth = 0;
		m_blLeaf = FALSE;
	}
	~SNode()
	{
		if (m_pChild[0] != nullptr)
		{
			for (int iChild = 0; iChild < 4; iChild++)
			{
				delete m_pChild[iChild];
			}
		}
	}
};