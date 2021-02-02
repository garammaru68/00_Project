#include "SEffect.h"
void SEffect::SetSprite(std::vector<RECT>& list)
{
	m_rtList = list;
}
bool SEffect::Frame()
{
	m_fTimer += g_fSecondPerFrame;
	if (m_fTimer >= m_fStep)
	{
		m_iRectIndex++;
		if (m_rtList.size() <= m_iRectIndex)
		{
			m_iRectIndex = 0;
		}
		m_fTimer -= m_fStep;
	}
	Set(m_rtList[m_iRectIndex],
		{  (LONG)m_ptPos.x, (LONG)m_ptPos.y,
			m_rtList[m_iRectIndex].right,
			m_rtList[m_iRectIndex].bottom });
	return false;
}
SEffect::SEffect() :m_iRectIndex(0.0f)
{
	m_iObjectType = 1000;
	m_fTimer = 0.0f;
	m_fStep = 1.0f;
}
SEffect::~SEffect()
{
}