#pragma once
#include "GameScene.h"
class SceneBattle : public GameScene
{
public:
	bool	Frame() override;
	bool	Reset() override;
public:
	SceneBattle();
	virtual ~SceneBattle();
};

