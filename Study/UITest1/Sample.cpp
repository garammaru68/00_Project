#include "Sample.h"
#define WM_TEXT (WM_USER+1)

int		 Sample::WndProc(HWND hWnd, UINT msg, WPARAM wData,
	LPARAM lData)
{
	if (WM_TEXT == msg)
	{
		UINT width = LOWORD(lData);
		UINT height = HIWORD(lData);
	}
	return I_Ime.WndProc(hWnd, msg, wData, lData);
}
SControlUI* Sample::AddRect(TUI_TYPE type)
{
	SControlUI* pUIControl = NULL;
	switch (type)
	{
	case T_BUTTON: {
		pUIControl = new SButtonCtl(); 
		pUIControl->Create(g_pd3dDevice, nullptr, L"../../data/ui/exit_lg.bmp");
		pUIControl->Scale(50, 50, 0);
	}
	case T_EDIT: {
		pUIControl = new SEditCtl();
		pUIControl->Create(g_pd3dDevice, nullptr, L"../../data/ui/main_listfrnd.png");
		pUIControl->Scale(50, 50, 0);
		}
	}
	pUIControl->Set(m_pSwapChainDesc.BufferDesc.Width, m_pSwapChainDesc.BufferDesc.Height);	
	m_pUIList.push_back(pUIControl);
	return pUIControl;
}
SControlUI* Sample::SelectRect()
{
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(m_hWnd, &mouse);
	vector<SControlUI*> SelectList;
	for (int iRect = 0; iRect < m_pUIList.size(); iRect++)
	{
		SControlUI* pRect = m_pUIList[iRect];
		if (g_Input.GetKey(VK_RBUTTON) == KEY_HOLD)
		{
			RECT rt= pRect->m_rt;
			// 뒤집어 진 경우
			if (pRect->m_rt.left > pRect->m_rt.right)
			{
				rt.left = pRect->m_rt.right;
				rt.right = pRect->m_rt.left;
			}
			if (pRect->m_rt.top > pRect->m_rt.bottom)
			{
				rt.top = pRect->m_rt.bottom;
				rt.bottom = pRect->m_rt.top;
			}
			if (rt.left <= mouse.x && rt.right >= mouse.x)
			{
				if (rt.top <= mouse.y && rt.bottom >= mouse.y)
				{
					SelectList.push_back(pRect);
				}
			}							
		}
	}
	// 가장 앞에 있는 컨트롤을 반환한다.
	SControlUI* pSelect = NULL;
	if (SelectList.size() > 0)
	{
		pSelect = SelectList[0];
		for (int iSelect = 1; iSelect < SelectList.size(); iSelect++)
		{
			if (pSelect->m_vTrans.z > SelectList[iSelect]->m_vTrans.z)
			{
				pSelect = SelectList[iSelect];
			}
		}
	}
	return pSelect;
}
bool Sample::Init()
{
	UINT id = RegisterWindowMessage(L"WM_KGCA");

	I_Ime.ImeUi_Initialize(m_hWnd);

	SImageCtl* pImageCtl = new SImageCtl();
	pImageCtl->Set(m_pSwapChainDesc.BufferDesc.Width, m_pSwapChainDesc.BufferDesc.Height);
	pImageCtl->Create(g_pd3dDevice, nullptr, L"../../data/ui/hud.dds");
	pImageCtl->Scale(400-1.0f, 300-1.0f, 1.0f-1.0f);
	pImageCtl->Move(0, 0, 100);
	m_pUIList.push_back(pImageCtl);

	SButtonCtl* pBoxCtl = new SButtonCtl();
	pBoxCtl->Set(m_pSwapChainDesc.BufferDesc.Width, m_pSwapChainDesc.BufferDesc.Height);
	pBoxCtl->Create(g_pd3dDevice, nullptr, L"../../data/ui/exit_lg.bmp");
	pBoxCtl->Scale(100 - 1.0f, 50 - 1.0f, 1 - 1.0f);
	pBoxCtl->Move(0, 0, 0);
	m_pUIList.push_back(pBoxCtl);
	
#pragma region g_pTextureSRV
	m_pTextureSRV[0].Attach(CreateShaderResourceView(g_pd3dDevice, L"../../data/ui/Background.bmp"));
	m_pTextureSRV[1].Attach(CreateShaderResourceView(g_pd3dDevice, L"../../data/ui/_connect.bmp"));
	m_pTextureSRV[2].Attach(CreateShaderResourceView(g_pd3dDevice, L"../../data/ui/_exit_lg.bmp"));
#pragma endregion
	return true;
}
bool Sample::Frame()
{
	if (g_Input.GetKey('P') == KEY_UP)
	{
		m_pSelectPlane = AddRect(T_BUTTON);
		//SendMessage(g_hWnd, WM_TEXT, 0, 0);
		PostMessage(g_hWnd, WM_TEXT, MAKELONG(1, 2), MAKELONG(3, 4));
	}
	if (g_Input.GetKey('O') == KEY_UP)
	{
		m_pSelectPlane = AddRect(T_EDIT);
	}

	SControlUI* pSelect = SelectRect();

	if (pSelect != NULL && m_pSelectPlane != pSelect)
	{
		m_pSelectPlane = pSelect;
	}

	if (m_pSelectPlane)
	{
		Vector3 vPos(0, 0, 0);
		Vector3 vScale(0, 0, 0);
		if (g_Input.GetKey('Q'))
		{
			vScale.x += 50 * g_fSecondPerFrame;
		}
		if (g_Input.GetKey('E'))
		{
			vScale.x += -50 * g_fSecondPerFrame;
		}
		if (g_Input.GetKey('Z'))
		{
			vScale.y += 50 * g_fSecondPerFrame;
		}
		if (g_Input.GetKey('C'))
		{
			vScale.y += -50 * g_fSecondPerFrame;
		}
		if (g_Input.GetKey('W'))
		{
			vPos.y = 50 * g_fSecondPerFrame;
		}
		if (g_Input.GetKey('S'))
		{
			vPos.y = -50 * g_fSecondPerFrame;
		}
		if (g_Input.GetKey('A'))
		{
			vPos.x = -50 * g_fSecondPerFrame;
		}
		if (g_Input.GetKey('D'))
		{
			vPos.x = 50 * g_fSecondPerFrame;
		}
		m_pSelectPlane->Move(vPos.x,vPos.y,	vPos.z);
		m_pSelectPlane->Scale(vScale.x,	vScale.y,vScale.z);
	}

	for (int iPlane = 0; iPlane < m_pUIList.size(); iPlane++)
	{
		SControlUI* pRect = m_pUIList[iPlane];
		pRect->Update();
	}
	return true;
}
bool Sample::Render()
{
	Matrix matTrans, matRotation, matZ;
	// 2초당 1회전( 1 초 * D3DX_PI = 3.14 )
	float t = g_Timer.m_fGameTimer * SBASIS_PI;

	//MatrixTranspose(&m_cbData.matProj, &m_Projection[1]);
	for (int iPlane = 0; iPlane < m_pUIList.size(); iPlane++)
	{
		SControlUI* pRect = m_pUIList[iPlane];
		pRect->Render(g_pImmediateContext);		
	}
	return true;
}
bool Sample::Release()
{
	for (int iPlane = 0; iPlane < m_pUIList.size(); iPlane++)
	{
		SControlUI* pRect = m_pUIList[iPlane];
		pRect->Release();
		delete pRect;
	}
	I_Ime.Uninitialize();
	return true;
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::CreateResource()
{
	HRESULT hr;
	// 투영행렬도 윈도우 사이즈가 변경되면 반드시 업테이트 해야 한다.
	int iRectWidth = m_pSwapChainDesc.BufferDesc.Width;
	int iRectHeight = m_pSwapChainDesc.BufferDesc.Height;
	for (int iPlane = 0; iPlane < m_pUIList.size(); iPlane++)
	{
		SControlUI* pRect = m_pUIList[iPlane];
		if( pRect ) pRect->CreateResource(iRectWidth, iRectHeight);
	}
	return S_OK;
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
Sample::Sample(void)
{
	m_pSelectPlane = NULL;
}

Sample::~Sample(void)
{
}
SGAME_RUN;