#include "SGraphicAPI.h"
namespace SBASIS_CORE_LIB
{
	HDC		g_hScreenDC;
	HDC		g_hOffScreenDC;
}
bool SGraphicAPI::Init()
{
	m_hScreenDC = GetDC(g_hWnd);
	g_hScreenDC = m_hScreenDC;
	m_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);
	m_hOffScreenBitmap = CreateCompatibleBitmap(m_hScreenDC, g_rtClient.right, g_rtClient.bottom);
	SelectObject(m_hOffScreenDC, m_hOffScreenBitmap);
	g_hOffScreenDC = m_hOffScreenDC;

	COLORREF bkColor = RGB(255, 255, 255);
	m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hOffScreenDC, m_hbrRed);
	return true;
};
bool SGraphicAPI::Frame() {
	return true;
};
bool SGraphicAPI::PreRender()
{
	PatBlt(m_hOffScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom, PATCOPY);
	return true;
};
bool SGraphicAPI::Render()
{
	return true;
}
bool SGraphicAPI::PostRender()
{
	BitBlt(m_hScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom,
		   m_hOffScreenDC, 0, 0, SRCCOPY);
	return true;
}
bool SGraphicAPI::Release()
{
	DeleteObject(m_hbrRed);
	DeleteObject(m_hOffScreenBitmap);
	ReleaseDC(g_hWnd, m_hOffScreenDC);
	ReleaseDC(g_hWnd, m_hScreenDC);
	return true;
}