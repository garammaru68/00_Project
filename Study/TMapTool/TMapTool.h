
// TMapTool.h : TMapTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "TToolMain.h"

// CTMapToolApp:
// �� Ŭ������ ������ ���ؼ��� TMapTool.cpp�� �����Ͻʽÿ�.
//

class CTMapToolApp : public CWinApp 
{
public:
	TToolMain   m_Tool;
	CTMapToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CTMapToolApp theApp;
