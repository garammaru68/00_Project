
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "KGCATool.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_CREATEMAP, &CMainFrame::OnCreatemap)
	ON_COMMAND(ID_SAVEMAP, &CMainFrame::OnSavemap)
	ON_COMMAND(ID_OPENMAP, &CMainFrame::OnOpenmap)
	ON_UPDATE_COMMAND_UI(ID_CREATEMAP, &CMainFrame::OnUpdateCreatemap)
	ON_UPDATE_COMMAND_UI(ID_SAVEMAP, &CMainFrame::OnUpdateSavemap)
	ON_COMMAND(ID_MAPTOOLWORK, &CMainFrame::OnMaptoolwork)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	m_bCreateMap = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG
//(&C) \t Ctrl + Q

// CMainFrame 메시지 처리기



void CMainFrame::OnCreatemap()
{
	m_bCreateMap = TRUE;
	MapCtrl mapCtrl;
	mapCtrl.DoModal();
}
void CMainFrame::OnOpenmap()
{
	
	AfxMessageBox(L"지형열기 클릭");
}

void CMainFrame::OnSavemap()
{
	m_bCreateMap = FALSE;
	// modeless 
	if (m_mapSaveCtrl.GetSafeHwnd() == NULL)
	{
		m_mapSaveCtrl.Create(IDD_MAPSAVECTRL);
	}
	m_mapSaveCtrl.ShowWindow(SW_SHOW);
}


void CMainFrame::OnUpdateCreatemap(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bCreateMap);
}


void CMainFrame::OnUpdateSavemap(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bCreateMap);
}


void CMainFrame::OnMaptoolwork()
{
	// modeless 
	if (m_mapControl.GetSafeHwnd() == NULL)
	{
		m_mapControl.Create(IDD_DIALOG1);
	}
	m_mapControl.ShowWindow(SW_SHOW);
}
