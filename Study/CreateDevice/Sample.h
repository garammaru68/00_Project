#pragma once
#include "SCore.h"
#include "SDevice.h"
class Sample : public SCore
{
public:
	SDevice	m_Device;
public:
	bool	Init();
	bool	Frame();
	bool	PreRender();
	bool	Render();
	bool	PostRender();
	bool	Release();
};
SGAME_RUN;