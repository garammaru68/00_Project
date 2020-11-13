#pragma once
#include "SObject.h"
class SEffect : public SObject
{
public:
	std::vector<RECT> m_rtList;
	int		m_iRectIndex;
	float	m_fTimer;
	float	m_fStep;
public:
	void	SetSprite(std::vector<RECT>& list);
	bool	Frame();
	SEffect();
	virtual ~SEffect();
};

