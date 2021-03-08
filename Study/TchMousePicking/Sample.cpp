#include "Sample.h"
bool SHeightMap::CreateHeightMap(ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext, const TCHAR* pszFileName)
{
	HRESULT hr;
	ID3D11Resource* pTexture;
	size_t maxsize = 0;

	if (FAILED(hr = CreateWICTextureFromFileEx(pDevice,
		pszFileName,
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		WIC_LOADER_DEFAULT,
		&pTexture, nullptr)))
	{
		return false;
	}
	ID3D11Texture2D *pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);

	m_fHeightList.resize(
		desc.Height*desc.Width);

	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_VERTEX	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					m_fHeightList[row * desc.Width + col] = uRed;
				}
			}
			pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
		}
	}

	m_iNumRows = desc.Height;
	m_iNumCols = desc.Width;
	pTexture2D->Release();
	pTexture->Release();
	return true;
}
float   SHeightMap::GetHeight(UINT index)
{
	//return 0;
	return m_fHeightList[index] / m_MapDesc.fScaleHeight;
}
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
	m_Quadtree.CreateQuadtree(&m_Map);
	return true;
}
bool    Sample::GetIntersection(
	Vector3 vStart,
	Vector3 vEnd,
	Vector3 vNormal,
	Vector3 v0,
	Vector3 v1,
	Vector3 v2)
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
bool    Sample::PointInPolygon(
	Vector3 vert,
	Vector3 faceNormal,
	Vector3 v0,
	Vector3 v1,
	Vector3 v2)
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
			if (GetIntersection(vRayOrigin, vEnd,
				vNormal, v0, v1, v2))
			{
				if (PointInPolygon(m_vIntersection,
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
	m_Quadtree.Render(g_pImmediateContext);

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
	m_Quadtree.Release();
	return true;
}