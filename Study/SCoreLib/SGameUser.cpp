#include "SGameUser.h"
#include "SInput.h"
#include "SObjectManager.h"
SObject* SGameUser::Clone()
{
	return new SGameUser(*this);
}
bool SGameUser::Damage()
{
	return true;
}
bool SGameUser::Dead()
{
	return true;
}
bool SGameUser::Hit(SPoint p, int iIndex)
{
	return true;
}

bool  SGameUser::Reset()
{
	m_ProjectileList.clear();
	return true;
}
bool SGameUser::Init()
{
	SObject::Init();
	m_pProjectile = (SEffect*)g_ObjectMgr.GetPtr(L"rSProjectile");
	m_bAttack = false;
	m_fKeyTimer = 0.0f;
	return true;
}
bool SGameUser::ProjectileTick()
{
	if (m_pProjectile && m_bAttack)
	{
		SProjectileInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;// rand() % 4;
		info.iMaxCount = (int)m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };

		m_ProjectileList.push_back(info);
		m_bAttack = false;
	}

	/// TODO : OtherUser projectle frame
	for (std::vector<SProjectileInfo>::iterator iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end(); )
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
bool SGameUser::Frame()
{
	m_dwMove = 0;
	SPoint dir(0, 0);
	if (g_bActive)
	{
		if (g_KeyMap.m_bRight)
		{
			dir.x = 1.0f;
			m_dwMove |= 8;
		}
		if (g_KeyMap.m_bLeft)
		{
			dir.x = -1.0f;
			m_dwMove |= 4;
		}
		if (g_KeyMap.m_bFront)
		{
			dir.y = -1.0f;
			m_dwMove |= 2;
		}
		if (g_KeyMap.m_bBack)
		{
			dir.y = 1.0f;
			m_dwMove |= 1;
		}
		m_ptDirection = dir.Normalize();
		m_ptPos.x += m_ptDirection.x * m_fSpeed * g_fSecondPerFrame;
		m_ptPos.y += m_ptDirection.y * m_fSpeed * g_fSecondPerFrame;
	}
	else
	{
		m_ptDirection = dir.Normalize();
		m_dwMove = m_dwPreMove = 0;
	}
	SetPos(m_ptPos);
	ProjectileTick();
	return true;
}

bool SGameUser::Render(ID3D11DeviceContext*	pd3dContext)
{
	SObject::Render(pd3dContext);
	for (auto& pInfo : m_ProjectileList)
	{
		m_pProjectile->Set(pInfo.p, m_pProjectile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectile->m_rtCollide;
		m_pProjectile->Render(pd3dContext);
	}
	return true;
}