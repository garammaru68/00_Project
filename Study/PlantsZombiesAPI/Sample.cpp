#include "Sample.h"
#include "SObjectManager.h"
//»ç¶÷
bool Sample::Init()
{
	m_iCurrentScene = 0;
	m_SceneList.push_back(new SceneTitle);
	m_SceneList.push_back(new SceneBattle);
	m_SceneList[0]->Load(L"Title.txt");
	m_SceneList[1]->Load(L"Battle.txt");
	SScene::m_pCurrentScene = m_SceneList[0];
	SScene::m_pCurrentScene->CreateScene();
	return true;
}
bool Sample::Frame()
{
	SScene::m_pCurrentScene->Frame();
	if (SScene::m_pCurrentScene->m_bGameFinish)
	{
		m_bGameRun = false;
	}
	return true;
}
bool Sample::Release()
{
	for (SScene* scene : m_SceneList)
	{
		scene->Release();
	}
	return true;
}
bool Sample::Render()
{
	SScene::m_pCurrentScene->Render();
	if (SScene::m_pCurrentScene->m_bSceneChange == true)
	{
		SScene::m_pCurrentScene = m_SceneList[SScene::m_pCurrentScene->m_iNextScene];
		SScene::m_pCurrentScene->CreateScene();
	}
	return true;
}
GAMERUN;