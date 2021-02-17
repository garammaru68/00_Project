#include "SShape.h"
void SShape::CompilerCheck(ID3DBlob* pErrorMsgs)
{
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}
bool	SShape::Init()
{
	m_matWorld = Matrix::Identity;
	m_matView = Matrix::Identity;
	m_matProj = Matrix::Identity;
	return true;
}
bool	SShape::Frame() {
	return true;
}
bool	SShape::SetMatrix(Matrix* pWorld,
	Matrix* pView,
	Matrix* pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
	}
	if (pView != nullptr)
	{
		m_matView = *pView;
	}
	if (pProj != nullptr)
	{
		m_matProj = *pProj;
	}
	return true;
}
bool    SShape::Update(ID3D11DeviceContext*	pd3dContext)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	HRESULT hr = pd3dContext->Map(m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		SDataCB* pData = (SDataCB*)mr.pData;
		pData->matWorld = m_matWorld.Transpose();
		pData->matView = m_matView.Transpose();
		pData->matProject = m_matProj.Transpose();

		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer)*0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		pd3dContext->Unmap(m_pConstantBuffer, 0);
		return true;
	}
	return false;
}
bool	SShape::Render(ID3D11DeviceContext*	pd3dContext)
{
	Update(pd3dContext);
	UINT iStride = sizeof(PNCT_VERTEX);
	UINT iOffset = 0;
	pd3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->IASetInputLayout(m_pInputLayout);
	pd3dContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pd3dContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	pd3dContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	//pd3dContext->Draw(m_VertexList.size(), 0);
	pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
bool	SShape::Relase()
{
	m_pTextureSRV->Release();
	m_pConstantBuffer->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;
}

SShape::SShape()
{
	m_szVertexShader = "VS";
	m_szPixelShader = "PS";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
SShape::~SShape()
{

}
bool    SShape::CreateVertexData() {
	return true;
};

bool    SShape::CreateIndexData() {
	return true;
};
bool    SShape::CreateConstantBuffer()
{
	// constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(SDataCB);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cbData;
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	SShape::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	SShape::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool SShape::LoadShader(T_STR szVS, T_STR szPS)
{
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsgs;
	HRESULT hr = D3DCompileFromFile(szVS.c_str(), NULL, NULL,
		m_szVertexShader.c_str(), "vs_5_0", 0, 0, &m_pVSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	hr = m_pd3dDevice->CreateVertexShader(m_pVSObj->GetBufferPointer(), m_pVSObj->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr)) return false;

	hr = D3DCompileFromFile(szPS.c_str(), NULL, NULL,
		m_szPixelShader.c_str(), "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(hr)) return false;

	if (pPSObj)	pPSObj->Release();
	return true;
}
bool	SShape::LoadTexture(T_STR szTex)
{
	// load texture
	ID3D11Resource* texture;
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice, szTex.c_str(),
		NULL,
		&m_pTextureSRV);
	if (FAILED(hr)) return false;
	return true;
}
bool	SShape::CreateInputLayout()
{
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = m_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		m_pVSObj->GetBufferPointer(),
		m_pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	if (FAILED(hr)) return false;
	return true;
}
bool	SShape::Create(ID3D11Device* pDevice,
	T_STR szVS, T_STR szPS,
	T_STR	szTex)
{
	m_pd3dDevice = pDevice;

	CreateVertexData();
	CreateConstantBuffer();
	CreateVertexBuffer();
	CreateIndexData();
	CreateIndexBuffer();
	LoadShader(szVS, szPS);
	CreateInputLayout();
	LoadTexture(szTex);
	return true;
}
bool	SShapeBox::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(24);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));
	// µÞ¸é
	m_VertexList[4] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	m_VertexList[8] = PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

	// ¿ÞÂÊ
	m_VertexList[12] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));

	// À­¸é
	m_VertexList[16] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

	// ¾Æ·§¸é
	m_VertexList[20] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));
	return true;
}
bool    SShapeBox::CreateIndexData()
{
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;
	return true;
}
SShapeBox::SShapeBox()
{

}
SShapeBox::~SShapeBox()
{

}

bool SShapePlane::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(4);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));
	return true;
}

bool SShapePlane::CreateIndexData()
{
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 3;
	return true;
}

SShapePlane::SShapePlane()
{
}

SShapePlane::~SShapePlane()
{
}

bool SShapeLine::Draw(ID3D11DeviceContext* pd3dContext,
	Vector3 p, Vector3 e, Vector4 c)
{
	m_VertexList[0] = PNCT_VERTEX(p, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(e, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(1.0f, 0.0f));
	pd3dContext->UpdateSubresource(
		m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return SShape::Render(pd3dContext);
}
bool SShapeLine::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(2);
	m_VertexList[0] = PNCT_VERTEX(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(100.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	return true;
}

bool SShapeLine::CreateIndexData()
{
	m_IndexList.resize(2);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	return true;
}

SShapeLine::SShapeLine()
{
	m_szPixelShader = "PSLine";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

SShapeLine::~SShapeLine()
{
}
