#pragma once
#include "TStd.h"
#include "TShape.h"
class TFrustum 
{
public:
	TShapeBox  m_FrustumObj;
	std::vector<PNCT_VERTEX>	m_VertexList;
public:
	virtual bool Create(ID3D11Device* pd3dDevice);
};

