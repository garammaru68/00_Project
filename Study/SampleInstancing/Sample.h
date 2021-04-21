#pragma once
#include "SCore.h"
#include "SMap.h"
#include "SQuadtree.h"
#include "SModel.h"
#include "SHeightMap.h"

class Sample : public SCore
{
	std::vector<Matrix> m_ModelList;
public:
	SModel	m_pObj;
	SMap	m_Map;
	SQuadtree m_Quadtree;
	S_RAY	m_Ray;
	Vector3 vRayOrigin;
	Vector3 vRayDir;
	bool m_bSelect;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();

};

