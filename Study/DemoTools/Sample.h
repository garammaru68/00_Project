#pragma once
#include "SCore.h"
#include "SCamera.h"
#include "SMap.h"
class Sample : public SCore
{
public:
	SMap		m_CustomMap;
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