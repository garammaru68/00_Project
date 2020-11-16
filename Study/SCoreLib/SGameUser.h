#pragma once
#include "SProjectile.h"

class SGameUser : public SObject
{
public:
	SEffect*	m_pProjectile;
	std::vector<SProjectileInfo>	m_ProjectileList;
public:
	bool Reset()	override;
	bool Init()		override;
	bool Frame()	override;
	bool Render()	override;
	SGameUser()
	{
		m_iObjectType = 100;
	}
};

