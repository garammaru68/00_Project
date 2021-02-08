#include "Sample.h"
bool Sample::PreRender()
{
	SCore::PreRender();
	ID3D11RenderTargetView* pNullRTV = NULL;
	m_pd3dContext->OMSetRenderTargets(1, &pNullRTV, NULL);

	m_pd3dContext->OMSetRenderTargets(
		1, 
		&m_pRenderTargetView, 
		m_pDSV);
	m_pd3dContext->ClearDepthStencilView(
		m_pDSV, D3D11_CLEAR_DEPTH |
		D3D11_CLEAR_STENCIL, 1, 0);
	return true;
}
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
	// create depth texture
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width				= g_rtClient.right;
	texDesc.Height				= g_rtClient.bottom;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &pTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice	= 0;
	hr = m_pd3dDevice->CreateDepthStencilView(
		pTexture,
		&dsvDesc,
		&m_pDSV);
	// DS STATE
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DepthStencilDesc.DepthEnable	= TRUE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc		= D3D11_COMPARISON_LESS_EQUAL;

	hr = m_pd3dDevice->CreateDepthStencilState(
						&DepthStencilDesc, &m_pDSS);
	// load texture
	ID3D11Resource* texture;
	hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice, L"../../data/main_start_nor.png",
		NULL,
		&m_pTextureSRV);
	
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0]	= 1;
	samplerDesc.BorderColor[1]	= 0;
	samplerDesc.BorderColor[2]	= 0;
	samplerDesc.BorderColor[3]	= 1;
	samplerDesc.MinLOD			= FLT_MIN;
	samplerDesc.MaxLOD			= FLT_MAX;
	hr = m_pd3dDevice->CreateSamplerState(&samplerDesc, &m_pWrapLinear);


	// CreateWICTextureFromFileEx();
	// Rasterizer State
	m_FillMode = D3D11_FILL_SOLID;
	m_CullMode = D3D11_CULL_BACK;
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

	// Vertex Data
	m_VertexList.resize(4);
	m_VertexList[0] = {
		SVertex3(-1.0f, 1.0f, -1.0f),
		SVertex3(0, 0, 0),
		SVertex4(1, 0, 0, 1),
		SVertex2(0, 0) };
	m_VertexList[1] = {
		SVertex3(1.0f, 1.0f, -1.0f),
		SVertex3(0, 0, 0),
		SVertex4(0, 1, 0, 1),
		SVertex2(3, 0) };
	m_VertexList[2] = {
		SVertex3(-1.0f, -1.0f, -1.0f),
		SVertex3(0, 0, 0),
		SVertex4(0, 0, 1, 1),
		SVertex2(0, 3) };
	m_VertexList[3] = {
		SVertex3(1.0f, -1.0f, -1.0f),
		SVertex3(0, 0, 0),
		SVertex4(1, 1, 1, 1),
		SVertex2(3, 3) };

	//P_VERTEX v = { TVertex3(-0.5f, 0.5f, 0.5f), TVertex3(0,0,0) };
	//m_VertexList.emplace(m_VertexList.end(), v);
	//m_VertexList.emplace_back(TVertex3(0.5f, 0.5f, 0.5f),
	//							TVertex3(0.0f, 0.0f, 0.0f));
	//m_VertexList.emplace_back(TVertex3(-0.5f, -0.5f, 0.5f));
	//m_VertexList.emplace_back(TVertex3(0.5f, -0.5f, 0.5f));

	// constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth		= sizeof(SDataCB);
	bd.Usage			= D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cbData;
	hr = m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	// vertex buffer
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth	= sizeof(P_VERTEX) * m_VertexList.size();
	bd.Usage		= D3D11_USAGE_DEFAULT;
	bd.BindFlags	= D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;

	// index buffer
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth	= sizeof(DWORD) * m_IndexList.size();
	bd.Usage		= D3D11_USAGE_DEFAULT;
	bd.BindFlags	= D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	ID3DBlob* pVSObj;
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsgs;
	hr = D3DCompileFromFile(L"vs.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, &pVSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	hr = m_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL, &m_pVertexShader);
	hr = D3DCompileFromFile(L"PS.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);

	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, &m_pPixelShader);

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	hr = m_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	return true;
}
bool Sample::Frame()
{
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_WIREFRAME;
		SetRasterizerState();
		//m_pd3dContext->RSSetState(m_pRSWireBack);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_SOLID;
		SetRasterizerState();
		//m_pd3dContext->RSSetState(m_pRSSolidBack);
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		m_CullMode = D3D11_CULL_BACK;
		SetRasterizerState();
		//m_pd3dContext->RSSetState(m_pRSWireBack);
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		m_CullMode = D3D11_CULL_FRONT;
		SetRasterizerState();
		//m_pd3dContext->RSSetState(m_pRSSolidBack);
	}
	D3D11_MAPPED_SUBRESOURCE mr;
	HRESULT hr = m_pd3dContext->Map(m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		SDataCB* pData = (SDataCB*)mr.pData;
		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer)*0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		m_pd3dContext->Unmap(m_pConstantBuffer, 0);
	}
	return true;
}
bool Sample::Render()
{
	UINT iStride = sizeof(P_VERTEX);
	UINT iOffset = 0;
	m_pd3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pd3dContext->IASetInputLayout(m_pInputLayout);
	m_pd3dContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pd3dContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(m_pRS);
	m_pd3dContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	m_pd3dContext->PSSetSamplers(0, 1, &m_pWrapLinear);
	m_pd3dContext->OMSetDepthStencilState(m_pDSS, 0);
	//m_pd3dContext->Draw(m_VertexList.size(), 0);
	m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
bool Sample::Release()
{
	m_pDSV->Release();
	m_pDSS->Release();
	m_pWrapLinear->Release();
	m_pTextureSRV->Release();
	m_pRS->Release();
	m_pRSSolidBack->Release();
	m_pRSWireBack->Release();
	m_pConstantBuffer->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;

}