#include "SObject.h"
#include "SObjectManager.h"

DWORD SObject::Overlap(int iIndex)
{
	auto iter = m_OverlapState.find(iIndex);
	if (iter == m_OverlapState.end())
	{
		return OVERLAP_NONE;
	}
	return iter->second;
}
void SObject::HitOverlap(SObject * pDesk, DWORD dwState)
{
#ifdef _DEBUG
	T_STR szMsg = this->m_szName;
	szMsg += L"<->";
	if (pDesk != nullptr)
	{
		szMsg += pDesk->m_szName;
	}
	else
	{
		szMsg += L"none";
	}
	if (m_dwOverlapState == SOverlapState::OVERLAP_BEGIN)
	{
		szMsg += L" OVERLAP_BEGIN ";
	}
	/*if (m_dwOverlapState == SOverlapState::OVERLAP_STAY)
	{
		szMsg += L" OVERLAP_STAY ";
	}	*/
	if (m_dwOverlapState == SOverlapState::OVERLAP_END)
	{
		szMsg += L" OVERLAP_END ";
		OutputDebugString(szMsg.c_str());
	}
	szMsg += L"\n";
	OutputDebugString(szMsg.c_str());
#endif
}
void SObject::MouseOverlap(POINT ptMouse, DWORD dwState)
{
	if (dwState == SSelectState::S_SELECTED)
	{
		m_bSelect = true;
	}
#ifdef _DEBUG
	//T_STR szMsg = this->m_szName;
	//DWORD dwSelect = m_dwSelectState;
	///*if( dwSelect == SSelectState::T_DEFAULT)
	//{
	//	szMsg += L" Default ";
	//}
	//if (m_dwSelectState & SSelectState::T_HOVER)
	//{
	//	szMsg += L" Hover ";
	//}*/
	//if (m_dwSelectState & SSelectState::T_ACTIVE)
	//{
	//	szMsg += L" Active\n";		
	//	OutputDebugString(szMsg.c_str());
	//}
	////if (m_dwSelectState & SSelectState::T_FOCUS)
	////{
	////	szMsg += L" Focus ";
	////}	
	//if (m_dwSelectState & SSelectState::S_SELECTED)
	//{
	//	szMsg += L" Selected\n";
	//	OutputDebugString(szMsg.c_str());
	//}
	////szMsg += L"\n";
	////OutputDebugString(szMsg.c_str());
#endif
}
SObject* SObject::Clone()
{
	return new SObject(*this);
}
bool  SObject::Reset()
{
	return true;
}
void  SObject::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}
void  SObject::SetDir(float* p)
{
	m_ptDirection.x = p[0];
	m_ptDirection.y = p[1];
}
void  SObject::SetPos(float* d)
{
	m_ptPos.x = d[0];
	m_ptPos.y = d[1];
}
void  SObject::SetDir(SPoint p)
{
	m_ptDirection = p;
}
// 영역의 중앙이 원점이 되며 화면 밖을 벗어 나지 못하도록 한다.
void  SObject::SetPos(SPoint p)
{
	if (p.x < g_rtClient.left + (m_rtSrc.right / 2))
	{
		p.x = g_rtClient.left + (m_rtSrc.right / 2);
	}
	if (p.y < g_rtClient.top + (m_rtSrc.bottom / 2))
	{
		p.y = g_rtClient.top + (m_rtSrc.bottom / 2);
	}
	if (p.x > g_rtClient.right - (m_rtSrc.right / 2))
	{
		p.x = g_rtClient.right - (m_rtSrc.right / 2);
	}
	if (p.y > g_rtClient.bottom - (m_rtSrc.bottom / 2))
	{
		p.y = g_rtClient.bottom - (m_rtSrc.bottom / 2);
	}
	m_ptPos = p;
	Set(p, m_rtSrc);
}
// 상단-좌측 코너가 원점이 된다.
void  SObject::SetPosTL(SPoint p)
{
	m_ptPos = p;
	m_rtDesk.left = p.x;
	m_rtDesk.top = p.y;

	m_rtDraw = m_rtDesk;
	m_rtCollide = m_rtDesk;
	m_rtCollide.right += m_rtDesk.left;
	m_rtCollide.bottom += m_rtDesk.top;
}
bool  SObject::Init()
{
	//m_pMaskBmp		= nullptr;
	//m_pColorBmp		= nullptr;
	//g_ObjectMgr.AddCollisionExecute(this, bind(&SObject::HitOverlap, this, std::placeholders::_1, std::placeholders::_2));
	//g_ObjectMgr.AddSelectExecute(this, bind(&SObject::MouseOverlap, this, std::placeholders::_1, std::placeholders::_2));
	////g_ObjectMgr.m_fnExecute[m_iCollisionObjectID] = bind(&SObject::HitOverlap, this, std::placeholders::_1);
	return true;
}
void  SObject::Update()
{
}
// 상단-좌측 코너가 원점이 된다.
void  SObject::Set(RECT rtSrc, RECT rtDesk)
{
	if (rtSrc.left >= 0) m_rtSrc.left = rtSrc.left;
	if (rtSrc.top >= 0) m_rtSrc.top = rtSrc.top;
	if (rtSrc.right >= 0) m_rtSrc.right = rtSrc.right;
	if (rtSrc.bottom >= 0) m_rtSrc.bottom = rtSrc.bottom;
	if (rtDesk.left >= 0) m_rtDesk.left = rtDesk.left;
	if (rtDesk.top >= 0) m_rtDesk.top = rtDesk.top;
	if (rtDesk.right >= 0) m_rtDesk.right = rtDesk.right;
	if (rtDesk.bottom >= 0) m_rtDesk.bottom = rtDesk.bottom;
	m_ptPos.x = m_rtDesk.left;
	m_ptPos.y = m_rtDesk.top;

	m_rtDraw = m_rtDesk;
	m_rtCollide = m_rtDesk;
	m_rtCollide.right += m_rtDesk.left;
	m_rtCollide.bottom += m_rtDesk.top;
}
// 영역의 중앙이 원점이 된다.
void  SObject::Set(SPoint p, RECT rtSrc)
{
	m_ptPos = p;
	if (rtSrc.left >= 0) m_rtSrc.left = rtSrc.left;
	if (rtSrc.top >= 0) m_rtSrc.top = rtSrc.top;
	if (rtSrc.right >= 0) m_rtSrc.right = rtSrc.right;
	if (rtSrc.bottom >= 0) m_rtSrc.bottom = rtSrc.bottom;
	m_rtDesk.left = m_ptPos.x - (m_rtSrc.right / 2);
	m_rtDesk.top = m_ptPos.y - (m_rtSrc.bottom / 2);
	m_rtDesk.right = m_rtSrc.right;
	m_rtDesk.bottom = m_rtSrc.bottom;

	m_rtDraw = m_rtDesk;
	m_rtCollide = m_rtDesk;
	m_rtCollide.right += m_rtDesk.left;
	m_rtCollide.bottom += m_rtDesk.top;
}
bool  SObject::Load(const TCHAR* color, const TCHAR* mask, DWORD dwColor)
{
	//if (color != nullptr)
	//{
	//	m_pColorBmp = g_BitmapMgr.Load(color);
	//	if (m_pColorBmp != nullptr)
	//	{
	//		Set({ 0, 0,
	//			m_pColorBmp->m_Bit	mapInfo.bmWidth,
	//			m_pColorBmp->m_BitmapInfo.bmHeight },
	//			{ 0,0,
	//			m_pColorBmp->m_BitmapInfo.bmWidth,
	//			m_pColorBmp->m_BitmapInfo.bmHeight });
	//	}
	//}
	//if (mask != nullptr)
	//{
	//	m_pMaskBmp = g_BitmapMgr.Load(mask);
	//}
	//if (mask == nullptr)
	//{
	//	m_bColorKey = true;
	//}
	//m_dwColorKey = dwColor;
	return true;
}
bool  SObject::PreFrame()
{
	return true;
}
bool  SObject::Frame()
{
	PreFrame();
	for (int iChild = 0; iChild < m_pChildObjects.size(); iChild++)
	{
		m_pChildObjects[iChild]->Frame();
	}
	PostFrame();
	return true;
}
bool  SObject::PostFrame()
{
	return true;
}
bool  SObject::Release()
{
	for (int iChild = 0; iChild < m_pChildObjects.size(); iChild++)
	{
		m_pChildObjects[iChild]->Release();
		delete m_pChildObjects[iChild];
	}
	m_pChildObjects.clear();
	return SDxObject::Release();
}
bool SObject::PreRender(ID3D11DeviceContext*	pd3dContext)
{
	return true;
}
bool  SObject::Render(ID3D11DeviceContext*	pd3dContext)
{
	return SDxObject::Render(pd3dContext);
}
bool SObject::PostRender(ID3D11DeviceContext*	pd3dContext)
{
	for (int iChild = 0; iChild < m_pChildObjects.size(); iChild++)
	{
		m_pChildObjects[iChild]->Render(pd3dContext);
	}
	return SDxObject::PostRender(pd3dContext);
}
void  SObject::DrawColorKey()
{
	//m_pColorBmp->DrawColorKey(
	//	m_rtDesk, 
	//	m_rtSrc,
	//	m_dwColorKey);
}

SObject::SObject()
{
	m_iCollisionObjectID = -1;
	m_bCollisionEnabled = false;
	m_iCollisionType = SCollisionType::S_IGNORE;
	m_dwOverlapState = SOverlapState::OVERLAP_NONE;
	m_fAlphaBlend = 255.0f;
	m_fLifeTime = -1.0f;
	m_iObjectType = -1;
	m_bDead = false;
	m_ptDirection.x = 0.0f;
	m_ptDirection.y = 0.0f;
	m_fSpeed = 50.0f;
	m_bColorKey = false;
	m_dwColorKey = RGB(255, 255, 255);
	m_bSelect = false;
	m_bDrawDebugRect = false;
	m_iSortValue = 0;
}
SObject::~SObject()
{
}