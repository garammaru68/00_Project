#pragma once
#include "SProjectile.h"
#include "SEnemyState.h"
#include "SPlayerState.h"

class SEAttackState :public SEnemyState
{
public:
	void Process(SObject* pPlayer);
public:
	SEAttackState(SObject* owner);
};

class SPAttackState : public SPlayerState
{
public:
	void Process(SObject* pEnemy);
public:
	SPAttackState(SObject* owner);
};

