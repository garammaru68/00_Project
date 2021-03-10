#pragma once
#include "SCore.h"
#include "SCamera.h"
#include "SModelViewCamera.h"
#include "SShape.h"
#include "SQuadTree.h"
#include "Pick.h"
#include "SMiniMap.h"

class SBoxObject : public SShapeBox
{
public:
	void FrontMovement(float fDir)
	{
		Vector3 vOffset = m_vLook * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vOffset;
	}
	void RightMovement(float fDir)
	{
		Vector3 vMove = m_vRight * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
	}
	void UpMovement(float fDir)
	{
		Vector3 vMove = m_vUp * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
	}
	void FrontBase(float fDir)
	{
		Vector3 vSide = { 0,0,1 };
		Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
		m_vTarget += m_vLook * m_fSpeed;
	}
	void RightBase(float fDir)
	{
		Vector3 vSide = { 1,0,0 };
		Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
		m_vTarget += m_vLook * m_fSpeed;
	}
	void UpBase(float fDir)
	{
		Vector3 vUp = { 0,1,0 };
		Vector3 vMove = vUp * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
		m_vTarget += m_vLook * m_fSpeed;
	}
};

class Sample : public SCore
{
public:
	SHeightMap			m_Map;
	SMiniMap			m_MiniMap;
	SModelViewCamera	m_ModelCamera;
	SCamera				m_MiniMapCameara;
	SQuadTree			m_QuadTree;
	Pick				m_Pick;
	S_RAY				m_Ray;
	S_BoxObject*		m_pBoxObject;
	SBoxObject			m_BoxShape;

	Vector3 vList[3];
public:
	bool	Init();
	bool    Frame();
	bool    Render();
	bool    Release();

	//LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool	DrawQuadLine(SNode* pNode);
	void	DrawBoxObject(Matrix* pView, Matrix* pProj);
};
SGAME_RUN;