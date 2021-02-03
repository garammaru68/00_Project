#include "Sample.h"

void Sample::CompilerCheck(ID3DBlob* pErrorMsgs)
{
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}
bool Sample::Init()
{
	m_VertexList.resize(8);
	m_VertexList[0] = { SVertex3(-1.0f, 1.0f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[1] = { SVertex3(0.0f, 1.0f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[2] = { SVertex3(-1.0f, 0.0f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[3] = { SVertex3(0.0f, 0.0f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[4] = { SVertex3(0.0f, 0.0f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[5] = { SVertex3(1.0f, 0.0f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[6] = { SVertex3(0.0f, -1.0f, 0.5f), SVertex3(0,0,0) };
	m_VertexList[7] = { SVertex3(1.0f, -1.0f, 0.5f), SVertex3(0,0,0) };

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(P_VERTEX) * m_VertexList.size();
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
	m_IndexList.resize(12);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;
	m_IndexList[6] = 4;
	m_IndexList[7] = 5;
	m_IndexList[8] = 6;
	m_IndexList[9] = 6;
	m_IndexList[10] = 5;
	m_IndexList[11] = 7;

	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

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
	hr = D3DCompileFromFile(L"vs.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, &m_pPixelShader);

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "POSITION1", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
bool Sample::Render()
{
	UINT iStride = sizeof(P_VERTEX);
	UINT iOffset = 0;
	m_pd3dContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &iStride, &iOffset);
	m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R32_UINT, 0);
	m_pd3dContext->IASetInputLayout(m_pInputLayout);
	m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
bool Sample::Release()
{
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;
}