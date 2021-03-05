#pragma once
#include "SCore.h"
#include "SHeightMap.h"
#include "SQuadTree.h"
#include "Pick.h"

class Sample
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

