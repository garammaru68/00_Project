#include "TBasisLib.h"
#include "TButton.h"
/*
  1)Ŭ���̾�Ʈ��ǥ->������ǥ ��ȯ(x,y=>-1~1, z=0~1)
  2)Ŭ���̾�Ʈ��ǥ->������ǥ ��ȯ->Ŭ���̾�Ʈ ������ǥ��
	0 ~ 800 -> -1~1 -> -400 ~ 400) ����ǥ ��ȯ
*/
class Sample : public TBasisLib
{
	TButton    m_Button;
	ID3D11RasterizerState*  m_pRSSolid;
	ID3D11RasterizerState*  m_pRSWireFrame;
public:
	// ������ ��� �ʱ�ȭ �۾�
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
	// ������ �� ������ ��� �۾�
	bool Frame() 
	{
		m_Button.Frame(0, 0);// m_Timer.GetElapsedTime(), g_fSecPerFrame);
		return true;
	}
	// ������ ��� ��� �� ��ο� �۾�
	bool Render() {
		DX::ApplyBS(m_pImmediateContext, DX::TDxState::g_pAlphaBlend);
		if (g_InputData.bEKey)
		{
			m_pImmediateContext->RSSetState(m_pRSWireFrame);
		}
		m_Button.Render(m_pImmediateContext);
		return true;
	}
	// ������ ������,�Ҹ��۾�
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