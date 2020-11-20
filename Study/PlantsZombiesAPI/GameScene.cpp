#include "GameScene.h"

SObject*  GameScene::NewObj(int iType)
{
	SObject* pObj = SScene::NewObj(iType);
	if (pObj == nullptr)
	{
		if (iType == SGameEnum::S_AIRNPC)
		{
			pObj = new Enemy;
			Enemy* pNpc = dynamic_cast<Enemy*>(pObj);
			if (pNpc != nullptr)
			{
				pNpc->FSM();
			}
		}
	}
	return pObj;
}