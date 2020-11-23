#pragma once

#include "SStandState.h"
#include "SAttackState.h"

class SGameUser : public SObject
{
public:
	SEffect*						m_pProjectile;
	std::vector<SProjectileInfo>	m_ProjectileList;
	SPlayerState*					m_pAction;
	std::vector<SPlayerState*>		m_pActionList;
public:
	void FSM();
	void SetTransition(DWORD dwEvent);
public:
	virtual void Damage()	override;
	virtual void Dead()		override;
	bool Frame()	override;
	bool Render()	override;
	SGameUser()
	{
		m_iObjectType = 100;
	}
};

