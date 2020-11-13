#include "SInput.h"
SKeyMap g_KeyMap;

SInput::SInput()
{
	
}
SInput::~SInput()
{

}
DWORD SInput::GetKey(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}

bool SInput::Init()
{
	ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
	return true;
}

bool SInput::Frame()
{
	GetCursorPos(&m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);
	for (int iKey = 0; iKey < 256; iKey++)
	{
		SHORT sKey = GetAsyncKeyState(iKey);
		// 00000000 00000000
		// 1000 0000
		//     &
		// 1000 0000
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[iKey] == KEY_FREE)
			{
				m_dwKeyState[iKey] = KEY_PUSH;
			}
			else
			{
				m_dwKeyState[iKey] = KEY_HOLD;
			}
		}
		else
		{
			if (m_dwKeyState[iKey] == KEY_PUSH ||
				m_dwKeyState[iKey] == KEY_HOLD)
			{
				m_dwKeyState[iKey] = KEY_UP;
			}
			else
			{
				m_dwKeyState[iKey] = KEY_FREE;
			}
		}
	}

	g_KeyMap.m_bFront = false;
	g_KeyMap.m_bLeft = false;
	g_KeyMap.m_bRight = false;
	g_KeyMap.m_bBack = false;
	g_KeyMap.m_bJump = false;
	g_KeyMap.m_bAttack = false;
	if (m_dwKeyState[VK_UP] || m_dwKeyState['W'])
	{
		g_KeyMap.m_bFront = true;
	}
	if (m_dwKeyState[VK_LEFT] || m_dwKeyState['A'])
	{
		g_KeyMap.m_bLeft = true;
	}
	if (m_dwKeyState[VK_RIGHT] || m_dwKeyState['D'])
	{
		g_KeyMap.m_bRight = true;
	}
	if (m_dwKeyState[VK_BACK] || m_dwKeyState['S'])
	{
		g_KeyMap.m_bBack = true;
	}
	if (m_dwKeyState[VK_RETURN] || m_dwKeyState[VK_MBUTTON])
	{
		g_KeyMap.m_bAttack = true;
	}
	if (m_dwKeyState[VK_SPACE] || m_dwKeyState[VK_RBUTTON])
	{
		g_KeyMap.m_bJump = true;
	}

	SHORT sKey = GetKeyState(VK_NUMLOCK);
	g_KeyMap.m_bNumLock = false;
	g_KeyMap.m_bCapsLock = false;
	g_KeyMap.m_bHangul = false;
	if (sKey & 0x01)
	{
		g_KeyMap.m_bNumLock = true;
	}
	sKey = GetKeyState(VK_CAPITAL);
	if (sKey & 0x01)
	{
		g_KeyMap.m_bCapsLock = true;
	}
	sKey = GetKeyState(VK_HANGUL);
	if (sKey & 0x01)
	{
		g_KeyMap.m_bHangul = true;
	}
	return true;
}
bool SInput::Release()
{
	return true;
}
bool SInput::Render()
{
	//char szBuffer[256] = { 0, };
	//sprintf_s(szBuffer, "\nNum=%d,Caps=%d,Han=%d",
	//	(g_KeyMap.m_bNumLock) ? 1 : 0,
	//	(g_KeyMap.m_bCapsLock) ? 1 : 0,
	//	(g_KeyMap.m_bHangul) ? 1 : 0);
	//OutputDebugStringA(szBuffer);
	return true;
}