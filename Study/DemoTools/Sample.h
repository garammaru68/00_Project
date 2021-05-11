#pragma once
#include "SCore.h"
#include "SCamera.h"
#include "SMap.h"
#include "SQuadTree.h"
#include "SHeightMap.h"

class Sample : public SCore
{
public:
	SHeightMap  m_CustomMap;
	SQuadtree	m_Quadtree;
	//SMap		m_CustomMap;
	SCamera*	m_pMainCamera;
	SShapeLine	m_LineDraw;
	bool		m_bCreateMap;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool CreateMap(SMapInfo& MapDesc);
	bool DrawDebug();

	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	Sample();
	virtual ~Sample();
};