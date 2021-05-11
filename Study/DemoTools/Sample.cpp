#include "pch.h"
#include "Sample.h"
/*
#include <d3d11_x.h> 연결하면 mfc 오류가 생긴다.
#include <dxgix.h> 연결하면 mfc 오류가 생긴다.
winsdk는 8.1로 하여 컴파일 한다.
*/
bool Sample::Init()
{
	m_bCreateMap = false;
	//if (FAILED(m_LineDraw.Create(g_pd3dDevice, L"../../data/shader/line.hlsl")))
	//{
	//	MessageBox(0, _T("m_LineDraw 실패"), _T("Fatal error"), MB_OK);
	//	return 0;
	//}

	if (FAILED(CreateResource()))
	{
		return false;
	}
	//--------------------------------------------------------------------------------------
	// 카메라  행렬 
	//--------------------------------------------------------------------------------------
	SAFE_NEW(m_pMainCamera, SCamera);
	m_pMainCamera->CreateViewMatrix(Vector3(0.0f, 300.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f));
	m_pMainCamera->CreateProjMatrix(SBASIS_PI * 0.25f,
		m_pSwapChainDesc.BufferDesc.Width / (float)(m_pSwapChainDesc.BufferDesc.Height),
		1.0f, 1000.0f);
	return true;
}
bool Sample::CreateMap(SMapInfo& info)
{

	//SMapDesc MapDesc = { 50, 50, 1.0f, 0.1f,
	//	L"../../data/map/002.jpg",
	//	L"../../data/shader/CustomizeMap.hlsl" };
	//MapDesc.iNumCols = info.iNumTile*info.iNumCell + 1;
	//MapDesc.iNumRows = info.iNumTile*info.iNumCell + 1;
	//MapDesc.fCellDistance = info.fCellSize;

	SMapDesc desc;
	desc.iNumCols = info.iNumTile*info.iNumCell + 1;
	desc.iNumRows = info.iNumTile*info.iNumCell + 1;
	desc.fCellDistance = info.fCellSize;
	//desc.iNumCols = m_CustomMap.m_iNumCols;
	//desc.iNumRows = m_CustomMap.m_iNumRows;
	//desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/castle.jpg";
	desc.szVS = L"../../data/shader/CustomizeMap.hlsl";
	desc.szPS = L"../../data/shader/CustomizeMap.hlsl";
	//desc.szVS = L"../../data/shader/VS.txt";
	//desc.szPS = L"../../data/shader/PS.txt";

	m_CustomMap.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);

//	m_CustomMap.Init(g_pd3dDevice, g_pImmediateContext);
	if (FAILED(m_CustomMap.CreateMap(g_pd3dDevice, g_pImmediateContext, desc)))
	{
		return false;
	}
	m_bCreateMap = true;
	return true;
}
bool Sample::Render()
{
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		SDxState::g_RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		SDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, SDxState::g_RasterizerDesc);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		SDxState::g_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		SDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, SDxState::g_RasterizerDesc);
	}
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->RSSetState(SDxState::g_pRS[2]);
	g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::g_pSSWrapLinear);
	g_pImmediateContext->OMSetDepthStencilState(SDxState::g_pDSS[0], 0);
	m_CustomMap.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	//m_Map.Render(g_pImmediateContext);

	return m_Quadtree.Render(g_pImmediateContext);


	//if (m_bCreateMap == false) return true;
	//DX::ApplyDSS(g_pImmediateContext, DX::SDxState::g_pDSSDepthEnable);
	//DX::ApplyBS(g_pImmediateContext, DX::SDxState::g_pAlphaBlend);
	//m_CustomMap.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	//return m_CustomMap.Render(g_pImmediateContext);
}
bool Sample::Release()
{
	//SAFE_RELEASE(m_pMainCamera);
	SAFE_DEL(m_pMainCamera);
	//m_LineDraw.Release();
	return m_CustomMap.Release();
}
bool Sample::Frame()
{
	if (m_bCreateMap == false) return true;
	m_pMainCamera->Frame();
	return m_CustomMap.Frame();
}
bool Sample::DrawDebug()
{
	DX::ApplyDSS(g_pImmediateContext, DX::SDxState::g_pDSSDepthDisable);

	//m_LineDraw.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	//Vector3 vStart(0, 0, 0);
	//Vector3 vEnd(1000, 0, 0);
	//m_LineDraw.Draw(g_pImmediateContext, vStart, vEnd, Vector4(1, 0, 0, 1.0f));
	//vEnd = Vector3(0, 1000, 0);
	//m_LineDraw.Draw(g_pImmediateContext, vStart, vEnd, Vector4(0, 1, 0, 1.0f));
	//vEnd = Vector3(0, 0, 1000);
	//m_LineDraw.Draw(g_pImmediateContext, vStart, vEnd, Vector4(0, 0, 1, 1.0f));
	//if (!SCore::DrawDebug()) return false;
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::CreateResource()
{
	HRESULT hr = S_OK;
	if (m_pMainCamera)
	{
		float fAspectRatio = m_pSwapChainDesc.BufferDesc.Width /
			(float)m_pSwapChainDesc.BufferDesc.Height;
		m_pMainCamera->CreateProjMatrix(SBASIS_PI / 4, fAspectRatio, 0.1f, 500.0f);
	}
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::DeleteResource()
{
	HRESULT hr = S_OK;
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	return S_OK;
}


Sample::Sample()
{
	m_pMainCamera = nullptr;
}


Sample::~Sample()
{
}
