#pragma once
#include "SCore.h"
#include "SObject.h"
#include "SHeightMap.h"
#include "SModelViewCamera.h"

class Sample : public SCore
{
public:
	SHeightMap			m_Map;
	SModelViewCamera	m_MainCamera;
public:
	Matrix				m_matPlaneWorld;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	

};

SGAME_RUN;