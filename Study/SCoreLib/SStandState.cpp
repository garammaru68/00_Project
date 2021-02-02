#include "SStandState.h"

void SStandState::Process(SObject* pPlayer)
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
SStandState::SStandState(SObject* owner)
	: SEnemyState(owner)
{
	m_dwState = STATE_STAND;
}