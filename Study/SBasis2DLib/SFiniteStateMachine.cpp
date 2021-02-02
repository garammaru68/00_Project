#include "SFiniteStateMachine.h"

void SFiniteStateMachine::AddStateTrasition(
	DWORD dwState, DWORD dwEvent, DWORD dwOutputState)
{
	SFiniteState* pState = nullptr;
	auto ret = m_mapState.find(dwState);
	if (ret != m_mapState.end())
	{
		pState = ret->second;
	}
	else
	{
		pState = new SFiniteState(dwState);
		m_mapState[dwState] = pState;
	}
	pState->AddTransition(dwEvent, dwOutputState);
}
DWORD SFiniteStateMachine::GetTransition(DWORD dwState,
	DWORD dwEvent)
{
	return m_mapState[dwState]->m_mapList[dwEvent];
}
SFiniteStateMachine::~SFiniteStateMachine()
{
	for (auto list : m_mapState)
	{
		delete list.second;
	}
	m_mapState.clear();
}