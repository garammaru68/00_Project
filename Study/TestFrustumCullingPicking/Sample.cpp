#include "Sample.h"

bool Sample::Init()
{
	m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext,
		L"../../data/map/129.jpg");
	//L"../../data/map/HEIGHT_MOUNDS.bmp");

	SMapDesc desc;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.iNumRows = m_Map.m_iNumRows;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/002.jpg";
	desc.szVS = L"../../data/shader/VS.txt";
	desc.szPS = L"../../data/shader/PS.txt";

	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);

	m_QuadTree.CreateQuadtree(&m_Map);

	m_MiniMap.Create(g_pd3dDevice, L"../../data/shader/vs.txt",
		L"../../data/shader/ps.txt",
		L"../../data/tileA.jpg");

	if (!m_BoxShape.Create(g_pd3dDevice, L"../../data/shader/VS.txt",
		L"../../data/shader/PS.txt",
		L"../../data/1KGCABK.bmp"))
	{
		return false;
	}
	SAFE_NEW_ARRAY(m_pBoxObject, S_BoxObject, 10);
	for (int iBox = 0; iBox < 10; iBox++)
	{
		m_QuadTree.AddObject(&m_pBoxObject[iBox]);
	}

	//m_Camera.CreateViewMatrix({ 0,40,-40 }, { 0,0,0 });
	//float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	//m_Camera.CreateProjMatrix(1, 1000, SBASIS_PI / 4.0f, fAspect);
	//m_Camera.Init();
	//m_Camera.m_Frustum.Create(g_pd3dDevice, g_pImmediateContext);
	//m_pMainCamera = &m_Camera;

	m_ModelCamera.CreateViewMatrix({ 0,40,-40 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjMatrix(1, 1000, SBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();
	m_ModelCamera.CreateFrustum(g_pd3dDevice, g_pImmediateContext);
	m_pMainCamera = &m_ModelCamera;

	m_MiniMapCameara.CreateViewMatrix({ 0,30,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_MiniMapCameara.CreateOrthographic(
		desc.iNumCols, desc.iNumRows, 1.0f, 1000);
	m_MiniMapCameara.Init();

	return true;
}
bool Sample::Frame()
{
	if (g_Input.GetKey(VK_RBUTTON) == KEY_PUSH)
	{
		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(g_hWnd, &cursor);

		//Matrix matProj = m_Camera.m_matProj;
		Matrix matProj = m_pMainCamera->m_matProj;
		Vector3 v;
		v.x = (((2.0f*cursor.x) / g_rtClient.right) - 1) / matProj._11;
		v.y = -(((2.0f*cursor.y) / g_rtClient.bottom) - 1) / matProj._22;
		v.z = 1.0f;

		Matrix matInvView = m_pMainCamera->m_matView;
		matInvView = matInvView.Invert();

		m_Ray.vOrigin = Vector3(0, 0, 0);
		m_Ray.vDirection = v;
		m_Ray.vOrigin = Vector3::Transform(m_Ray.vOrigin, matInvView);
		m_Ray.vDirection = Vector3::TransformNormal(m_Ray.vDirection, matInvView);
		m_Ray.vDirection.Normalize();

		Vector3 v0, v1, v2;
		for (int iFace = 0;
			iFace < m_Map.m_IndexList.size() / 3;
			iFace++)
		{
			v0 = m_Map.m_VertexList[m_Map.m_IndexList[iFace * 3 + 0]].p;
			v1 = m_Map.m_VertexList[m_Map.m_IndexList[iFace * 3 + 1]].p;
			v2 = m_Map.m_VertexList[m_Map.m_IndexList[iFace * 3 + 2]].p;

			Vector3 vEnd = m_Ray.vOrigin + m_Ray.vDirection * 1000.0f;
			Vector3 vNormal = (v1 - v0).Cross(v2 - v0);
			vNormal.Normalize();
			if (m_Pick.GetIntersection(m_Ray.vOrigin, vEnd,
				vNormal, v0, v1, v2))
			{
				if (m_Pick.PointInPolygon(m_Pick.m_vIntersection,
					vNormal, v0, v1, v2))
				{
					vList[0] = v0;
					vList[1] = v1;
					vList[2] = v2;
					return true;
				}
			}
		}
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
		m_Camera.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_Camera.RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_Camera.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_Camera.UpMovement(-1.0f);
	}
	m_ModelCamera.Frame();
	//m_Camera.Update();

	m_pMainCamera->m_vCameraTarget = m_ModelCamera.m_vCameraPos;
	m_pMainCamera->FrameFrustum(g_pImmediateContext);
	m_BoxShape.m_matRotation = m_pMainCamera->m_matWorld;

	return true;
}
bool Sample::Render()
{
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		SDxState::SetRasterizerState(g_pd3dDevice);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		SDxState::m_FillMode = D3D11_FILL_SOLID;
		SDxState::SetRasterizerState(g_pd3dDevice);
	}

	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->RSSetState(SDxState::m_pRS);
	g_pImmediateContext->PSSetSamplers(0, 1, &SDxState::m_pWrapLinear);
	g_pImmediateContext->OMSetDepthStencilState(SDxState::m_pDSS, 0);

	if (m_MiniMap.Begin(g_pImmediateContext))
	{
		m_Map.SetMatrix(NULL,
			&m_MiniMapCameara.m_matView,
			&m_MiniMapCameara.m_matProj);
		m_Map.Render(g_pImmediateContext);

		Matrix matWorld;
		matWorld._41 = m_MiniMapCameara.m_vCameraPos.x;
		matWorld._42 = m_MiniMapCameara.m_vCameraPos.y;
		matWorld._43 = m_MiniMapCameara.m_vCameraPos.z;

		m_BoxShape.SetMatrix(NULL,
			&m_MiniMapCameara.m_matView,
			&m_MiniMapCameara.m_matProj);
		m_BoxShape.Render(g_pImmediateContext);

		DrawBoxObject(&m_MiniMapCameara.m_matView,
			&m_MiniMapCameara.m_matProj);

		m_MiniMap.End(g_pImmediateContext);
	}

	m_Map.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);

	m_MiniMap.SetMatrix(NULL, NULL, NULL);
	m_MiniMap.Render(g_pImmediateContext);

	DrawQuadLine(m_QuadTree.m_pRootNode);
	DrawBoxObject(&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	//DrawBoxObject( &m_Camera.m_matView,
	//				 &m_Camera.m_matProj);
	m_QuadTree.Render(g_pImmediateContext);

	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_LineShape.Draw(g_pImmediateContext, m_Ray.vOrigin, m_Ray.vOrigin + m_Ray.vDirection * 100.0f);
	m_LineShape.Draw(g_pImmediateContext, vList[0], vList[1]);
	m_LineShape.Draw(g_pImmediateContext, vList[1], vList[2]);
	m_LineShape.Draw(g_pImmediateContext, vList[2], vList[0]);


	return true;
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
		vPoint[0] = Vector3(pNode->m_Box.vMin.x, pNode->m_Box.vMax.y, pNode->m_Box.vMax.z);
		vPoint[0].y -= 1.0f * pNode->m_dwDepth;
		vPoint[1] = Vector3(pNode->m_Box.vMax.x, pNode->m_Box.vMax.y, pNode->m_Box.vMax.z);
		vPoint[1].y -= 1.0f * pNode->m_dwDepth;
		vPoint[2] = Vector3(pNode->m_Box.vMin.x, pNode->m_Box.vMax.y, pNode->m_Box.vMin.z);
		vPoint[2].y -= 1.0f * pNode->m_dwDepth;
		vPoint[3] = Vector3(pNode->m_Box.vMax.x, pNode->m_Box.vMax.y, pNode->m_Box.vMin.z);
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
void Sample::DrawBoxObject(Matrix* pView, Matrix* pProj)
{
	for (int iBox = 0; iBox < 10; iBox++)
	{
		m_pBoxObject[iBox].m_matWorld._42 =
			m_Map.GetHeightMap(m_pBoxObject[iBox].m_matWorld._41,
				m_pBoxObject[iBox].m_matWorld._43);

		m_BoxShape.SetMatrix(&m_pBoxObject[iBox].m_matWorld,
			pView,
			pProj);

		if (m_pMainCamera->m_Frustum.CheckOBBInPlane(&m_pBoxObject[iBox].m_Box))
		{
			m_BoxShape.Render(g_pImmediateContext);
		}
	}
}
bool Sample::Release()
{
	SAFE_DELETE_ARRAY(m_pBoxObject);
	m_Map.Release();
	m_MiniMap.Release();
	m_QuadTree.Release();
	m_BoxShape.Release();

	return true;
}