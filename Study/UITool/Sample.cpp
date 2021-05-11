#include "pch.h"
#include "Sample.h"

bool Sample::Init()
{
	return true;
}
bool Sample::Frame()
{
	if (g_Input.GetKey(VK_RBUTTON) == KEY_PUSH)
	{
		m_bSelect = true;

		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(g_hWnd, &cursor);
		Matrix matProj = m_pMainCamera->m_matProj;
		Vector2 v;
		v.x = (((2.0f * cursor.x) / g_rtClient.right) - 1) / matProj._11;
		v.y = -(((2.0f * cursor.y) / g_rtClient.bottom) - 1) / matProj._22;
	}
	return true;
}
bool Sample::Render()
{
	return true;
}
bool Sample::Release()
{
	return true;
}