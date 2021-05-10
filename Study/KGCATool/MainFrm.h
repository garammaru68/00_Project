
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "MapCtrl.h"
#include "MapSaveCtrl.h"
#include "MapControl.h"
class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:
	BOOL			m_bCreateMap;
	MapCtrl		m_mapCtrl;
	MapSaveCtrl  m_mapSaveCtrl;
	MapControl   m_mapControl;
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCreatemap();
	afx_msg void OnSavemap();
	afx_msg void OnOpenmap();
	afx_msg void OnUpdateCreatemap(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSavemap(CCmdUI *pCmdUI);
	afx_msg void OnMaptoolwork();
};


