#pragma once
#include "SStd.h"
#include "dxtk/WICTextureLoader.h"
#include "dxtk/DDSTextureLoader.h"
#include "dxtk/DDS.h"

class STexture
{
public:
	ID3D11ShaderResourceView* m_pTextureSRV;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	bool Load(ID3D11Device* pd3dDevice, T_STR szTex);
public:
	STexture();
	virtual ~STexture();
};