#pragma once
#include "STexture.h"
class STextureManager : public SSingleton<STextureManager>
{
public:
	friend class SSingleton<STextureManager>;
private:
	std::map<wstring, STexture*>			m_List;
	std::map<wstring, STexture*>::iterator	m_iter;
	std::wstring	m_szDefaultPath;
public:
	STexture*	m_pWhiteTexture;
public:
	STexture*	Load(ID3D11Device* pd3dDevice, const TCHAR* filename);
	STexture*	GetPtr(wstring filename);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
private:
	STextureManager();
public:
	~STextureManager();
};
#define g_TexMgr STextureManager::GetInstance()