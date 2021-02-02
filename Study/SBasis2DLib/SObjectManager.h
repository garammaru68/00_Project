#pragma once
#include "SEffect.h"
class SObjectManager : public SSingleton<SObjectManager>
{
public:
	friend class SSingleton<SObjectManager>;
private:
	std::map<wstring, SObject*>				m_List;
	std::map<wstring, SObject*>::iterator	m_iter;
	std::wstring	m_szDefaultPath;
public:
	SObject*	Add(SObject* pAddObject);
	SObject*	Load(const TCHAR* filename, DWORD dwMaskColor = RGB(255, 0, 255));
	SObject*	GetPtr(wstring filename);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	SObjectManager();
public:
	~SObjectManager();
};
#define g_ObjectMgr SObjectManager::GetInstance()