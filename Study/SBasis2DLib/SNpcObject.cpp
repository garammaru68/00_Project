#include "SNpcObject.h"
#include "SFiniteStateMachine.h"
#include "SScene.h"
#include "SObjectManager.h"
void SNpcObject::FSM()
{
	m_pActionList.push_back(new SEStandState(this));
	m_pActionList.push_back(new SMoveState(this));
	m_pActionList.push_back(new SEAttackState(this));
	m_pAction = m_pActionList[0];

	m_fTmpTimer = 1.0f;
	m_pProjectile = (SEffect*)g_ObjectMgr.GetPtr(L"rtBean");

}
void SNpcObject::SetTransition(DWORD dwEvent)
{
	DWORD dwOutput = g_Fsm.GetTransition(m_pAction->m_dwState,
		dwEvent);
	m_pAction = m_pActionList[dwOutput];
}
bool SNpcObject::Frame()
{
	if (m_bDead == true) return true;
	m_pAction->Process(SScene::m_pGamePlayer);
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
	for (SProjectileInfo& project : m_ProjectileList)
	{
		if (SCollision::Rect2Rect(
			project.m_rtCollide,
			SScene::m_pGamePlayer->m_rtCollide))
		{
			SScene::m_pCurrentScene->AddEffect(L"rtBean", project.p);
			project.m_bDead = true;
		}
	}

	return true;
}
bool SNpcObject::Render()
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

void SNpcObject::Damage()
{

}
void SNpcObject::Dead()
{

}
