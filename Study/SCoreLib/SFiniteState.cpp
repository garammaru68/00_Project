#include "SFiniteState.h"

void SFiniteState::AddTransition(DWORD dwEvent, DWORD dwOutputState)
{
	m_mapList[dwEvent] = dwOutputState;
}
