#pragma once
#include "SNpcObject.h"

class Enemy : public SNpcObject
{
	std::vector<SPoint> SPZombie;

	virtual void NpcDamage()				override;
	virtual void NpcDead()					override;
	virtual void Attack(SObject* pPlayer)override;
};

