#include "STextureManager.h"
STexture* STextureManager::Load(
	ID3D11Device* pd3dDevice, const TCHAR* filename)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	std::wstring Dir = dir;
	std::wstring key;
	std::wstring loadfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}
	STexture* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}

	pData = new STexture;
	pData->Init();
	if (pData->Load(pd3dDevice, loadfile))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}

STexture* STextureManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool STextureManager::Init()
{
	m_pWhiteTexture = Load(g_pd3dDevice, L"../../data/white.jpg");
	return true;
}
bool STextureManager::Frame()
{
	return true;
}
bool STextureManager::Render()
{
	return true;
}
bool STextureManager::Release()
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

STextureManager::STextureManager()
{
	m_szDefaultPath = L"../../data/";
}
STextureManager::~STextureManager()
{
	Release();
}