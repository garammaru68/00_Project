#pragma once
#include "SSound.h"

class SSoundManager : public SSingleton<SSoundManager>
{
public:
	friend class SSingleton<SSoundManager>;
private:
	FMOD::System*				m_pSystem;
	std::map<string, SSound*>	m_List;
	std::map<string, SSound*>::iterator m_iter;
	std::string					m_szDefaultPath;
public:
	SSound*	Load(const char* filename);
	bool	PlayEffect(const char* filename, bool bLoop = false);
	SSound*	GetPtr(string filename);

	bool Init();
	bool Frame();
	bool Render();
	bool Release();
private:
	SSoundManager();
public:
	~SSoundManager();
};
#define g_SoundMgr SSoundManager::GetInstance()
