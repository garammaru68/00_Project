#pragma once
#include "GameScene.h"
#include "SInput.h"
class SceneTitle : public GameScene
{
public:
	bool	Frame() override;
	bool	Reset() override;
public:
	SceneTitle();
	virtual ~SceneTitle();
};

