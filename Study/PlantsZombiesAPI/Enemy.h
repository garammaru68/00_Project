#pragma once
#include "SNpcObject.h"

class Enemy : public SNpcObject
{
	virtual void Damage()				override;
	virtual void Dead()					override;
	virtual void Attack(SObject* pPlayer)override;
};

