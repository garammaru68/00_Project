#pragma once
#include "SObject.h"
#include "SFiniteState.h"
class SPlayerState
{
public:
	SObject*	m_pOwner;
	DWORD		m_dwState;
public:
	virtual void Process(SObject* pEnemy) = 0;
	SPlayerState(SObject* owner);
};

