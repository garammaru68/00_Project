#include "Sample.h"
bool Sample::PreRender()
{
	SCore::PreRender();
	ID3D11RenderTargetView* pNullRTV = NULL;
	m_pd3dContext->OMSetRenderTargets(1, &pNullRTV, NULL);

	m_pd3dContext->OMSetRenderTargets(
		1, &m_pRenderTargetView,
		m_pDSV);
	m_pd3dContext->ClearDepthStencilView(
		m_pDSV, D3D11_CLEAR_DEPTH |
		D3D11_CLEAR_STENCIL, 1, 0);
	return true;
}
void Sample::SetRasterizerState()
{
	HRESULT hr;
	if (m_pRS != nullptr) m_pRS->Release();
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = m_FillMode;
	rdesc.CullMode = m_CullMode;
	rdesc.DepthClipEnable = TRUE;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRS);
	if (FAILED(hr))
	{
		return;
	}
}
bool Sample::Init()
{
	HRESULT hr;
	m_vCameraPos = { 10,10,-10 };
	m_vCameraTarget = { 0,0,0 };

	Vector3 p = m_vCameraPos;
	Vector3 t = m_vCameraTarget;
	Vector3 u = { 0,1,0 };
	m_matView.CreateViewLook(p, t, u);
	float fN = 1;
	float fF = 1000;
	float fFov = SBASIS_PI / 4.0f;
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_matProj.PerspectiveFovLH(fN, fF, fFov, fAspect);

	// create depth texture
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = g_rtClient.right;
	texDesc.Height = g_rtClient.bottom;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(
		pTexture,
		&dsvDesc,
		&m_pDSV);

	if (pTexture)pTexture->Release();
	if (FAILED(hr))
	{
		return false;
	}
	// DS STATE
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DepthStencilDesc.DepthEnable = TRUE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = m_pd3dDevice->CreateDepthStencilState(
		&DepthStencilDesc, &m_pDSS);
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1;
	samplerDesc.MinLOD = FLT_MIN;
	samplerDesc.MaxLOD = FLT_MAX;
	hr = m_pd3dDevice->CreateSamplerState(&samplerDesc, &m_pWrapLinear);
	if (FAILED(hr))
	{
		return false;
	}

	m_FillMode = D3D11_FILL_SOLID;
	m_CullMode = D3D11_CULL_NONE;
	SetRasterizerState();

	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_SOLID;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSSolidBack);
	if (FAILED(hr))
	{
		return false;
	}

	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSWireBack);
	if (FAILED(hr))
	{
		return false;
	}
	if (!m_Box.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg"))
	{
		return false;
	}
	if (!m_Plane.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg"))
	{
		return false;
	}
	if (!m_Line.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg"))
	{
		return false;
	}
	return true;
}
bool Sample::Frame()
{
	xMatrix matScale;
	xMatrix matRotation;
	matScale.Scale(1, 1, 1);
	matRotation.YRotate(g_fGameTimer);
	m_matBoxWorld = matScale * matRotation;
	m_matPlaneWorld = matScale * matRotation;
	m_matPlaneWorld._41 = 3.0f;
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_vCameraPos.z += 10.0f * g_fSecondPerFrame;
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_vCameraPos.z -= 10.0f * g_fSecondPerFrame;
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_vCameraPos.x += 10.0f * g_fSecondPerFrame;
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_vCameraPos.x -= 10.0f * g_fSecondPerFrame;
	}

	Vector3 u = { 0,1,0 };
	m_matView.CreateViewLook(m_vCameraPos, m_vCameraTarget, u);

	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_WIREFRAME;
		SetRasterizerState();
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_SOLID;
		SetRasterizerState();
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		m_CullMode = D3D11_CULL_BACK;
		SetRasterizerState();
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		m_CullMode = D3D11_CULL_FRONT;
		SetRasterizerState();
	}
	return true;
}
bool Sample::Render()
{
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(m_pRS);
	m_pd3dContext->PSSetSamplers(0, 1, &m_pWrapLinear);
	m_pd3dContext->OMSetDepthStencilState(m_pDSS, 0);
	m_Box.SetMatrix(&m_matBoxWorld, &m_matView, &m_matProj);
	m_Box.Render(m_pd3dContext);

	m_Plane.SetMatrix(&m_matPlaneWorld, &m_matView, &m_matProj);
	m_Plane.Render(m_pd3dContext);

	m_Line.SetMatrix(NULL, &m_matView, &m_matProj);
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(100, 0, 0), Vector4(1, 0, 0, 1));
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 100, 0), Vector4(0, 1, 0, 1));
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 0, 100), Vector4(0, 0, 1, 1));
	return true;
}
bool Sample::Release()
{
	m_pDSV->Release();
	m_pDSS->Release();
	m_pWrapLinear->Release();
	m_pRS->Release();
	m_pRSSolidBack->Release();
	m_pRSWireBack->Release();
	m_Box.Relase();
	m_Plane.Relase();
	m_Line.Relase();
	return true;
}