#include "SWrite.h"
#include "STimer.h"

void SWrite::Push(SMsg msg)
{
	if (m_TextList.size() > 10)
	{
		m_TextList.pop_back();
	}
	m_TextList.insert(
			m_TextList.begin(),
			msg);
}
bool SWrite::Init()
{
	m_hFont = CreateFont(20, 0, 0, FW_BOLD, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"����");
	SelectObject(g_hOffScreenDC, m_hFont);
	return true;
}
bool SWrite::Frame()
{
	return true;
}
bool SWrite::Render()
{
	if (g_hOffScreenDC != NULL)
	{
		//RECT rect;
		//GetClientRect(g_hWnd, &rect);
		SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(g_hOffScreenDC, RGB(0, 0, 255));
		SetBkMode(g_hOffScreenDC, TRANSPARENT);
		for (int iMsg = 0; iMsg < m_TextList.size(); iMsg++)
		{
			DrawText(g_hOffScreenDC,
				m_TextList[iMsg].msg.c_str(),
				-1,
				&m_TextList[iMsg].rt,
				DT_LEFT);
			//TextOut(g_hOffScreenDC, 0, 0,
			//	g_Timer.m_szBuffer,
			//	_tcslen(g_Timer.m_szBuffer) - 1);
		}
	}
	return true;
}
void SWrite::Draw(int x, int y, wstring msg, DWORD color)
{
	POINT p = { x, y };
	Draw(p, msg, color);
}
void SWrite::Draw(POINT pos, wstring msg, DWORD color)
{
	if (g_hOffScreenDC != NULL)
	{
		SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(g_hOffScreenDC, color);
		SetBkMode(g_hOffScreenDC, TRANSPARENT);
		TextOut(g_hOffScreenDC, pos.x, pos.y,
			msg.c_str(),
			msg.size());
	}
}
bool SWrite::Release()
{
	DeleteObject(m_hFont);
	return true;
}

SWrite::SWrite()
{

}
SWrite::~SWrite()
{

}