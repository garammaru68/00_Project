#pragma once
#include "SCore.h"
#include "SShape.h"
#include "SCamera.h"
class Sample : public SCore
{
public:
	Vector4			m_vDirValue;
	SShapeBox		m_Box;
	SShapePlane		m_Plane;
	SShapeLine		m_Line;
public:
	xMatrix    m_matBoxWorld;
	xMatrix    m_matPlaneWorld;
	SCamera	   m_Camera;
public:

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	xMatrix CreateMatrixShadow(
		Vector4* plane,
		Vector4* Light);
};

SGAME_RUN;