#pragma once
#include "TDxRT.h"
#include "TShape.h"
class TMiniMap : public TShapePlane
{
public:
	TShapePlane  m_Plane;
	TDxRT		 m_DxRT;
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

