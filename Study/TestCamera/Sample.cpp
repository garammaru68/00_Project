#include "Sample.h"
xMatrix* SD3DXMatrixShadow(xMatrix *pout,
	Vector4 *plight,
	Vector4 *pplane)
{
	Vector4 Nplane;
	FLOAT dot;
	Nplane = Nplane.Normal();
	//D3DXPlaneNormalize(&Nplane, pplane);
	dot = Nplane | *plight;
	pout->m[0][0] = dot - Nplane.x * plight->x;
	pout->m[0][1] = -Nplane.x * plight->y;
	pout->m[0][2] = -Nplane.x * plight->z;
	pout->m[0][3] = -Nplane.x * plight->w;
	pout->m[1][0] = -Nplane.y * plight->x;
	pout->m[1][1] = dot - Nplane.y * plight->y;
	pout->m[1][2] = -Nplane.y * plight->z;
	pout->m[1][3] = -Nplane.y * plight->w;
	pout->m[2][0] = -Nplane.z * plight->x;
	pout->m[2][1] = -Nplane.z * plight->y;
	pout->m[2][2] = dot - Nplane.z * plight->z;
	pout->m[2][3] = -Nplane.z * plight->w;
	pout->m[3][0] = -Nplane.w * plight->x;
	pout->m[3][1] = -Nplane.w * plight->y;
	pout->m[3][2] = -Nplane.w * plight->z;
	pout->m[3][3] = dot - Nplane.w * plight->w;
	return pout;
}
xMatrix Sample::CreateMatrixShadow(
	Vector4* pPlane,
	Vector4* pLight)
{
	xMatrix mat;
	Vector4 plane, light;
	pPlane->Normal();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane | light);
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}
bool Sample::Init()
{
	HRESULT hr;
	m_vDirValue = { 0,0,0,0 };
	m_Camera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjMatrix(1, 1000, SBASIS_PI / 4.0f, fAspect);

	xMatrix matScale, matRotation;
	matScale.Scale(100, 100, 100);
	matRotation.XRotate(SBASIS_PI*0.5f);
	m_matPlaneWorld = matScale * matRotation;

	if (!m_Box.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg"))
	{
		return false;
	}
	if (!m_Plane.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg"))
	{
		return false;
	}
	if (!m_Line.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg"))
	{
		return false;
	}
	return true;
}
bool Sample::Frame()
{
	xMatrix matScale;
	xMatrix matRotation;
	matScale.Scale(1, 1, 1);
	matRotation.YRotate(g_fGameTimer);
	m_matBoxWorld = matScale * matRotation;
	m_matBoxWorld._42 = 3.0f;


	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		SDxState::SetRasterizerState(m_pd3dDevice);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_SOLID;
		SDxState::SetRasterizerState(m_pd3dDevice);
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		SDxState::m_CullMode = D3D11_CULL_BACK;
		SDxState::SetRasterizerState(m_pd3dDevice);
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		SDxState::m_CullMode = D3D11_CULL_FRONT;
		SDxState::SetRasterizerState(m_pd3dDevice);
	}

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_Camera.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_Camera.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_Camera.RightMovement(1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_Camera.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_Camera.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_Camera.UpMovement(-1.0f);
	}
	if (g_Input.GetKey(VK_LEFT) == KEY_HOLD)
	{
		m_vDirValue.y += g_fSecondPerFrame;
		m_Camera.Update(m_vDirValue);
	}
	//m_Camera.SetPos(m_Camera.m_vCameraPos);	
	m_Camera.Frame();
	return true;
}
bool Sample::Render()
{
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(SDxState::m_pRS);
	m_pd3dContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	m_pd3dContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);
	m_Box.SetMatrix(&m_matBoxWorld,
		&m_Camera.m_matView,
		&m_Camera.m_matProj);
	m_Box.Render(m_pd3dContext);

	xMatrix matShadow;
	Vector4 PLANE = Vector4(0, 1, 0, -0.1f);
	Vector4 LIGHT = Vector4(-10, 10, 0, 1);
	matShadow = CreateMatrixShadow(&PLANE, &LIGHT);
	matShadow = m_matBoxWorld * matShadow;
	m_Box.SetMatrix(&matShadow, &m_Camera.m_matView,
		&m_Camera.m_matProj);
	m_Box.Render(m_pd3dContext);

	m_Plane.SetMatrix(&m_matPlaneWorld,
		&m_Camera.m_matView,
		&m_Camera.m_matProj);
	m_Plane.Render(m_pd3dContext);

	m_Line.SetMatrix(NULL, &m_Camera.m_matView,
		&m_Camera.m_matProj);
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(100, 0, 0), Vector4(1, 0, 0, 1));
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 100, 0), Vector4(0, 1, 0, 1));
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 0, 100), Vector4(0, 0, 1, 1));
	return true;
}
bool Sample::Release()
{
	m_Box.Relase();
	m_Plane.Relase();
	m_Line.Relase();
	return true;
}