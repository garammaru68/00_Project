#pragma once
#include "SEnemyState.h"

class SMoveState :
	public SEnemyState
{
public:
	void Process(SObject* pPlayer);
	SMoveState(SObject* owner);
};

