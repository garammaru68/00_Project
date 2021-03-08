#include "Sample.h"

bool Sample::Init()
{
	m_Camera.CreateViewMatrix({ 0,40,-40 }, { 0,0,0 });

	m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext,
		L"../../data/map/129.jpg");

	SMapDesc desc;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.iNumRows = m_Map.m_iNumRows;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/grass_2.jpg";
	desc.szVS = L"../../data/shader/VS.txt";
	desc.szPS = L"../../data/shader/PS.txt";

	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);
	m_QuadTree.CreateQuadtree(&m_Map);

	return true;
}
bool Sample::Frame()
{
	if (g_Input.GetKey(VK_RBUTTON) == KEY_PUSH)
	{
		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(g_hWnd, &cursor);

		Matrix matProj = m_pMainCamera->m_matProj;
		Vector3 v;
		v.x = (((2.0f*cursor.x) / g_rtClient.right) - 1) / matProj._11;
		v.y = -(((2.0f*cursor.y) / g_rtClient.bottom) - 1) / matProj._22;
		v.z = 1.0f;

		Matrix matInvView = m_pMainCamera->m_matView;
		matInvView = matInvView.Invert();

		vRayOrigin = Vector3(0, 0, 0);
		vRayDir = v;
		vRayOrigin = Vector3::Transform(vRayOrigin, matInvView);
		vRayDir = Vector3::TransformNormal(vRayDir, matInvView);
		vRayDir.Normalize();

		Vector3 v0, v1, v2;
		for (int iFace = 0;
			iFace < m_Map.m_IndexList.size() / 3;
			iFace++)
		{
			v0 = m_Map.m_VertexList[m_Map.m_IndexList[iFace * 3 + 0]].p;
			v1 = m_Map.m_VertexList[m_Map.m_IndexList[iFace * 3 + 1]].p;
			v2 = m_Map.m_VertexList[m_Map.m_IndexList[iFace * 3 + 2]].p;

			Vector3 vEnd = vRayOrigin + vRayDir * 1000.0f;
			Vector3 vNormal = (v1 - v0).Cross(v2 - v0);
			vNormal.Normalize();
			if (m_Pick.GetIntersection(vRayOrigin, vEnd,
				vNormal, v0, v1, v2))
			{
				if (m_Pick.PointInPolygon(m_vIntersection,
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

	m_Map.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_QuadTree.Render(g_pImmediateContext);

	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_LineShape.Draw(g_pImmediateContext, vRayOrigin, vRayOrigin + vRayDir * 100.0f);
	m_LineShape.Draw(g_pImmediateContext, vList[0], vList[1]);
	m_LineShape.Draw(g_pImmediateContext, vList[1], vList[2]);
	m_LineShape.Draw(g_pImmediateContext, vList[2], vList[0]);
	return true;
}

bool Sample::Release()
{
	m_Map.Release();
	m_QuadTree.Release();
	return true;
}