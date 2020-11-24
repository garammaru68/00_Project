#pragma once
#include "SStandState.h"
#include "SMoveState.h"
#include "SAttackState.h"

class SNpcObject : public SObject
{
public:
	SEffect*						m_pProjectile;
	std::vector<SProjectileInfo>	m_ProjectileList;
	SEnemyState*					m_pAction;
	std::vector<SEnemyState*>  m_pActionList;
public:
	void FSM();
	void SetTransition(DWORD dwEevnt);
public:
	virtual void Damage()				override;
	virtual void Dead()					override;
public:
	bool Frame() override;
	bool Render() override;
	SNpcObject()
	{
		m_iObjectType = 200;
	}
};