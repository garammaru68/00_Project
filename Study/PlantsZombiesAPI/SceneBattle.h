#pragma once
#include "GameScene.h"
class SceneBattle : public GameScene
{
public:
	bool Frame()	override;
	bool Render()	override;
	bool Reset()	override;
	bool CreateScene() override;
public:
	SceneBattle();
};

