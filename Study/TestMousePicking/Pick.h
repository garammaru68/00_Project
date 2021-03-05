#pragma once
#include "SShape.h"
#include "SCamera.h"

class Pick
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

