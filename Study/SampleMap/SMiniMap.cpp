#include "SMiniMap.h"
bool	SMiniMap::Create(ID3D11Device* pDevice,
	T_STR szVS, T_STR szPS,
	T_STR	szTex)
{
	m_DxRT.Set(pDevice);
	g_pd3dDevice = pDevice;
	m_DxRT.SetRenderTargetView();
	m_DxRT.SetDepthStencilView();
	m_DxRT.SetViewport();
	SDxObject::Create(pDevice, szVS, szPS, szTex);
	return true;
}
bool    SMiniMap::CreateVertexData()
{
	m_VertexList.resize(4);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(-0.5f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(-0.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

	return true;
}
bool    SMiniMap::CreateIndexData()
{
	SShapePlane::CreateIndexData();
	return true;
}
bool  SMiniMap::Release()
{
	m_DxRT.Release();
	return SObject::Release();
}
bool SMiniMap::Begin(ID3D11DeviceContext*	pd3dContext)
{
	m_DxRT.Begin(pd3dContext);
	return true;
}
bool SMiniMap::End(ID3D11DeviceContext*	pd3dContext)
{
	m_DxRT.End(pd3dContext);
	return true;
}
bool SMiniMap::Render(ID3D11DeviceContext*	pd3dContext)
{
	SShapePlane::Render(pd3dContext);
	return true;
}

bool SMiniMap::PostRender(ID3D11DeviceContext*	pd3dContext)
{
	pd3dContext->PSSetShaderResources(0, 1, &m_DxRT.m_pSRV);
	SObject::PostRender(pd3dContext);
	m_DxRT.ClearShaderResources(pd3dContext);
	return true;
}