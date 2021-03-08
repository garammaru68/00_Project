#pragma once
#include "SCore.h"
#include "SQuadTree.h"
#include "Pick.h"

class Sample : public SCore
{
public:
	SHeightMap  m_Map;
	SQuadTree	m_QuadTree;
	Pick		m_Pick;

	Vector3 vRayOrigin;
	Vector3 vRayDir;
	Vector3 m_vIntersection;
	Vector3 vList[3];
public:
	bool	Init();
	bool    Frame();
	bool    Render();
	bool    Release();
};
SGAME_RUN;