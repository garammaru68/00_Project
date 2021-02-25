#pragma once
#include "SDxRT.h"
#include "SShape.h"
class SMiniMap : public SShapePlane
{
public:
	SShapePlane  m_Plane;
	SDxRT		 m_DxRT;
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
	virtual bool	Create(ID3D11Device* pDevice,
		T_STR szVS, T_STR szPS,
		T_STR	szTex);
	virtual bool Begin(ID3D11DeviceContext*	pd3dContext);
	virtual bool End(ID3D11DeviceContext*	pd3dContext);
	virtual bool Release() override;
	virtual bool Render(ID3D11DeviceContext*	pd3dContext) override;
	virtual bool PostRender(ID3D11DeviceContext*	pd3dContext) override;

};

