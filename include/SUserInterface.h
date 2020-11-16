#pragma once
#include "SObject.h"
#include "SInput.h"
class SUserInterface : public SObject
{
public:
	int		m_iState;
	SUserInterface() : m_iState(0)
	{
		m_iObjectType = 300;
	}
};

class SButton : public SUserInterface
{
public:
	bool Frame();
	bool Render();
public:
	SButton()
	{
		m_iObjectType = 301;
	}
};