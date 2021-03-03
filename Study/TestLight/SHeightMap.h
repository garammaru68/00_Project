#pragma once
#include "SMap.h"
class SHeightMap :public SMap
{
public:
	//float	GetHeight;
	float   GetHeightMap(int row, int col) override;
	bool	CreateHeightMap(ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext, const TCHAR* pszFileName);
};

