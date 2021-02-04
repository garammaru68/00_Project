#include "Sample.h"
void Sample::SetRasterizerState()
{
	HRESULT hr;
	// Rasterizer State
	if( m_pRS != nullptr) m_pRS->Release();
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = m_FillMode;
	rdesc.CullMode = m_CullMode;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRS);
	if (FAILED(hr))
	{
		return;
	}
}
void Sample::CompilerCheck(ID3DBlob* pErrorMsgs)
{
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}
bool Sample::Init()
{
	HRESULT hr;
	// load texture
	ID3D11Resource* texture;
	hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice, L"../../data/main_start_nor.png",
		NULL,
		&m_pTextureSRV);
}