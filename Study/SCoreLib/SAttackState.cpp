#include "SAttackState.h"
#include "SNpcObject.h"
#include "SGameUser.h"

void SEAttackState::Process(SObject* pPlayer)
{
	SPoint vDir = pPlayer->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = vDir.Length();
	if (fDistance > 500)
	{
		m_pOwner->SetTransition(EVENT_LOSTTARGET);
	}
	m_pOwner->m_ptPos = m_pOwner->m_ptPos + vDir.Normalize() * g_fSecondPerFrame* 100.0f;
	m_pOwner->SetPos(m_pOwner->m_ptPos);

	m_pOwner->m_fTmpTimer += g_fSecondPerFrame;
	if (m_pOwner->m_fTmpTimer > 0.33f)
	{
		m_pOwner->Attack(pPlayer);
		m_pOwner->m_fTmpTimer -= 0.33f;
	}
}
SEAttackState::SEAttackState(SObject* owner) : SEnemyState(owner)
{
	m_dwState = STATE_ATTACK;
}

void SPAttackState::Process(SObject* pPlayer)
{
	SPoint vDir = pPlayer->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = vDir.Length();
	if (fDistance > 500)
	{
		m_pOwner->SetTransition(EVENT_LOSTTARGET);
	}
	m_pOwner->m_ptPos = m_pOwner->m_ptPos + vDir.Normalize() * g_fSecondPerFrame* 100.0f;
	m_pOwner->SetPos(m_pOwner->m_ptPos);

	m_pOwner->m_fTmpTimer += g_fSecondPerFrame;
	if (m_pOwner->m_fTmpTimer > 0.33f)
	{
		m_pOwner->Attack(pPlayer);
		m_pOwner->m_fTmpTimer -= 0.33f;
	}
}

SPAttackState::SPAttackState(SObject* owner) : SPlayerState(owner)
{
	m_dwState = STATE_ATTACK;
}