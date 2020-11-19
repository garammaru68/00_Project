#include "SceneBattle.h"
#include "STimer.h"

bool SceneBattle::CreateScene()
{
	SScene::Reset();
	SScene::CreateScene();

	return true;
}
bool SceneBattle::Reset()
{
	SScene::Reset();
	m_bSceneChange = true;
	m_iNextScene = 1;
	return true;
}
bool SceneBattle::Render()
{
	SScene::Render();

	return true;
}
bool SceneBattle::Frame()
{
	SScene::Frame();

	return true;
}
SceneBattle::SceneBattle()
{
	m_bGameFinish = false;
	m_iSceneID = 1;
}
SceneBattle::~SceneBattle()
{

}