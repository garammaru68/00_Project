#pragma once
#include "SCore.h"

class Sample : public SCore
{
public:
	bool m_bSelect;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

