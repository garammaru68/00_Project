#include "Pick.h"

bool Pick::Init()
{

}
bool Pick::Frame()
{
	POINT cursor;
	GetCursorPos(&cursor);
	ScreenToClient(g_hWnd, &cursor);

	Matrix matProj = m_pMainCamera->m_matProj;
	Vector3 v;
	
}
bool Pick::Render()
{

}
bool Pick::Release()
{

}