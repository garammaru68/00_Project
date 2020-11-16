#include "SceneBattle.h"
bool  SceneBattle::Reset()
{
	SScene::Reset();
	m_bSceneChange = true;
	return true;
}
bool	SceneBattle::Frame()
{
	SScene::Frame();
	for (auto obj : m_UIObjList)
	{
		if (obj->m_iObjectType != 301) continue;
		if (SCollision::RectInPt(obj->m_rtDesk,
			g_Input.GetPos()))
		{
			if (g_Input.GetKey(VK_LBUTTON) == KEY_UP)
			{
				Reset();
				m_iNextScene = 2;
			}
		}
	}
	return true;
}
SceneBattle::SceneBattle()
{
	m_iSceneID = 1;
}
SceneBattle::~SceneBattle()
{

}