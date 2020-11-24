#pragma once
#include "SEffect.h"
typedef std::vector<RECT> RECT_ARRAY;
struct SItemObjectInfo
{
	wstring		m_csName;
	SPoint		p;
	SPoint		m_vDir;
	int			id;
	int			iMaxCount;
	bool		m_bDead;
	float		m_fLifeTime;
	float		m_fTimer;
	float		m_fStep;
	int			m_iRectIndex;
	RECT		m_rtSrc;
	RECT		m_rtDesk;
	RECT		m_rtCollide;
	void Frame()
	{
		m_fLifeTime -= g_fSecondPerFrame;
		if (m_fLifeTime <= 0.0f)
		{
			m_bDead = true;
			return;
		}
		m_fTimer += g_fSecondPerFrame;
		if (m_fLifeTime >= m_fStep)
		{
			m_iRectIndex++;
			if (iMaxCount <= m_iRectIndex)
			{
				m_iRectIndex = 0.0f;
			}
			m_fTimer -= m_fStep;
		}
		p = p + (m_vDir * g_fSecondPerFrame * 300.0f);
	}
	SItemObjectInfo()
	{
		m_vDir = SPoint(0.0f, -1.0f);
		m_bDead = false;
		m_fLifeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 1.0f;
		m_iRectIndex = 0;
	}
};
class SItemObject : public SObject
{
public:
	SEffect*					m_pItemObject;
	std::vector<SItemObjectInfo>	m_ItemObjectList;
public:
	void Initialize();
	bool Frame() override;
};