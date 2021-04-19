#include ".\sskybox.h"

bool SSkyBox::Create(ID3D11Device* pDevice,
	T_STR szVS, T_STR szPS,
	T_STR	szTex)
{
	if (SObject::Create(pDevice, szVS, szPS, szTex) == false)
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pPSTextrueIndex.Attach(SBASIS_CORE_LIB::LoadPixelShaderFile(pDevice, szPS.c_str(), "PS_TextureIndex"));
	return true;
}
bool	SSkyBox::LoadTexture(T_STR szTex)
{
	HRESULT hr = S_OK;
	//const TCHAR* g_szSkyTextures[] =
	//{
	//	L"..\\..\\data\\sky\\ft.bmp",
	//	L"..\\..\\data\\sky\\bk.bmp",
	//	L"..\\..\\data\\sky\\rt.bmp",
	//	L"..\\..\\data\\sky\\lt.bmp",
	//	L"..\\..\\data\\sky\\up.bmp",
	//	L"..\\..\\data\\sky\\dn.bmp"
	//};
	const TCHAR* g_szSkyTextures[] =
	{
		L"..\\..\\data\\sky\\st00_cm_front.bmp",
		L"..\\..\\data\\sky\\st00_cm_back.bmp",
		L"..\\..\\data\\sky\\st00_cm_right.bmp",
		L"..\\..\\data\\sky\\st00_cm_left.bmp",
		L"..\\..\\data\\sky\\st00_cm_up.bmp",
		L"..\\..\\data\\sky\\st00_cm_down.bmp"
	};
	int iNumTexture = sizeof(g_szSkyTextures) / sizeof(g_szSkyTextures[0]);

	for (int iTex = 0; iTex < iNumTexture; iTex++)
	{
		m_pTexSRV[iTex].Attach(SBASIS_CORE_LIB::CreateShaderResourceView(
			g_pd3dDevice,
			g_pImmediateContext, g_szSkyTextures[iTex]));
	}
	m_pTexCubeSRV.Attach(SBASIS_CORE_LIB::CreateShaderResourceView(
		g_pd3dDevice,
		g_pImmediateContext, L"..\\..\\data\\sky\\grassenvmap1024.dds"));
	return true;
}

bool SSkyBox::Render(ID3D11DeviceContext*    pContext)
{
	m_matWorld = Matrix::CreateScale(100.0f, 100.0f, 100.0f);
	Matrix matSkyBoxView = m_matView;
	matSkyBoxView._41 = 0;
	matSkyBoxView._42 = 0;
	matSkyBoxView._43 = 0;

	g_pImmediateContext->RSSetState(SDxState::g_pRSNoneCullSolid);
	g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::g_pSSClampPoint);
	g_pImmediateContext->OMSetDepthStencilState(SDxState::g_pDSSDepthDisable, 0);
	/*TBASIS_CORE_LIB::ApplyDSS(pContext,
		TBASIS_CORE_LIB::SDxState::g_pDSSDepthDisable);

	ID3D11SamplerState* ppSamplerStates[2] = {
		SDxState::g_pSSClampPoint,
		SDxState::g_pSSWrapLinear };
	pContext->PSSetSamplers(0, 2, ppSamplerStates);*/

	SetMatrix(&m_matWorld, &matSkyBoxView, &m_matProj);
	SDxObject::Render(g_pImmediateContext);
	return true;
}
bool SSkyBox::PostRender(ID3D11DeviceContext*	pd3dContext)
{
	g_pImmediateContext->PSSetShaderResources(1, 1, m_pTexCubeSRV.GetAddressOf());

	if (m_bRenderType)
	{
		//--------------------------------------------------------------------------------------
		// 한장씩 랜더링 하는 방법
		//--------------------------------------------------------------------------------------
		for (int iTex = 0; iTex < MAX_SKYBOX_TEXTURE; iTex++)
		{
			if (m_pTexSRV[iTex] == nullptr) break;
			g_pImmediateContext->PSSetShaderResources(0, 1, m_pTexSRV[iTex].GetAddressOf());
			g_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
			// 랜더링에 사용할 인덱스 버퍼 갯수, 인덱스 버퍼 시작, 버텍스 버퍼 시작
			g_pImmediateContext->DrawIndexed(6, 6 * iTex, 0);
		}
	}
	else
	{
		//g_pImmediateContext->PSSetShaderResources(1, 6, m_pTexSRV[0].GetAddressOf());
		g_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
		// 랜더링에 사용할 인덱스 버퍼 갯수, 인덱스 버퍼 시작, 버텍스 버퍼 시작
		g_pImmediateContext->DrawIndexed(36, 0, 0);
	}
	return true;
}
SSkyBox::SSkyBox()
{
	m_bRenderType = false;
}
SSkyBox::~SSkyBox()
{
}