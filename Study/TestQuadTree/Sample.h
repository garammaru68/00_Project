#pragma once
#include "SCore.h"
#include "SMap.h"
#include "SQuadtree.h"
class SHeightMap :public SMap
{
public:
	float   GetHeight(UINT index) override;
	bool	CreateHeightMap(ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext, const TCHAR* pszFileName);
};
class Sample : public SCore
{
public:
	SHeightMap  m_Map;
	SQuadtree	m_Quadtree;
public:
	bool	Init();
	bool    Render();
};
SGAME_RUN;
