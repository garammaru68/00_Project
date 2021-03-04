#pragma once
#include "SCore.h"
#include "SMap.h"
#include "SQuadtree.h"
class SHeighSMap :public SMap
{
public:
	float   GetHeightOfVertex(UINT index) override;
	bool	CreateHeighSMap(ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext, const TCHAR* pszFileName);
};
class Sample : public SCore
{
public:
	SHeighSMap  m_Map;
	SQuadtree	m_Quadtree;
public:
	bool	Init();
	bool    Render();
};
SGAME_RUN;
