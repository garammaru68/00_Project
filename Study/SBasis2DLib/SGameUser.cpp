#include "SGameUser.h"
#include "SFiniteStateMachine.h"
#include "SScene.h"
#include "SObjectManager.h"
void SGameUser::FSM()
{
	m_pActionList.push_back(new SPStandState(this));
	m_pActionList.push_back(new SPAttackState(this));
	m_pAction = m_pActionList[0];

	m_fTmpTimer = 1.0f;
	m_pProjectile = (SEffect*)g_ObjectMgr.GetPtr(L"rtBean");
}
void SGameUser::SetTransition(DWORD dwEvent)
{
	DWORD dwOutput = g_Fsm.GetTransition(m_pAction->m_dwState, dwEvent);

	m_pAction = m_pActionList[dwOutput];
}
bool SGameUser::Frame()
{
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

void SGameUser::Damage()
{

}
void SGameUser::Dead()
{

}