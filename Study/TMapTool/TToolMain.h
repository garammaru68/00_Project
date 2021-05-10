#pragma once
#include "SCore.h"
class TToolMain : public SCore
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	TToolMain();
	virtual ~TToolMain();
};

