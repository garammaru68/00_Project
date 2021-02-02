#pragma once
#include "SEnemyState.h"
class SStandState :
	public SEnemyState
{
public:
	void Process(SObject* pPlayer);
	SStandState(SObject* owner);
};

