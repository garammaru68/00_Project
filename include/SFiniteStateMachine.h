#pragma once
#include "SFiniteState.h"

class SFiniteStateMachine : public SSingleton<SFiniteStateMachine>
{
	friend class SSingleton<SFiniteStateMachine>;
private:
	//        STAND   STAND-CLASS
	//        인덱스  상태클래스
	std::map< DWORD, SFiniteState*> m_mapState;
public:
	void    AddStateTrasition(	DWORD dwState,
								DWORD dwEvent,
								DWORD dwOutputState);
	DWORD	GetTransition(DWORD dwState, DWORD dwEvent);
	SFiniteStateMachine() = default;
	~SFiniteStateMachine();
};

#define g_Fsm SSingleton<SFiniteStateMachine>::GetInstance()