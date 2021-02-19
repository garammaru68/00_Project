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
	virtual void FSM();
	virtual void SetTransition(DWORD dwEevnt);
public:
	virtual bool Damage()				override;
	virtual bool Dead()					override;
public:
	virtual SObject* Clone() override;
	virtual bool Frame() override;
	virtual bool Render(ID3D11DeviceContext*	pd3dContext) override;
	virtual bool Release() override;
	SNpcObject()
	{
		m_iObjectType = 200;
	}
};

