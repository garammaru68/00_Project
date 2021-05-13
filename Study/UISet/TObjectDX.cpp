#include "TObjectDX.h"
HRESULT TObjectDX::CreateConstantBuffer(
	ID3D11Device* pd3dDevice,
	void* pData,
	int iSize)
{
	HRESULT hr;
	D3D11_BUFFER_DESC pDesc;
	ZeroMemory(&pDesc, sizeof(D3D11_BUFFER_DESC));
	pDesc.ByteWidth = iSize;
	pDesc.Usage = D3D11_USAGE_DEFAULT;
	pDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA pInitialData;
	ZeroMemory(&pInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	pInitialData.pSysMem = pData;

	if (FAILED(hr = pd3dDevice->CreateBuffer(
		&pDesc,
		NULL,
		&m_pConstantBuffer)))
	{
		return hr;
	}
	return hr;
}
HRESULT TObjectDX::CreateVertexBuffer(
	ID3D11Device* pd3dDevice,
	PCT_VERTEX* pVertexList,
	int iNumCount)
{
	HRESULT hr;
	D3D11_BUFFER_DESC pDesc;
	ZeroMemory(&pDesc, sizeof(D3D11_BUFFER_DESC));
	pDesc.ByteWidth = sizeof(PCT_VERTEX)*iNumCount;
	pDesc.Usage = D3D11_USAGE_DEFAULT;
	pDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA pInitialData;
	ZeroMemory(&pInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	pInitialData.pSysMem = pVertexList;

	if (FAILED(hr = pd3dDevice->CreateBuffer(
		&pDesc,
		&pInitialData,
		&m_pVertexBuffer)))
	{
		return hr;
	}

	m_iNumVertex = iNumCount;
	return hr;
}
HRESULT TObjectDX::CreateVertexShader(
	ID3D11Device* pd3dDevice,
	const TCHAR* pszFileName)
{
	HRESULT hr;
	hr = D3DX11CompileFromFile(pszFileName,
		NULL, NULL,
		// VS함수를 정점쉐이더5.0으로 컴파일 해라.
		"VS", "vs_5_0", 0, 0, NULL,
		&m_pVSBlob, &m_pErrorBlob, NULL);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)m_pErrorBlob->GetBufferPointer());
		return false;
	}

	//4)정점쉐이더 생성
	// 정점버퍼의 정점을 실제 처리하는 곳-->정점쉐이더

	if (FAILED(hr = pd3dDevice->CreateVertexShader(
		m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		return hr;
	}
	return hr;
}
HRESULT TObjectDX::CreatePixelShader(
	ID3D11Device* pd3dDevice,
	const TCHAR* pszFileName)
{
	HRESULT hr;
	hr = D3DX11CompileFromFile(pszFileName,
		NULL, NULL,
		// VS함수를 정점쉐이더5.0으로 컴파일 해라.
		"PS", "ps_5_0", 0, 0, NULL,
		&m_pPSBlob, &m_pErrorBlob, NULL);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)m_pErrorBlob->GetBufferPointer());
		return false;
	}

	//5)정점쉐이더 생성
	// 정점버퍼의 정점을 실제 처리하는 곳-->정점쉐이더

	if (FAILED(hr = pd3dDevice->CreatePixelShader(
		m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(m_pPSBlob);
		return NULL;
	}
	SAFE_RELEASE(m_pPSBlob);
	return hr;
}
HRESULT TObjectDX::CreateLayout(
	ID3D11Device* pd3dDevice,
	ID3DBlob*	pVSBlob)
{
	HRESULT hr;
	if (pVSBlob == NULL) return	E_FAIL;
	//6)레이아웃 설정

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,12,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,28,D3D11_INPUT_PER_VERTEX_DATA, 0 },		
	};
	int iNumElement = sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	if (FAILED(hr = pd3dDevice->CreateInputLayout(
		ied, iNumElement,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&m_pInputlayout)))
	{
		return hr;
	}
	return hr;
}
HRESULT TObjectDX::LoadTexture(
	ID3D11Device* pd3dDevice, 
	const TCHAR* pLoadFile)
{
	HRESULT hr;
	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice,
		pLoadFile, NULL, NULL, &m_pTextureSRV, NULL)))
	{
		return hr;
	}
	return hr;
}

bool	TObjectDX::PreRender(ID3D11DeviceContext* pContext)
{
	//m_pImmediateContext->PSSetSamplers(0, 1, &m_pSS[4]);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->PSSetShader(m_pPixelShader, NULL, NULL);
	pContext->VSSetShader(m_pVertexShader, NULL, NULL);
	UINT pStrides = sizeof(PCT_VERTEX);
	UINT pOffsets = 0;
	//*** 정점버퍼의 처리는 정점1개 단위로 연산된다.
	pContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &pStrides, &pOffsets);
	
	pContext->IASetInputLayout(m_pInputlayout);
	pContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	return true;
}
bool    TObjectDX::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}
bool	TObjectDX::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(m_iNumVertex, 0);
	return true;
}
bool	TObjectDX::Release()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pConstantBuffer);	
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pErrorBlob);
	SAFE_RELEASE(m_pInputlayout);
	SAFE_RELEASE(m_pTextureSRV);
	return true;
}
TObjectDX::TObjectDX()
{
}


TObjectDX::~TObjectDX()
{
}
