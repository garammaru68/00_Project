#pragma once
#include "SCore.h"
#include "SHeightMap.h"
#include "SQuadTree.h"

class Sample : public SCore
{
public:
	SHeightMap	m_Map;
	SQuadTree	m_QuadTree;
public:
	bool Init();
	bool Render();
	bool Release();
};
SGAME_RUN;