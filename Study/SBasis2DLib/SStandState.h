#pragma once
#include "SEnemyState.h"
#include "SPlayerState.h"

class SEStandState : public SEnemyState
{
public:
	void Process(SObject* pPlayer);
	SEStandState(SObject* owner);
};

class SPStandState : public SPlayerState
{
public:
	void Process(SObject* pEnemy);
	SPStandState(SObject* owner);
};