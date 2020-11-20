#include "SceneTitle.h"

bool  SceneTitle::Reset()
{
	SScene::Reset();
	m_bSceneChange = true;
	return true;
}
bool	SceneTitle::Frame()
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
				m_iNextScene = 1;
			}
		}
	}
	return true;
}

SceneTitle::SceneTitle()
{
	m_iSceneID = 0;
}
SceneTitle::~SceneTitle()
{

}