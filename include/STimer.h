#pragma once
#include "SStd.h"
class CStopwatch
{
public:
	// 타이머의 주기(1초당 진동 수)를 반환
	CStopwatch() { QueryPerformanceFrequency(&m_liPerFreq); Start(); }
	// 일정 지점 안에서 몇 번 진동 하였는지 알려주는 함수
	void Start() { QueryPerformanceCounter(&m_liPerfStart); }

	__int64 Now()
	{
		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		// 시작부터 현재까지 걸린 시간
		__int64 qwElapsedTime = (((liPerfNow.QuadPart - m_liPerfStart.QuadPart)));
		return qwElapsedTime;
	}
	void Output(const TCHAR* pData)
	{
#ifdef _DEBUG
		TCHAR strMessage[MAX_PATH] = { 0, };
		__int64 qwElapsedTime = Now();
		float Second = static_cast<float>(qwElapsedTime) / static_cast<float>(m_liPerFreq.QuadPart);
		_stprintf_s(strMessage, _T("<=====%s[%10.4f]=====>\n"), pData, Second);
		OutputDebugString(strMessage);
#endif
	}
private:
	LARGE_INTEGER m_liPerFreq;	// 초당 계수
	LARGE_INTEGER m_liPerfStart;; // 시작 계수
};

class STimer : public SSingleton<STimer>
{
public:
	float	m_fGameTimer;
	float	m_fSecondPerFrame;
	int		m_iFPS;

	WCHAR	m_szBuffer[256] = { 0, };
	float	m_fBeforeTime;
public:
	bool	Reset();
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	STimer();
	virtual ~STimer();
};
#define g_Timer STimer::GetInstance()

