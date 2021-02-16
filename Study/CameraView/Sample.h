#pragma once
#include "SCore.h"
#include "SShape.h"
#include "SDebugCamera.h"
class Sample : public SCore
{
public:
	Vector4			m_vDirValue;
	SShapeBox		m_Box;
	SShapePlane		m_Plane;
	SShapeLine		m_Line;
public:
	Matrix    m_matBoxWorld;
	Matrix    m_matPlaneWorld;
	SDebugCamera	   m_Camera;
public:

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	Matrix CreateMatrixShadow(
		Vector4* plane,
		Vector4* Light);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

TGAME_RUN;