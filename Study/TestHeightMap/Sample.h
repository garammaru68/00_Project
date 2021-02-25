#pragma once
#include "SCore.h"
#include "SShape.h"
#include "SObject.h"
#include "SModelViewCamera.h"
#include "SHeightMap.h"
#include "SMiniMap.h"
class SBoxUser : public SShapeBox
{
public:
	bool  PreFrame()
	{
		m_matWorld._41 = m_vPos.x;
		m_matWorld._42 = m_vPos.y;
		m_matWorld._43 = m_vPos.z;
		UpdateVector();
		return true;
	}
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
	void UpdateVector()
	{
		m_vLook.x = m_matWorld._31;
		m_vLook.y = m_matWorld._32;
		m_vLook.z = m_matWorld._33;
		m_vUp.x = m_matWorld._21;
		m_vUp.y = m_matWorld._22;
		m_vUp.z = m_matWorld._23;
		m_vRight.x = m_matWorld._11;
		m_vRight.y = m_matWorld._12;
		m_vRight.z = m_matWorld._13;
		m_vLook.Normalize();
		m_vUp.Normalize();
		m_vRight.Normalize();
	}
};
class Sample : public SCore
{
public:
	SMiniMap		m_MiniMap;
	SHeightMap		m_Map;
	Vector4			m_vDirValue;
	std::vector<SObject*> m_ObjList;
	SBoxUser		m_BoxShape;
	SShapePlane		m_PlaneShape;
	SShapeLine		m_LineShape;
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
	Matrix CreateMatrixShadow(
		Vector4* plane,
		Vector4* Light);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
SGAME_RUN;