#pragma once


// SMapToolPane
#include "SMapToolForm.h"

class SMapToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(SMapToolPane)
public:
	SMapToolForm* m_wndForm;
public:
	SMapToolPane();
	virtual ~SMapToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


