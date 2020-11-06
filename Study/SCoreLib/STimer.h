#pragma once
#include "SStd.h"

class STimer : public SSingleton<STimer>
{
public:
	float	m_fGameTimer;
	float	m_fSecondPerFrame;
	int		m_iFPS;

	WCHAR	m_szBuffer[256] = { 0, };
	float	m_fBeforeTime;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	STimer();
	virtual ~STimer();
};
#define g_Timer STimer::GetInstance()

