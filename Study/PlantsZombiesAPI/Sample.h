#pragma once
#include "SCore.h"
#include "SceneTitle.h"
#include "SceneBattle.h"

class Sample : public SCore
{

	std::vector<SScene*>	m_SceneList;
	int						m_iCurrentScene;

public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
};

