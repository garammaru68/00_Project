#pragma once
#include "SCoreStd.h"

class SWindow
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	MSG			m_msg;
	RECT		m_rtClient;
public:
	bool		SetWindow(HINSTANCE hInstance);
	bool		MsgProcess();
public:
	SWindow();
	virtual ~SWindow();
};

