#pragma once
#include "SCore.h"
#include "SCamera.h"
#include "SShape.h"
#include "Object.h"
#include "SQuadTree.h"
#include "Pick.h"
#include "SMiniMap.h"

class Sample : public SCore
{
public:
	SHeightMap		m_Map;
	SMiniMap		m_MiniMap;
	SCamera			m_MiniMapCameara;
	SQuadTree		m_QuadTree;
	Pick			m_Pick;
	S_RAY			m_Ray;
	S_BoxObject*	m_pBoxObject;
	SShape			m_BoxShape;

	Vector3 vList[3];
public:
	bool	Init();
	bool    Frame();
	bool    Render();
	bool    Release();

	void	DrawBoxObject(Matrix* pView, Matrix* pProj);
};
SGAME_RUN;