#include "TMiniMap.h"
bool	TMiniMap::Create(ID3D11Device* pDevice,
	T_STR szVS, T_STR szPS,
	T_STR	szTex)
{
	m_DxRT.Set(pDevice);
	m_pd3dDevice = pDevice;
	m_DxRT.SetRenderTargetView();
	m_DxRT.SetDepthStencilView();
	m_DxRT.SetViewport();
	TDxObject::Create(pDevice, szVS, szPS, szTex);
	return true;
}
bool    TMiniMap::CreateVertexData()
{
	m_VertexList.resize(4);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(-0.5f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(-0.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

	return true;
}
bool    TMiniMap::CreateIndexData()
{
	TShapePlane::CreateIndexData();
	return true;
}
bool  TMiniMap::Release()
{
	m_DxRT.Release();
	return TObject::Release();
}
bool TMiniMap::Begin(ID3D11DeviceContext*	pd3dContext)
{
	m_DxRT.Begin(pd3dContext); 
	return true;
}
bool TMiniMap::End(ID3D11DeviceContext*	pd3dContext)
{
	m_DxRT.End(pd3dContext);
	return true;
}
bool TMiniMap::Render(ID3D11DeviceContext*	pd3dContext)
{
	TShapePlane::Render(pd3dContext);
	return true;
}

bool TMiniMap::PostRender(ID3D11DeviceContext*	pd3dContext)
{
	pd3dContext->PSSetShaderResources(0, 1,&m_DxRT.m_pSRV);
	TObject::PostRender(pd3dContext);
	m_DxRT.ClearShaderResources(pd3dContext);
	return true;
}