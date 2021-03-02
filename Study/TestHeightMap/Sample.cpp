#include "Sample.h"
//Matrix* SD3DXMatrixShadow(Matrix *pout,
//	Vector4 *plight,
//	Vector4 *pplane)
//{
//	Vector4 Nplane;
//	FLOAT dot;
//	Nplane.Normalize();
//	//D3DXPlaneNormalize(&Nplane, pplane);
//	dot = Nplane.Dot(*plight);
//	pout->m[0][0] = dot - Nplane.x * plight->x;
//	pout->m[0][1] = -Nplane.x * plight->y;
//	pout->m[0][2] = -Nplane.x * plight->z;
//	pout->m[0][3] = -Nplane.x * plight->w;
//	pout->m[1][0] = -Nplane.y * plight->x;
//	pout->m[1][1] = dot - Nplane.y * plight->y;
//	pout->m[1][2] = -Nplane.y * plight->z;
//	pout->m[1][3] = -Nplane.y * plight->w;
//	pout->m[2][0] = -Nplane.z * plight->x;
//	pout->m[2][1] = -Nplane.z * plight->y;
//	pout->m[2][2] = dot - Nplane.z * plight->z;
//	pout->m[2][3] = -Nplane.z * plight->w;
//	pout->m[3][0] = -Nplane.w * plight->x;
//	pout->m[3][1] = -Nplane.w * plight->y;
//	pout->m[3][2] = -Nplane.w * plight->z;
//	pout->m[3][3] = dot - Nplane.w * plight->w;
//	return pout;
//}
//Matrix Sample::CreateMatrixShadow(
//	Vector4* pPlane,
//	Vector4* pLight)
//{
//	Matrix mat;
//	Vector4 plane, light;
//	pPlane->Normalize();
//	plane.x = pPlane->x * -1.0f;
//	plane.y = pPlane->y * -1.0f;
//	plane.z = pPlane->z * -1.0f;
//	plane.w = pPlane->w * -1.0f;
//	light = *pLight;// * -1.0f;
//	float D = -(plane.Dot(light));
//	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
//	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
//	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
//	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
//	return mat;
//}
LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);
	return -1;
}
bool Sample::Init()
{
	HRESULT hr;
	m_Map.CreateHeightMap(m_pd3dDevice, m_pd3dContext,
		L"../../data/map/HEIGHT_CASTLE.bmp");

	SMapDesc desc;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.iNumRows = m_Map.m_iNumRows;
	desc.fCellDistance = 1;
	desc.szTexFile = L"../../data/map/castle.jpg";
	desc.szVS = L"VS.txt";
	desc.szPS = L"PS.txt";

	m_Map.CreateMap(m_pd3dDevice, m_pd3dContext, desc);


	m_MiniMap.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg");

	m_vDirValue = { 0,0,0,0 };

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(1000, 1000, 1000);
	matRotation = Matrix::CreateRotationX(SBASIS_PI*0.5f);
	m_matPlaneWorld = matScale * matRotation;

	if (!m_BoxShape.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/1KGCABK.bmp"))
	{
		return false;
	}
	if (!m_PlaneShape.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg"))
	{
		return false;
	}

	m_ModelCamera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjMatrix(1, 1000, SBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();
	m_ModelCamera.CreateFrustum(m_pd3dDevice, m_pd3dContext);
	m_pMainCamera = &m_ModelCamera;


	m_TopCamera.CreateViewMatrix({ 0,30,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_TopCamera.CreateOrthographic(
		desc.iNumCols, desc.iNumRows, 1.0f, 1000);
	m_TopCamera.Init();
	return true;
}
bool Sample::Frame()
{
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
		m_BoxShape.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_BoxShape.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_BoxShape.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_BoxShape.RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_BoxShape.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_BoxShape.UpMovement(-1.0f);
	}
	m_BoxShape.Frame();
	m_pMainCamera->m_vCameraTarget = m_BoxShape.m_vPos;

	m_pMainCamera->FrameFrustum(m_pd3dContext);
	return true;
}
bool Sample::Render()
{
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(SDxState::m_pRS);
	m_pd3dContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	m_pd3dContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);

	// CULLING
	std::vector<DWORD> visibleIB;
	visibleIB.resize(m_Map.m_IndexList.size());
	m_Map.m_iNumFaces = 0;
	for (int iFace = 0; iFace < m_Map.m_IndexList.size() / 3; iFace++)
	{
		int a = m_Map.m_IndexList[iFace * 3 + 0];
		int b = m_Map.m_IndexList[iFace * 3 + 1];
		int c = m_Map.m_IndexList[iFace * 3 + 2];
		//visibleIB.push_back(a);
		//visibleIB.push_back(b);
		//visibleIB.push_back(c);
		//continue;
		Vector3 v[3];
		v[0] = m_Map.m_VertexList[a].p;
		v[1] = m_Map.m_VertexList[b].p;
		v[2] = m_Map.m_VertexList[c].p;
		SModelViewCamera* pCamera = (SModelViewCamera*)m_pMainCamera;
		for (int iV = 0; iV < 3; iV++)
		{
			BOOL bVisiable = pCamera->m_Frustum.ClassifyPoint(v[iV]);
			if (bVisiable)
			{
				//visibleIB.push_back(a);
				//visibleIB.push_back(b);
				//visibleIB.push_back(c);
				visibleIB[m_Map.m_iNumFaces * 3 + 0] = a;
				visibleIB[m_Map.m_iNumFaces * 3 + 1] = b;
				visibleIB[m_Map.m_iNumFaces * 3 + 2] = c;
				m_Map.m_iNumFaces++;
				break;
			}
		}
	}
	if (visibleIB.size() != 0)
	{
		m_Map.m_iNumFaces = visibleIB.size() / 3;
		m_pd3dContext->UpdateSubresource(
			m_Map.m_pIndexBuffer, 0, NULL, &visibleIB.at(0), 0, 0);
	}
	else
	{
		m_Map.m_iNumFaces = 0;
	}

	if (m_MiniMap.Begin(m_pd3dContext))
	{
		m_Map.SetMatrix(NULL,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);
		m_Map.Render(m_pd3dContext);

		Matrix matWorld;
		matWorld._41 = m_TopCamera.m_vCameraPos.x;
		matWorld._42 = m_TopCamera.m_vCameraPos.y;
		matWorld._43 = m_TopCamera.m_vCameraPos.z;

		m_BoxShape.SetMatrix(&m_BoxShape.m_matWorld,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);
		m_BoxShape.Render(m_pd3dContext);

		// ¹Ì´Ï¸Ê Á¤Áß¾Ó ¹èÄ¡
		//Vector3 vPos = m_BoxShape.m_vPos- m_BoxShape.m_vLook;
		//vPos.y = 30.0f;
		//m_TopCamera.CreateViewMatrix(
		//	vPos,
		//	m_BoxShape.m_vPos);

		/*m_pMainCamera->DrawFrustum(m_pd3dContext,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);*/

		m_MiniMap.End(m_pd3dContext);
	}

	m_Map.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Map.Render(m_pd3dContext);

	m_BoxShape.SetMatrix(&m_BoxShape.m_matWorld,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_BoxShape.Render(m_pd3dContext);
	m_MiniMap.SetMatrix(NULL,
		NULL, //&m_pMainCamera->m_matView,
		NULL); //&m_pMainCamera->m_matProj);
	m_MiniMap.Render(m_pd3dContext);

	return true;
}
bool Sample::PostRender()
{
	SCore::PostRender();
	return true;
}
bool Sample::Release()
{
	m_MiniMap.Release();
	m_Map.Release();
	m_BoxShape.Release();
	m_PlaneShape.Release();

	return true;
}