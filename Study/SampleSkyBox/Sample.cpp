#include "Sample.h"
#define NUM_OBJECTS 10
Matrix* TD3DXMatrixShadow(Matrix *pout,
	Vector4 *plight,
	Vector4 *pplane)
{
	Vector4 Nplane;
	FLOAT dot;
	Nplane.Normalize();
	//D3DXPlaneNormalize(&Nplane, pplane);
	dot = Nplane.Dot(*plight);
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
Matrix Sample::CreateMatrixShadow(
	Vector4* pPlane,
	Vector4* pLight)
{
	Matrix mat;
	Vector4 plane, light;
	pPlane->Normalize();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane.Dot(light));
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}
bool Sample::DrawQuadLine(SNode* pNode)
{
	if (pNode == NULL) return true;

	if (m_QuadTree.m_iRenderDepth >= pNode->m_dwDepth)
		//if (4 >= pNode->m_dwDepth)
	{
		m_LineShape.SetMatrix(NULL,
			&m_pMainCamera->m_matView,
			&m_pMainCamera->m_matProj);

		Vector4 vColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 0) vColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 1) vColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 2) vColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		if (pNode->m_dwDepth == 3) vColor = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		if (pNode->m_dwDepth == 4) vColor = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 5) vColor = Vector4(0.0f, 0.5f, 1.0f, 1.0f);
		if (pNode->m_dwDepth == 6) vColor = Vector4(1.0f, 0.5f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 7) vColor = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		if (pNode->m_dwDepth == 8) vColor = Vector4(1.0f, 0.5f, 0.5f, 1.0f);
		if (pNode->m_dwDepth == 9) vColor = Vector4(1.0f, 0.5f, 1.0f, 1.0f);

		Vector3 vPoint[4];
		vPoint[0] = Vector3(pNode->m_tBox.vMin.x, pNode->m_tBox.vMax.y, pNode->m_tBox.vMax.z);
		vPoint[0].y -= 1.0f * pNode->m_dwDepth;
		vPoint[1] = Vector3(pNode->m_tBox.vMax.x, pNode->m_tBox.vMax.y, pNode->m_tBox.vMax.z);
		vPoint[1].y -= 1.0f * pNode->m_dwDepth;
		vPoint[2] = Vector3(pNode->m_tBox.vMin.x, pNode->m_tBox.vMax.y, pNode->m_tBox.vMin.z);
		vPoint[2].y -= 1.0f * pNode->m_dwDepth;
		vPoint[3] = Vector3(pNode->m_tBox.vMax.x, pNode->m_tBox.vMax.y, pNode->m_tBox.vMin.z);
		vPoint[3].y -= 1.0f * pNode->m_dwDepth;

		m_LineShape.Draw(SBASIS_CORE_LIB::g_pImmediateContext, vPoint[0], vPoint[1], vColor);
		m_LineShape.Draw(SBASIS_CORE_LIB::g_pImmediateContext, vPoint[1], vPoint[3], vColor);
		m_LineShape.Draw(SBASIS_CORE_LIB::g_pImmediateContext, vPoint[2], vPoint[3], vColor);
		m_LineShape.Draw(SBASIS_CORE_LIB::g_pImmediateContext, vPoint[0], vPoint[2], vColor);
	}
	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		DrawQuadLine(pNode->m_ChildList[iNode]);
	}
	return true;
}
LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);
	return -1;
}
void Sample::DrawObject(Matrix* pView, Matrix* pProj)
{
	for (int iBox = 0; iBox < NUM_OBJECTS; iBox++)
	{
		m_pObject[iBox].m_matWorld._42 =
			m_Map.GetHeightMap(m_pObject[iBox].m_matWorld._41,
				m_pObject[iBox].m_matWorld._43);

		m_BoxShape.SetMatrix(&m_pObject[iBox].m_matWorld,
			pView,
			pProj);
		// OBB와 프로스텀 박스의 제외처리( 걸쳐 있어도 TRUE가 됨. )
		if (m_pMainCamera->m_Frustum.CheckOBBInPlane(&m_pObject[iBox].m_tBox))
		{
			m_BoxShape.Render(g_pImmediateContext);
		}
	}
}
bool Sample::Init()
{
	HRESULT hr;
	m_Skybox.Create(g_pd3dDevice, L"VS.txt", L"skyObject.txt", L"");

	m_pConstantBufferLight[0].Attach(SBASIS_CORE_LIB::CreateConstantBuffer(
		g_pd3dDevice, &m_cbLight1, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
	m_pConstantBufferLight[1].Attach(SBASIS_CORE_LIB::CreateConstantBuffer(
		g_pd3dDevice, &m_cbLight2, 1, sizeof(LIGHT_CONSTANT_BUFFER2)));
	m_pConstantBufferLight[2].Attach(SBASIS_CORE_LIB::CreateConstantBuffer(
		g_pd3dDevice, &m_cbLight3, 1, sizeof(LIGHT_CONSTANT_BUFFER3)));

	if (!m_UserShape.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/1KGCABK.bmp"))
	{
		return false;
	}

	m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext,
		L"../../data/map/HEIGHT_CASTLE.bmp");

	SMapDesc desc;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.iNumRows = m_Map.m_iNumRows;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/castle.jpg";
	desc.szVS = L"VS.txt";
	desc.szPS = L"PS.txt";

	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);

	m_QuadTree.Build(129, 129);


	m_MiniMap.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg");

	m_vDirValue = { 0,0,0,0 };

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(1000, 1000, 1000);
	matRotation = Matrix::CreateRotationX(SBASIS_PI*0.5f);
	m_matPlaneWorld = matScale * matRotation;

	if (!m_BoxShape.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/1KGCABK.bmp"))
	{
		return false;
	}
	SAFE_NEW_ARRAY(m_pObject, S_BoxObject, NUM_OBJECTS);
	for (int iBox = 0; iBox < NUM_OBJECTS; iBox++)
	{
		m_QuadTree.AddObject(&m_pObject[iBox]);
	}

	if (!m_PlaneShape.Create(g_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/tileA.jpg"))
	{
		return false;
	}

	m_ModelCamera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjMatrix(1, 1000, SBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();
	m_ModelCamera.CreateFrustum(g_pd3dDevice, g_pImmediateContext);
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
	m_Skybox.Frame();

	m_cbLight1.g_cAmbientMaterial[0] = Vector4(0.1f, 0.1f, 0.1f, 1);
	m_cbLight1.g_cDiffuseMaterial[0] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cSpecularMaterial[0] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cEmissionMaterial[0] = Vector4(0, 0, 0, 1);
	m_cbLight1.g_cAmbientLightColor[0] = Vector4(0.3f, 0.3f, 0.3f, 1);
	m_cbLight1.g_cSpecularLightColor[0] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cDiffuseLightColor[0] = Vector4(1, 0, 0, 1.0f);
	m_cbLight1.g_cAmbientLightColor[1] = Vector4(0.3f, 0.3f, 0.3f, 1);
	m_cbLight1.g_cSpecularLightColor[1] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cDiffuseLightColor[1] = Vector4(0, 1, 0, 1.0f);
	m_cbLight1.g_cAmbientLightColor[2] = Vector4(0.3f, 0.3f, 0.3f, 1);
	m_cbLight1.g_cSpecularLightColor[2] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cDiffuseLightColor[2] = Vector4(0, 0, 1, 1.0f);

	float fTime = 32.0f;// (cosf(g_fGameTimer*0.5f)*0.5f + 0.5f)*50.0f;
	m_cbLight2.g_vLightPos[0] = Vector4(0, 30, 0, 200);//범위
	m_cbLight2.g_vLightPos[1] = Vector4(50, 0, 0, 200);//범위
	m_cbLight2.g_vLightPos[2] = Vector4(0, 0, 50, 200);//범위

	m_cbLight2.g_vLightDir[0] = -m_cbLight2.g_vLightPos[0];
	m_cbLight2.g_vLightDir[0].w = 1.0f;
	m_cbLight2.g_vLightDir[0].Normalize();
	m_cbLight2.g_vLightDir[1] = -m_cbLight2.g_vLightPos[1];
	m_cbLight2.g_vLightDir[1].w = 1.0f;
	m_cbLight2.g_vLightDir[1].Normalize();
	m_cbLight2.g_vLightDir[2] = -m_cbLight2.g_vLightPos[2];
	m_cbLight2.g_vLightDir[2].w = 1.0f;
	m_cbLight2.g_vLightDir[2].Normalize();

	for (int iLight = 0; iLight < g_iNumLight; iLight++)
	{
		Matrix matInvWorld;
		m_cbLight2.g_matInvWorld[iLight] = Matrix::Identity;
		m_cbLight2.g_vEyeDir[iLight].x = m_pMainCamera->m_vLook.x;
		m_cbLight2.g_vEyeDir[iLight].y = m_pMainCamera->m_vLook.y;
		m_cbLight2.g_vEyeDir[iLight].z = m_pMainCamera->m_vLook.z;
		m_cbLight2.g_vEyeDir[iLight].w = 50.0f; // 강도
	}

	m_cbLight3.g_vSpotInfo[0].x = 20.0f;// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[0].y = 30.0f;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[0].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[0].w = 100;// 범위	
	m_cbLight3.g_vSpotInfo[1].x = 20.0f;// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[1].y = 30.0f;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[1].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[1].w = 100;// 범위	
	m_cbLight3.g_vSpotInfo[2].x = 20.0f;// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[2].y = 30.0f;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[2].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[2].w = 100;// 범위	

	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[0].Get(), 0, NULL, &m_cbLight1, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[1].Get(), 0, NULL, &m_cbLight2, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[2].Get(), 0, NULL, &m_cbLight3, 0, 0);

	ID3D11Buffer*               pBuffers[3];
	pBuffers[0] = m_pConstantBufferLight[0].Get();
	pBuffers[1] = m_pConstantBufferLight[1].Get();
	pBuffers[2] = m_pConstantBufferLight[2].Get();
	m_pImmediateContext->PSSetConstantBuffers(1, 3, pBuffers);

	//if (g_Input.GetKey('0') == KEY_PUSH)
	//{
	//	SDxState::m_FillMode = D3D11_FILL_WIREFRAME;
	//	SDxState::SetRasterizerState(g_pd3dDevice);
	//}
	//if (g_Input.GetKey('9') == KEY_PUSH)
	//{
	//	SDxState::m_FillMode = D3D11_FILL_SOLID;
	//	SDxState::SetRasterizerState(g_pd3dDevice);
	//}
	//if (g_Input.GetKey('8') == KEY_PUSH)
	//{
	//	SDxState::m_CullMode = D3D11_CULL_BACK;
	//	SDxState::SetRasterizerState(g_pd3dDevice);
	//}
	//if (g_Input.GetKey('7') == KEY_PUSH)
	//{
	//	SDxState::m_CullMode = D3D11_CULL_FRONT;
	//	SDxState::SetRasterizerState(g_pd3dDevice);
	//}

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_UserShape.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_UserShape.RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_UserShape.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_UserShape.UpMovement(-1.0f);
	}
	m_UserShape.Frame();

	m_UserShape.m_vPos.y =
		m_Map.GetHeightMap(m_UserShape.m_matWorld._41,
			m_UserShape.m_matWorld._43);


	m_pMainCamera->m_vCameraTarget = m_UserShape.m_vPos;
	m_pMainCamera->FrameFrustum(g_pImmediateContext);

	m_UserShape.m_matRotation = m_pMainCamera->m_matWorld;
	return true;
}
bool Sample::Render()
{
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Skybox.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Skybox.Render(g_pImmediateContext);
	g_pImmediateContext->RSSetState(SDxState::g_pRS[2]);
	g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::g_pSSWrapLinear);
	g_pImmediateContext->OMSetDepthStencilState(SDxState::g_pDSSDepthDisable, 0);
	if (m_MiniMap.Begin(g_pImmediateContext))
	{
		m_Map.SetMatrix(NULL,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);
		m_Map.Render(g_pImmediateContext);

		Matrix matWorld;
		matWorld._41 = m_TopCamera.m_vCameraPos.x;
		matWorld._42 = m_TopCamera.m_vCameraPos.y;
		matWorld._43 = m_TopCamera.m_vCameraPos.z;

		m_UserShape.SetMatrix(NULL,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);
		m_UserShape.Render(g_pImmediateContext);

		m_BoxShape.SetMatrix(NULL,
			&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);
		m_BoxShape.Render(g_pImmediateContext);

		DrawObject(&m_TopCamera.m_matView,
			&m_TopCamera.m_matProj);
		m_MiniMap.End(g_pImmediateContext);
	}

	m_Map.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Map.m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
	m_Map.m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
	m_Map.m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
	m_Map.Render(g_pImmediateContext);

	m_UserShape.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_UserShape.Render(g_pImmediateContext);

	m_MiniMap.SetMatrix(NULL,
		NULL, //&m_pMainCamera->m_matView,
		NULL); //&m_pMainCamera->m_matProj);
	m_MiniMap.Render(g_pImmediateContext);


	DrawQuadLine(m_QuadTree.m_pRootNode);
	DrawObject(&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	return true;
}
bool Sample::PostRender()
{
	SCore::PostRender();
	return true;
}
bool Sample::Release()
{
	m_Skybox.Release();
	SAFE_DELETE_ARRAY(m_pObject);
	m_QuadTree.Release();
	m_MiniMap.Release();
	m_Map.Release();
	m_UserShape.Release();
	m_BoxShape.Release();
	m_PlaneShape.Release();

	return true;
}