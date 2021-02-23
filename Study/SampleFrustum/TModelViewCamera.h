#pragma once
#include "TCamera.h"
#include "TFrustum.h"
class TArcBall
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
	TArcBall() {
		m_qNow = Quaternion::Identity;
		m_qDown = Quaternion::Identity;
		m_bDrag = false;
	}
};
class TModelViewCamera : public TCamera
{
public:
	//Matrix    m_matWorld;
	TFrustum  m_Frustum;
public:
	TArcBall	m_WorldArcBall;
	TArcBall	m_ViewArcBall;
	Matrix		m_mModelLastRot;
public:	
	virtual  int WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);
public:
	virtual bool CreateFrustum(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	d3dContext);
	virtual bool PostInit() override;
	virtual void Update(Vector4 data) override;
	virtual bool Frame() override;
	virtual void UpdateVector()override;
	virtual bool DrawFrustum(
		ID3D11DeviceContext*	pd3dContext,
		Matrix* pmatView, Matrix* pmatProj);
public:
	TModelViewCamera();
	virtual ~TModelViewCamera();
};

