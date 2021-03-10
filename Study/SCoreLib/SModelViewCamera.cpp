#include "SModelViewCamera.h"

Quaternion SArcBall::QuatFromPoints(Vector3 v0, Vector3 v1)
{
	Vector3 vCross;
	float fDot = v0.Dot(v1);
	vCross = v0.Cross(v1);
	return Quaternion(vCross.x, vCross.y, vCross.z, fDot);;
}
Vector3 SArcBall::ScreenToVector(float px, float py)
{
	float x = -((px / (float)g_rtClient.right) * 2.0f - 1.0f);
	float y = (py / (float)g_rtClient.bottom) * 2.0f - 1.0f;
	float z = 0.0f;
	float fDist = x * x + y * y;
	if (fDist > 1.0f)
	{
		float fScale = (1.0f / sqrtf(fDist));
		x *= fScale;
		y *= fScale;
	}
	else
	{
		z = sqrtf(1.0f - fDist);
	}
	return Vector3(x, y, z);
}
void SArcBall::OnBegin(int x, int y)
{
	m_bDrag = true;
	m_vDownPt = ScreenToVector(x, y);
	m_qDown = m_qNow;
}
void SArcBall::OnMove(int x, int y)
{
	if (m_bDrag)
	{
		m_vCurrentPt = ScreenToVector(x, y);
		m_qNow = m_qDown * QuatFromPoints(m_vDownPt, m_vCurrentPt);
	}
}
void SArcBall::OnEnd()
{
	m_bDrag = false;
}
Matrix SArcBall::GetRotationMatrix()
{
	return Matrix::CreateFromQuaternion(m_qNow);
}
bool SModelViewCamera::PostInit()
{
	Matrix matInvView = m_matView.Invert();
	m_ViewArcBall.m_qNow = Quaternion::CreateFromRotationMatrix(matInvView);
	m_ViewArcBall.m_qNow.Normalize();

	return true;
}
int SModelViewCamera::WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	int iMouseX = LOWORD(lParam);
	int iMouseY = HIWORD(lParam);
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		m_WorldArcBall.OnBegin(iMouseX, iMouseY);
	}break;
	case WM_MOUSEMOVE:
	{
		m_WorldArcBall.OnMove(iMouseX, iMouseY);
		m_ViewArcBall.OnMove(iMouseX, iMouseY);
	}break;
	case WM_LBUTTONUP:
	{
		m_WorldArcBall.OnEnd();
	}break;
	case WM_RBUTTONDOWN:
	{
		m_ViewArcBall.OnBegin(iMouseX, iMouseY);
	}break;
	case WM_RBUTTONUP:
	{
		m_ViewArcBall.OnEnd();
	}break;
	case WM_MOUSEWHEEL:
	{
		m_fWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
	}break;
	}
	return -1;
}
bool SModelViewCamera::CreateFrustum(
	ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*	d3dContext)
{
	m_Frustum.Create(pd3dDevice, d3dContext);
	return true;
}
bool  SModelViewCamera::FrameFrustum(
	ID3D11DeviceContext*	pd3dContext)
{
	Matrix matInvViewProj = m_matView * m_matProj;
	matInvViewProj = matInvViewProj.Invert();
	for (int iVertex = 0; iVertex < 24; iVertex++)
	{
		Vector3& v = m_Frustum.m_VertexList[iVertex].p;
		m_Frustum.m_FrustumObj.m_VertexList[iVertex].p = Vector3::Transform(v, matInvViewProj);// *matInvViewProj;
	}
	pd3dContext->UpdateSubresource(
		m_Frustum.m_FrustumObj.m_pVertexBuffer, 0, NULL,
		&m_Frustum.m_FrustumObj.m_VertexList.at(0), 0, 0);

	m_Frustum.Frame();
	return true;
}
bool  SModelViewCamera::DrawFrustum(
	ID3D11DeviceContext*	pd3dContext,
	Matrix* pmatView, Matrix* pmatProj)
{
	m_Frustum.m_FrustumObj.SetMatrix(NULL,
		pmatView,
		pmatProj);
	m_Frustum.m_FrustumObj.Render(pd3dContext);
	return true;
}
void SModelViewCamera::Update(Vector4 data)
{

}
bool SModelViewCamera::Frame()
{
	//VIEW
	m_fDistance += m_fWheelDelta;
	Matrix matCameraRotation;
	matCameraRotation = m_ViewArcBall.GetRotationMatrix();
	Vector3 vLocalUp = { 0,1,0 };
	Vector3 vLocalLook = { 0,0,1 };
	// TODO V*M
	vLocalUp = Vector3::Transform(vLocalUp, matCameraRotation);
	vLocalLook = Vector3::Transform(vLocalLook, matCameraRotation);
	vLocalLook.Normalize();
	vLocalUp.Normalize();

	m_vCameraPos = m_vCameraTarget - vLocalLook * m_fDistance;
	CreateViewMatrix(m_vCameraPos, m_vCameraTarget);

	//WORLD
	Matrix mInvView = m_matView.Invert();
	mInvView._41 = 0.0f;
	mInvView._42 = 0.0f;
	mInvView._43 = 0.0f;

	Matrix mModelRotInv = m_mModelLastRot.Invert();

	Matrix mModelRot = m_WorldArcBall.GetRotationMatrix();
	m_matWorld = m_matWorld * m_matView * mModelRotInv * mModelRot * mInvView;

	m_matWorld._41 = 0;
	m_matWorld._42 = 0;
	m_matWorld._43 = 0;
	m_mModelLastRot = mModelRot;

	UpdateVector();



	m_fWheelDelta = 0;
	return true;
}
void SModelViewCamera::UpdateVector()
{
	SCamera::UpdateVector();

}
SModelViewCamera::SModelViewCamera()
{

}
SModelViewCamera::~SModelViewCamera()
{
	m_Frustum.m_FrustumObj.Release();
}