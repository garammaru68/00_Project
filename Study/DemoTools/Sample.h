#pragma once
#include "SCore.h"
class Sample : public SCore
{
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

