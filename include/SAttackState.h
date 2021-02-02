#pragma once
#include "SProjectile.h"
#include "SEnemyState.h"
class SAttackState :public SEnemyState
{
public:
	void Process(SObject* pPlayer);
public:
	SAttackState(SObject* owner);
};

