// SMapToolPane.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "SMapToolPane.h"


// SMapToolPane

IMPLEMENT_DYNAMIC(SMapToolPane, CDockablePane)

SMapToolPane::SMapToolPane()
{

}

SMapToolPane::~SMapToolPane()
{
}


BEGIN_MESSAGE_MAP(SMapToolPane, CDockablePane)
END_MESSAGE_MAP()

// SMapToolPane 메시지 처리기

int SMapToolPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndForm = SMapToolForm::CreateOne(this);
	return 0;
}


void SMapToolPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_wndForm)
	{
		m_wndForm->SetWindowPos(NULL, 0, 0, cx, cy,
			SWP_NOZORDER);
	}
}


int SMapToolPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}

