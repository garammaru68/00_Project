#pragma once
#include "SStd.h"
#include "SShape.h"
class SFrustum
{
public:
	SShapeBox  m_FrustumObj;
	std::vector<PNCT_VERTEX>	m_VertexList;
public:
	virtual bool Create(ID3D11Device* pd3dDevice);
};

