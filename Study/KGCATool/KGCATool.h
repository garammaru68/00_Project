
// KGCATool.h : KGCATool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "SCore.h"

// CKGCAToolApp:
// �� Ŭ������ ������ ���ؼ��� KGCATool.cpp�� �����Ͻʽÿ�.
//

class CKGCAToolApp : 
	public CWinApp, SCore
{
public:
	CKGCAToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKGCAToolApp theApp;
