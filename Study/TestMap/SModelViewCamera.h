#pragma once
#include "SCamera.h"
#include "SFrustum.h"
class SArcBall
{
public:
	Vector3   m_vDownPt;
	Vector3   m_vCurrentPt;
	Quaternion  m_qDown;
	Quaternion  m_qNow;
	bool		m_bDrag;
public:
	Matrix		GetRotationMatrix();
	Quaternion  QuatFromPoints(Vector3   v0, Vector3   v1);
	Vector3		ScreenToVector(float x, float y);
	void		OnBegin(int x, int y);
	void		OnMove(int x, int y);
	void		OnEnd();
public:
	SArcBall() {
		m_qNow = Quaternion::Identity;
		m_qDown = Quaternion::Identity;
		m_bDrag = false;
	}
};
class SModelViewCamera : public SCamera
{
public:
	//Matrix    m_matWorld;
	SFrustum  m_Frustum;
public:
	SArcBall	m_WorldArcBall;
	SArcBall	m_ViewArcBall;
	Matrix		m_mModelLastRot;
public:
	virtual  int WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);
public:
	virtual bool PostInit() override;
	virtual void Update(Vector4 data) override;
	virtual bool Frame() override;
public:
	SModelViewCamera();
	virtual ~SModelViewCamera();
};

