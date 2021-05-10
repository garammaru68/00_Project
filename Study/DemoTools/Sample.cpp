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
	if (FAILED(m_LineDraw.Create(g_pd3dDevice, L"../../data/shader/line.hlsl")))
	{
		MessageBox(0, _T("m_LineDraw 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	if (FAILED(CreateResource()))
	{
		return false;
	}
	//--------------------------------------------------------------------------------------
	// 카메라  행렬 
	//--------------------------------------------------------------------------------------
	SAFE_NEW(m_pMainCamera, SCamera);
	m_pMainCamera->CreateViewMatrix(Vector3(0.0f, 30.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f));
	m_pMainCamera->CreateProjMatrix(SBASIS_PI * 0.25f,
		m_SwapChainDesc.BufferDesc.Width / (float)(m_SwapChainDesc.BufferDesc.Height),
		1.0f, 1000.0f);
	return true;
}
bool Sample::CreateMap(TMapInfo& info)
{

	TMapDesc MapDesc = { 50, 50, 1.0f, 0.1f,
		L"../../data/map/006.jpg",
		L"../../data/Shader/CustomizeMap.hlsl" };
	MapDesc.iNumCols = info.iTileCnt*info.iCellCnt + 1;
	MapDesc.iNumRows = info.iTileCnt*info.iCellCnt + 1;
	MapDesc.fCellDistance = info.fCellSize;

	m_CustomMap.Init(GetDevice(), m_pImmediateContext);
	if (FAILED(m_CustomMap.Load(MapDesc)))
	{
		return false;
	}
	m_bCreateMap = true;
	return true;
}
bool Sample::Render()
{
	if (m_bCreateMap == false) return true;
	DX::ApplyDSS(m_pImmediateContext, DX::TDxState::g_pDSSDepthEnable);
	DX::ApplyBS(m_pImmediateContext, DX::TDxState::g_pAlphaBlend);
	m_CustomMap.SetMatrix(m_pMainCamera->GetWorldMatrix(), m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	return m_CustomMap.Render(m_pImmediateContext);
}
bool Sample::Release()
{
	SAFE_RELEASE(m_pMainCamera);
	SAFE_DEL(m_pMainCamera);
	m_LineDraw.Release();
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
	DX::ApplyDSS(m_pImmediateContext, DX::TDxState::g_pDSSDepthDisable);
	m_LineDraw.SetMatrix(NULL, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	D3DXVECTOR3 vStart(0, 0, 0);
	D3DXVECTOR3 vEnd(1000, 0, 0);
	m_LineDraw.Draw(m_pImmediateContext, vStart, vEnd, D3DXVECTOR4(1, 0, 0, 1.0f));
	vEnd = D3DXVECTOR3(0, 1000, 0);
	m_LineDraw.Draw(m_pImmediateContext, vStart, vEnd, D3DXVECTOR4(0, 1, 0, 1.0f));
	vEnd = D3DXVECTOR3(0, 0, 1000);
	m_LineDraw.Draw(m_pImmediateContext, vStart, vEnd, D3DXVECTOR4(0, 0, 1, 1.0f));
	if (!TBasisLib::DrawDebug()) return false;
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
		float fAspectRatio = m_SwapChainDesc.BufferDesc.Width /
			(float)m_SwapChainDesc.BufferDesc.Height;
		m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 500.0f);
	}
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::DeleteResource()
{
	HRESULT hr = S_OK;
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	return S_OK;
}


Sample::Sample()
{
	m_pMainCamera = nullptr;
}


Sample::~Sample()
{
}
