#include "SceneBattle.h"
#include "STimer.h"

bool SceneBattle::CreateScene()
{
	SScene::Reset();
	SScene::CreateScene();
	for (SObjAttribute& info : m_ObjAttribute)
	{
		if (!(info.iObjType >= 200 && info.iObjType < 300)) continue;
		//if (info.iObjType != 200) continue;

		for (int iObj = 0; iObj < m_iStageCounter; iObj++)
		{
			SObject* pObj = NewObj(info.iObjType);
			pObj->Init();
			if (info.bColorKey == false)
			{
				pObj->Load(info.colorbitMap.c_str(),
					info.maskbitMap.c_str());
			}
			else
			{
				pObj->Load(info.colorbitMap.c_str(),
					nullptr,
					info.dwColor);
			}

			SPoint pos;
			pos.x = (float)(rand() % 800);
			pos.y = (float)(rand() % 300);
			pObj->Set(pos, info.rtSrc);
			m_ObjList.push_back(pObj);
		}
	}
	m_iMaxCounter = m_ObjList.size();
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
	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Render();
	}
	if (m_iMaxCounter <= m_iNpcCounter)
	{
		SScene::m_iStageCounter++;
		CreateScene();

		//wstring nextstage = to_wstring(m_iStageCounter);
		//nextstage += L"Next Scene Play";
		//MessageBox(g_hWnd, L"Click", nextstage.c_str(), MB_OK);
		//g_Timer.Reset();
	}
	return true;
}
bool SceneBattle::Frame()
{
	SScene::Frame();
	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Frame();
	}
	for (SObject* pObj : m_ObjList)
	{
		if (SCollision::Rect2Rect(
			pObj->m_rtCollide,
			m_pGamePlayer->m_rtCollide))
		{
			Reset();
			SScene::m_iStageCounter = 0;
			m_iNextScene = 1;
		}
		for (SProjectileInfo& project : m_pGamePlayer->m_ProjectileList)
		{
			if (SCollision::Rect2Rect(
				project.m_rtCollide,
				pObj->m_rtCollide))
			{
				if (pObj->m_bDead == false)
				{
					m_iNpcCounter++;
					AddEffect(L"rtBean", project.p);
				}
				pObj->m_bDead = true;
			}
		}
	}
	return true;
}
SceneBattle::SceneBattle()
{
	m_bGameFinish = false;
	m_iSceneID = 1;
}