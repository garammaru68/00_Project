#include "SItemObject.h"
#include "SScene.h"
#include "SObjectManager.h"
void SItemObject::Initialize()
{
	m_fTmpTimer = 1.0f;
	m_pItemObject = (SEffect*)g_ObjectMgr.GetPtr(L"rtBean");
}
bool SItemObject::Frame()
{
	if (m_bDead == true) return true;
	return true;
}