#include "TBasisLib.h"
#include "TButton.h"
/*
  1)클라이언트좌표->투영좌표 변환(x,y=>-1~1, z=0~1)
  2)클라이언트좌표->투영좌표 변환->클라이언트 직교좌표계
	0 ~ 800 -> -1~1 -> -400 ~ 400) 뷰좌표 변환
*/
class Sample : public TBasisLib
{
	TButton    m_Button;
	ID3D11RasterizerState*  m_pRSSolid;
	ID3D11RasterizerState*  m_pRSWireFrame;
public:
	// 게임의 모든 초기화 작업
	bool Init() 
	{
		SetRasterizerState();
		RECT rt = { 10, 10,m_rcWindowClient.right-20,m_rcWindowClient.bottom - 20 };
		m_Button.SetData(rt,0.1f, 0.1f, 
							0.1f, 0.1f,
							0.2f, 0.2f,
							0.33f, 0.33f);
		m_Button.Create(m_pd3dDevice,
			L"UISet.txt",
			L"UISet.txt",
			L"../../data/ui/popup_normal.png");
		return true;
	}
	// 게임의 매 프레임 계산 작업
	bool Frame() 
	{
		m_Button.Frame(0, 0);// m_Timer.GetElapsedTime(), g_fSecPerFrame);
		return true;
	}
	// 게임의 모든 출력 및 드로우 작업
	bool Render() {
		DX::ApplyBS(m_pImmediateContext, DX::TDxState::g_pAlphaBlend);
		if (g_InputData.bEKey)
		{
			m_pImmediateContext->RSSetState(m_pRSWireFrame);
		}
		m_Button.Render(m_pImmediateContext);
		return true;
	}
	// 게임의 모든삭제,소멸작업
	bool Release() 
	{		
		m_Button.Release();
		SAFE_RELEASE(m_pRSWireFrame);
		SAFE_RELEASE(m_pRSSolid);
		return true;
	}
	HRESULT SetRasterizerState(D3D11_FILL_MODE fill = D3D11_FILL_SOLID)
	{
		HRESULT hr;
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;
		desc.DepthClipEnable = TRUE;
		hr = m_pd3dDevice->CreateRasterizerState(&desc, &m_pRSWireFrame);
		desc.FillMode = D3D11_FILL_SOLID;
		hr = m_pd3dDevice->CreateRasterizerState(&desc, &m_pRSSolid);
		return hr;
	}
};
TBASIS_RUN(L"SampleButtonRotation");