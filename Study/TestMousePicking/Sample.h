#pragma once
#include "SCore.h"
#include "SQuadTree.h"
#include "Pick.h"

class Sample : public SCore
{
public:
	SHeightMap			m_Map;
	SQuadTree			m_QuadTree;
	Pick				m_Pick;
	S_RAY				m_Ray;

	Vector3 vList[3];
public:
	bool	Init();
	bool    Frame();
	bool    Render();
	bool    Release();
};
SGAME_RUN;