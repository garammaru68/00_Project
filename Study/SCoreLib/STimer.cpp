#include "STimer.h"
namespace SBASIS_CORE_LIB
{
	float	g_fGameTimer = 0.0f;
	float	g_fSecondPerFrame = 0.0f;
}

STimer::STimer()
{
}
STimer::~STimer()
{
}

bool STimer::Init()
{
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	m_fBeforeTime = timeGetTime();
	return true;
}
bool STimer::Reset()
{
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	m_fBeforeTime = timeGetTime();
	return true;
}
bool STimer::Frame()
{
	float fCurrentTime = timeGetTime();
	m_fSecondPerFrame = (fCurrentTime - m_fBeforeTime) / 1000.0f;
	m_fGameTimer += m_fSecondPerFrame;
	m_fBeforeTime = fCurrentTime;

	g_fSecondPerFrame = m_fSecondPerFrame;
	g_fGameTimer = m_fGameTimer;
	return true;
}
bool STimer::Render()
{
	static float fTimer = 0.0f;
	fTimer += m_fSecondPerFrame;
	if (fTimer >= 1.0f)
	{
		ZeroMemory(m_szBuffer, sizeof(WCHAR) * 256);
		_stprintf_s(m_szBuffer,
					L"게임시간=%10.2f, spf(%10.4f) fps(%d)\n",
					m_fGameTimer,
					m_fSecondPerFrame,
					m_iFPS);
		OutputDebugString(m_szBuffer);
		fTimer -= 1.0f;
		m_iFPS = 0;
	}

	m_iFPS++;
	return true;
}
bool STimer::Release()
{
	return true;
}