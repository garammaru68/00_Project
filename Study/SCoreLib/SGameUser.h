#pragma once
#include "SProjectile.h"

class SGameUser : public SObject
{
public:
	bool		m_bAttack;
	float		m_fKeyTimer;
	SPoint		m_tTargetPos;
	DWORD		m_dwMove;
	DWORD		m_dwPreMove;
	SEffect*    m_pProjectile;
	std::vector<SProjectileInfo>	m_ProjectileList;
public:
	bool Reset() override;
	bool Init() override;
	bool Frame() override;
	bool Render(ID3D11DeviceContext*	pd3dContext) override;
public:
	virtual SObject* Clone() override;
	virtual bool Damage();
	virtual bool Dead();
	virtual bool Hit(SPoint p, int iIndex);
	virtual bool ProjectileTick();
	SGameUser()
	{
		m_dwSvrID = 0;
		m_iObjectType = 100;
		m_fSpeed = 300.0f;
	}
};

