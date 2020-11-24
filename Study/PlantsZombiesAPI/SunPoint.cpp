#include "SunPoint.h"

void SunPoint::Initialize()
{
}
bool SunPoint::Frame()
{
	SItemObject::Frame();

	if (m_pItemObject)
	{
		SItemObjectInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;
		info.iMaxCount = m_pItemObject->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };
		info.m_vDir = SPoint(0.0f, 1.0f);
		m_ItemObjectList.push_back(info);
	}

	for (std::vector<SItemObjectInfo>::iterator iter = m_ItemObjectList.begin(); iter != m_ItemObjectList.end();)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_ItemObjectList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	for (SItemObjectInfo& sunpoint : m_ItemObjectList)
	{
		if (SCollision::RectInPt(sunpoint.m_rtCollide, g_Input.GetPos()))
		{
			if (g_Input.GetKey(VK_LBUTTON) == KEY_UP)
			{
				sunpoint.m_bDead = true;
			}
		}
	}
	return true;
}