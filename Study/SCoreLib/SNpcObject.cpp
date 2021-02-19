#include "SNpcObject.h"
#include "SFiniteStateMachine.h"
#include "SScene.h"
#include "SObjectManager.h"
SObject* SNpcObject::Clone()
{
	return new SNpcObject(*this);
}
void SNpcObject::FSM()
{
	m_pActionList.push_back(new SStandState(this));
	m_pActionList.push_back(new SMoveState(this));
	m_pActionList.push_back(new SAttackState(this));
	m_pAction = m_pActionList[0];

	m_fAttackTimer = 1.0f;
	m_pProjectile = (SEffect*)g_ObjectMgr.GetPtr(L"rSProjectile");

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
	return true;
}
bool SNpcObject::Render(ID3D11DeviceContext*	pd3dContext)
{
	SObject::Render(pd3dContext);
	return true;
}

bool SNpcObject::Damage()
{
	return false;
}
bool SNpcObject::Dead()
{
	return true;
}
bool SNpcObject::Release()
{
	SObject::Release();
	for (int iAction = 0; iAction < m_pActionList.size(); iAction++)
	{
		delete m_pActionList[iAction];
	}
	m_pActionList.clear();
	return true;
}
