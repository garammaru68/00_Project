#pragma once
#include "SScene.h"
#include "Enemy.h"

struct SGameEnum : SEnum
{
	enum SGameType1
	{
		S_AIRNPC = SEnum::S_NPC + 1,
	};
};
class GameScene : public SScene
{
public:
	SObject*  NewObj(int iType);
};

