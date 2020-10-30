#pragma once
#include "SStd.h"

enum KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD
};

class SInput : public SSingleton<SInput>
{
public:
	friend class SSingleton<SInput>
private:
	DWORD	m_dwKeyState[256];
	POINT	m_MousePos;
public:
	POINT	GetPos() { return m_MousePos; }
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	DWORD GetKey(DWORD dwKey);
private:
	SInput();d
};

