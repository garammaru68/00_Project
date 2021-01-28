#pragma once
#include "SObject.h"
#include "SFiniteState.h"

class SEnemyState
{
public:
	SObject*    m_pOwner;
	DWORD		m_dwState;
public:
	virtual void Process(SObject* pPlayer) = 0;
	SEnemyState(SObject* owner);
};

