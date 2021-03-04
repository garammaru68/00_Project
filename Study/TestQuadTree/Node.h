#pragma once
#include "SDxObject.h"

class Node
{
public:
	DWORD	m_dwDepth;
	BOOL	m_blLeaf;

	Node*					m_pChild;
	
	vector<UINT>			m_vCornerList;
	vector<DWORD>			m_vIndexList;
	vector<PNCT_VERTEX>		m_vVertexList;
	ComPtr<ID3D11Buffer>	m_vIndexBuffer;
	ComPtr<ID3D11Buffer>	m_vVertexBuffer;

public:
	Node();
	virtual ~Node();
};

