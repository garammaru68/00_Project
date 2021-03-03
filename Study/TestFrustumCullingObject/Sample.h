#pragma once
#include "SCore.h"
#include "SShape.h"
#include "SObject.h"
#include "SModelViewCamera.h"
#include "SHeightMap.h"
#include "SMiniMap.h"
#include "SQuadTree.h"
class TBoxUser : public SShapeBox
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
	SQuadTree		m_QuadTree;
	S_BoxObject*	m_pObject;
	S_BOX			m_TBoxBase;

	SMiniMap		m_MiniMap;
	SHeightMap		m_Map;
	Vector4			m_vDirValue;
	std::vector<SObject*> m_ObjList;
	TBoxUser		m_UserShape;
	TBoxUser		m_BoxShape;
	SShapePlane		m_PlaneShape;
	SModelViewCamera  m_ModelCamera;
	SCamera			m_TopCamera;
public:
	Matrix			m_matBoxWorld;
	Matrix			m_matPlaneWorld;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool PostRender() override;
	bool Release() override;
	bool DrawQuadLine(SNode* pNode);
	Matrix CreateMatrixShadow(
		Vector4* plane,
		Vector4* Light);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void DrawObject(Matrix* pView, Matrix* pProj);
};

TGAME_RUN;