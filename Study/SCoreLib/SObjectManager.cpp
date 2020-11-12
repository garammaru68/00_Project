#include "SObjectManager.h"
SObject* SObjectManager::Add(SObject* pAddObject)
{
	if (pAddObject == nullptr) return nullptr;
	SObject* pData = GetPtr(pAddObject->m_szName);
	if (pData != nullptr)
	{
		return pData;
	}
	m_List.insert(make_pair(pAddObject->m_szName, pAddObject));
	return nullptr;
}
SObject* SObjectManager::Load(const TCHAR* filename, DWORD dwMaskColor)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);
	
	std::wstring Dir = dir;
	std::wstring key;
	std::wstring loadfile;
	std::wstring maskfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += filename;

		maskfile = m_szDefaultPath;
		maskfile += name;
		maskfile += L"mask";
		maskfile += +ext;
		key = filename;
	}
	else
	{
		loadfile = filename;
		maskfile = drive;
		maskfile += dir;
		maskfile += name;
		maskfile += L"mask";
		maskfile += ext;
		key = name;
		key += ext;
	}
	SObject* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}
	
	pData = new SObject;
	pData->Init();
	pData->m_szName = key;
	if (pData->Load(loadfile.c_str(), maskfile.c_str(), dwMaskColor))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}

SObject* SObjectManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool SObjectManager::Init()
{
	return true;
}
bool SObjectManager::Frame()
{
	return true;
}
bool SObjectManager::Render()
{
	return true;
}
bool SObjectManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}

SObjectManager::SObjectManager()
{
	m_szDefaultPath = L"../../data/bitmap/";
}
SObjectManager::~SObjectManager()
{
	Release();
}