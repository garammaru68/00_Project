#include "SObject.h"

bool SObject::Reset()
{
	return true;
}
void SObject::SetPos(SPoint p)
{
	m_ptPos = p;
	Set(p, m_rtSrc, m_fSpeed);
}
void SObject::SetPosition(SPoint p)
{
	m_ptPos = p;
	m_rtDesk.left = p.x;
	m_rtDesk.top = p.y;

	m_rtCollide = m_rtDesk;
	m_rtCollide.right += m_rtDesk.left;
	m_rtCollide.bottom += m_rtDesk.top;
}
bool SObject::Init()
{
	m_pMaskBmp	= nullptr;
	m_pColorBmp = nullptr;
	return true;
}
void SObject::Set(RECT rtSrc, RECT rtDesk, float fSpeed)
{
	m_fSpeed = fSpeed;
	if (rtSrc.left >= 0)	m_rtSrc.left = rtSrc.left;
	if (rtSrc.top >= 0)		m_rtSrc.top = rtSrc.top;
	if (rtSrc.right >= 0)	m_rtSrc.right = rtSrc.right;
	if (rtSrc.bottom >= 0)	m_rtSrc.bottom = rtSrc.bottom;
	if (rtDesk.left >= 0)	m_rtDesk.left = rtDesk.left;
	if (rtDesk.top >= 0)	m_rtDesk.top = rtDesk.top;
	if (rtDesk.right >= 0)	m_rtDesk.right = rtDesk.right;
	if (rtDesk.bottom >= 0) m_rtDesk.bottom = rtDesk.bottom;
	m_ptPos.x = m_rtDesk.left;
	m_ptPos.y = m_rtDesk.top;

	SetPosition(m_ptPos);
}
void SObject::Set(SPoint p, RECT rtSrc, float fSpeed)
{
	m_ptPos = p;
	m_fSpeed = fSpeed;
	if (rtSrc.left >= 0) m_rtSrc.left = rtSrc.left;
	if (rtSrc.top >= 0) m_rtSrc.top = rtSrc.top;
	if (rtSrc.right >= 0) m_rtSrc.right = rtSrc.right;
	if (rtSrc.bottom >= 0) m_rtSrc.bottom = rtSrc.bottom;
	m_rtDesk.left = m_ptPos.x - (m_rtSrc.right / 2);
	m_rtDesk.top = m_ptPos.y - (m_rtSrc.bottom / 2);
	m_rtDesk.right = m_rtSrc.right;
	m_rtDesk.bottom = m_rtSrc.bottom;

	m_rtCollide = m_rtDesk;
	m_rtCollide.right += m_rtDesk.left;
	m_rtCollide.bottom += m_rtDesk.top;
}
bool SObject::Load(const TCHAR* color, const TCHAR* mask, DWORD dwColor)
{
	if (color != nullptr)
	{
		m_pColorBmp = g_BitmapMgr.Load(color);
		if (m_pColorBmp != nullptr)
		{
			Set({ 0, 0,
				m_pColorBmp->m_BitmapInfo.bmWidth,
				m_pColorBmp->m_BitmapInfo.bmHeight },
				{ 0,0,
				m_pColorBmp->m_BitmapInfo.bmWidth,
				m_pColorBmp->m_BitmapInfo.bmHeight });
		}
	}
	if (mask != nullptr)
	{
		m_pMaskBmp = g_BitmapMgr.Load(mask);
	}
	if (mask == nullptr)
	{
		m_bColorKey = true;
	}
	m_dwColorKey = dwColor;
	return true;
}
bool SObject::Frame()
{
	return true;
}
bool SObject::Release()
{
	return true;
}
bool SObject::Render()
{
	if (m_bColorKey == true)
	{
		DrawColorKey();
		return true;
	}
	if (m_pColorBmp &&
		m_pColorBmp->m_BitmapInfo.bmBitsPixel == 32)
	{
		m_pColorBmp->DrawAlphaBlend(m_rtDesk, m_rtSrc);
		return true;
	}
	if (m_pMaskBmp != nullptr)
	{
		m_pMaskBmp->Draw(m_rtDesk, m_rtSrc, SRCAND, 1);
		m_pColorBmp->Draw(m_rtDesk, m_rtSrc, SRCINVERT, 1);
		m_pMaskBmp->Draw(m_rtDesk, m_rtSrc, SRCINVERT, 1);
	}
	else
	{
		m_pColorBmp->Render();
	}
	return true;
}
void SObject::DrawColorKey()
{
	m_pColorBmp->DrawColorKey(
		m_rtDesk,
		m_rtSrc,
		m_dwColorKey);
}

SObject::SObject()
{
	m_fLifeTime = -1.0f;
	m_iObjectType = -1;
	m_bDead = false;
	m_ptDirection.x = 1.0f;
	m_ptDirection.y = 1.0f;
	m_fSpeed = 200.0f;
	m_bColorKey = false;
	m_dwColorKey = RGB(255, 255, 255);
}
SObject::~SObject()
{
}