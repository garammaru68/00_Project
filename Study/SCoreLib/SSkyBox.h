#pragma once
#include "SShape.h"

#define MAX_SKYBOX_TEXTURE	6

class SSkyBox : public SShapeBox
{
public:
	ComPtr<ID3D11ShaderResourceView>	m_pTexSRV[MAX_SKYBOX_TEXTURE];
	ComPtr<ID3D11ShaderResourceView>	m_pTexCubeSRV;
	ComPtr<ID3D11PixelShader>			m_pPSTextrueIndex;
	bool								m_bRenderType;
public:
	bool				Create(ID3D11Device* pDevice,
		T_STR szVS, T_STR szPS,
		T_STR	szTex);
	bool				Render(ID3D11DeviceContext*    pContext);
	bool	LoadTexture(T_STR szTex);
	bool	PostRender(ID3D11DeviceContext*	pd3dContext);
public:
	SSkyBox(void);
	~SSkyBox(void);
};


