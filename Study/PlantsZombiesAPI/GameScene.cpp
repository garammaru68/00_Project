#include "GameScene.h"

SObject*  GameScene::NewObj(int iType)
{
	SObject* pObj = SScene::NewObj(iType);

	return pObj;
}