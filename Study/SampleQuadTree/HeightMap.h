#pragma once
#include "SMap.h"
class HeightMap :public SMap
{
public:
	float   GetHeight(UINT index) override;
	bool	CreateHeightMap(ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext, const TCHAR* pszFileName);
};

