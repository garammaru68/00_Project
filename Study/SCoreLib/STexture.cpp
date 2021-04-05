#include "STexture.h"

bool STexture::Load(ID3D11Device* pd3dDevice, T_STR szTex)
{
	ID3D11Resource* texture;
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		pd3dDevice, szTex.c_str(),
		NULL,
		&m_pTextureSRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			pd3dDevice, szTex.c_str(),
			NULL,
			&m_pTextureSRV);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}
bool STexture::Init()
{
	return true;
}

bool STexture::Frame()
{
	return true;
}
bool STexture::Release()
{
	if (m_pTextureSRV)
	{
		m_pTextureSRV->Release();
		m_pTextureSRV = nullptr;
	}
	return true;
}
bool STexture::Render()
{
	return true;
}

STexture::STexture()
{
	m_pTextureSRV = nullptr;
}
STexture::~STexture()
{
	Release();
}