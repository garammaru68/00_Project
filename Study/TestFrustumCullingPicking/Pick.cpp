#include "Pick.h"

bool Pick::GetIntersection(Vector3 vStart, Vector3 vEnd, Vector3 vNormal,
	Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 vDirection = vEnd - vStart;
	float D = vNormal.Dot(vDirection);
	float a0 = vNormal.Dot(v0 - vStart);
	float fT = a0 / D;
	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}
	m_vIntersection = vStart + vDirection * fT;
	return true;
}
bool Pick::PointInPolygon(Vector3 vert, Vector3 faceNormal,
	Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 e0, e1, iInter, vNormal;

	e0 = v2 - v1;
	e1 = v0 - v1;
	iInter = vert - v1;
	vNormal = e0.Cross(iInter);
	vNormal.Normalize();
	float fDot = faceNormal.Dot(vNormal);
	if (fDot < 0.0f) return false;
	vNormal = iInter.Cross(e1);
	vNormal.Normalize();

	fDot = faceNormal.Dot(vNormal);
	if (fDot < 0.0f) return false;

	e0 = v0 - v2;
	e1 = v1 - v2;
	iInter = vert - v2;
	vNormal = e0.Cross(iInter);
	vNormal.Normalize();
	fDot = faceNormal.Dot(vNormal);
	if (fDot < 0.0f) return false;

	vNormal = iInter.Cross(e1);
	vNormal.Normalize();
	fDot = faceNormal.Dot(vNormal);
	if (fDot < 0.0f) return false;
	return true;
};
bool Pick::Init()
{
	return true;
}
bool Pick::Frame()
{
	//POINT cursor;
	//GetCursorPos(&cursor);
	//ScreenToClient(g_hWnd, &cursor);

	//Matrix matProj = m_pMainCamera->m_matProj;
	//Vector3 v;
	return true;
}
bool Pick::Render()
{
	return true;
}
bool Pick::Release()
{
	return true;
}