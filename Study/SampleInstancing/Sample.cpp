#include "Sample.h"
bool Sample::Init()
{
	m_Camera.CreateViewMatrix({ 0,500,-500 }, { 0, 0, 0 });
	m_pObj.Init();
	m_pObj.m_pMainCamera = m_pMainCamera;

	SMapDesc desc;
	desc.iNumCols = 257;
	desc.iNumRows = 257;
	desc.fCellDistance = 10;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/grass_2.jpg";
	desc.szVS = L"MapObject.hlsl";
	desc.szPS = L"MapObject.hlsl";

	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);
	m_Quadtree.CreateQuadtree(&m_Map);
	return true;
}
bool Sample::Frame()
{
	m_pObj.Frame();
	m_bSelect = false;
	if (g_Input.GetKey(VK_RBUTTON) == KEY_PUSH)
	{
		m_bSelect = true;

		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(g_hWnd, &cursor);
	}
}