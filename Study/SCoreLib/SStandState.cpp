#include "SStandState.h"

void SEStandState::Process(SObject* pPlayer)
{
	if (rand() % 10000 < 3)
	{
		m_pOwner->SetTransition(EVENT_TIMEMOVE);
	}

	SPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = ptDist.Length();
	if (fDistance < 500)
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}
}
SEStandState::SEStandState(SObject* owner) : SEnemyState(owner)
{
	m_dwState = STATE_STAND;
}

void SPStandState::Process(SObject* pEnemy)
{
	if (rand() % 10000 < 3)
	{
		m_pOwner->SetTransition(EVENT_TIMEMOVE);
	}

	SPoint ptDist = pEnemy->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = ptDist.Length();
	if (fDistance < 500)
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}
}
SPStandState::SPStandState(SObject* owner) : SPlayerState(owner)
{
	m_dwState = STATE_STAND;
}