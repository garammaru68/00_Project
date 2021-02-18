#pragma once
#include "SCore.h"
#include "SShape.h"
#include "SObject.h"
#include "SModelViewCamera.h"

class Sample : public SCore
{
public:
	Vector4			m_vDirValue;
	std::vector<SObject*> m_ObjList;
	SShapeBox		m_BoxShape;
	SShapePlane		m_PlaneShape;
	SShapeLine		m_LineShape;
	SModelViewCamera  m_ModelCamera;
public:
	Matrix			m_matBoxWorld;
	Matrix			m_matPlaneWorld;
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

SGAME_RUN;