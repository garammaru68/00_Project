#include "SDxRT.h"
void	SDxRT::Set(ID3D11Device* pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
}
HRESULT		SDxRT::SetRenderTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = 512;
	texDesc.Height = 512;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | 
		D3D11_BIND_SHADER_RESOURCE;
	hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_pd3dDevice->CreateRenderTargetView(
		pTexture, NULL,
		&m_pRTV);

	hr = m_pd3dDevice->CreateShaderResourceView(pTexture,
		NULL,
		&m_pSRV);

	if (pTexture) pTexture->Release();
	return hr;
}