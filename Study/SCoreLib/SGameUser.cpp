#include "SGameUser.h"
#include "SInput.h"
#include "SObjectManager.h"

bool SGameUser::Reset()
{
	m_ProjectileList.clear();
	return true;
}
bool SGameUser::Init()
{
	SObject::Init();
	m_pProjectile = (SEffect*)g_ObjectMgr.GetPtr(L"rtZombies");
	return true;
}
bool SGameUser::Frame()
{
	if (g_KeyMap.m_bRight)
	{
		m_ptPos.x += m_fSpeed * g_fSecondPerFrame;
	}
	if (g_KeyMap.m_bLeft)
	{
		m_ptPos.x -= m_fSpeed * g_fSecondPerFrame;
	}
	if (g_KeyMap.m_bFront)
	{
		m_ptPos.y -= m_fSpeed * g_fSecondPerFrame;
	}
	if (g_KeyMap.m_bBack)
	{
		m_ptPos.y += m_fSpeed * g_fSecondPerFrame;
	}
	SetPos(m_ptPos);

	if (m_pProjectile &&
		g_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
	{
		SProjectileInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;
		info.iMaxCount = m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };

		m_ProjectileList.push_back(info);
	}
	for (std::vector<SProjectileInfo>::iterator iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end();
		)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_ProjectileList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool SGameUser::Render()
{
	SObject::Render();
	for (auto& pInfo : m_ProjectileList)
	{
		m_pProjectile->Set(pInfo.p,
			m_pProjectile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectile->m_rtCollide;
		m_pProjectile->Render();
	}
	return true;
}