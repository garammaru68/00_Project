#include "Sample.h"
bool Sample::GetIntersection(SNode* pNode)
{
	// face list
	Vector3 v0, v1, v2;
	for (int iFace = 0;
		iFace < pNode->m_IndexList.size() / 3;
		iFace++)
	{
		v0 = m_Map.m_VertexList[pNode->m_IndexList[iFace * 3 + 0]].p;
		v1 = m_Map.m_VertexList[pNode->m_IndexList[iFace * 3 + 1]].p;
		v2 = m_Map.m_VertexList[pNode->m_IndexList[iFace * 3 + 2]].p;

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
	return false;
}
bool Sample::IntersectBox(S_BOX* pBox, S_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	Vector3 tmin;
	tmin.x = (pBox->vMin.x - pRay->vOrigin.x) / pRay->vDirection.x + 0.001f;
	tmin.y = (pBox->vMin.y - pRay->vOrigin.y) / pRay->vDirection.y + 0.001f;
	tmin.z = (pBox->vMin.z - pRay->vOrigin.z) / pRay->vDirection.z + 0.001f;

	Vector3 tmax;
	tmax.x = (pBox->vMax.x - pRay->vOrigin.x) / pRay->vDirection.x + 0.001f;
	tmax.y = (pBox->vMax.y - pRay->vOrigin.y) / pRay->vDirection.y + 0.001f;
	tmax.z = (pBox->vMax.z - pRay->vOrigin.z) / pRay->vDirection.z + 0.001f;

	Vector3 real_min;
	real_min.x = min(tmin.x, tmax.x);
	real_min.y = min(tmin.y, tmax.y);
	real_min.z = min(tmin.z, tmax.z);
	Vector3 real_max;
	real_max.x = max(tmin.x, tmax.x);
	real_max.y = max(tmin.y, tmax.y);
	real_max.z = max(tmin.z, tmax.z);

	float minmax = min(min(real_max.x, real_max.y), real_max.z);
	float maxmin = max(max(real_min.x, real_min.y), real_min.z);

	if (minmax >= maxmin)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection* maxmin;
		return true;
	}
	return false;
}
bool Sample::IntersectRayToSphere(S_SPHERE* pSphere, S_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	// p0 - c
	Vector3 vDir = pRay->vOrigin - pSphere->vCenter;
	// B = 2(u dot (p0 - c ))
	float  fProj = pRay->vDirection.Dot(vDir);
	// 제한된 길이(세그먼트) 제외처리
	//if (pRay->fExtent > -1.0f 
	// && pRay->fExtent < fabs(fProj)) return false;

	float b = 2.0f * fProj;
	// C= (p0-c) dot (p0-c) - r*r
	float c = vDir.Dot(vDir) - (pSphere->fRadius * pSphere->fRadius);
	// D = B*B - 4*AC;
	float fDiscriminant = (b*b) - (4.0f*c);
	if (fDiscriminant < 0.0f)
	{
		return false;
	}
	fDiscriminant = sqrtf(fDiscriminant);
	float t0 = (-b + fDiscriminant) / 2.0f;
	float t1 = (-b - fDiscriminant) / 2.0f;

	if (t0 >= 0.0f)
	{
		//m_vIntersection = pRay->vOrigin + pRay->vDirection * t0;
		return true;
	}
	if (t1 >= 0.0f)
	{
		//m_vIntersection = pRay->vOrigin + pRay->vDirection * t1;
		return true;
	}
	return false;
}
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
					m_fHeightList[row * desc.Width + col] = uRed;	/// DWORD이므로 pitch/4	
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
	m_Camera.CreateViewMatrix({ 0,10,-1 }, { 0,0,0 });

	m_HeightMap.CreateHeightMap(g_pd3dDevice, g_pImmediateContext,
	L"../../data/map/HEIGHT_CASTLE.bmp");

	SMapDesc desc;
	desc.iNumCols = m_HeightMap.m_iNumCols; //257
	desc.iNumRows = m_HeightMap.m_iNumRows; //257
	desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/castle.jpg";
	desc.szVS = L"../../data/shader/VS.txt";
	desc.szPS = L"../../data/shader/PS.txt";
	m_HeightMap.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);

	//m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);
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
// 폴리곤안에 위치하는지 판단한다.
bool    Sample::PointInPolygon(
	Vector3 vert,
	Vector3 faceNormal,
	Vector3 v0,
	Vector3 v1,
	Vector3 v2)
{
	Vector3 e0, e1, iInter, vNormal;
	// v0    v1
	//       v
	// v2    v3
	//
	//     v

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

	//     v1
	//     v
	// v0      v2
	//
	//     v
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
	m_bSelect = false;
	if (g_Input.GetKey(VK_RBUTTON) == KEY_HOLD)
	{
		m_bSelect = true;
		// ray
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

		m_Ray.vDirection = vRayDir;
		m_Ray.vOrigin = vRayOrigin;
		//// face list
		//Vector3 v0, v1, v2;
		//for (int iFace = 0;
		//	iFace < m_Map.m_IndexList.size() / 3;
		//	iFace++)
		//{
		//	v0 = m_Map.m_VertexList[m_Map.m_IndexList[iFace * 3 + 0]].p;
		//	v1 = m_Map.m_VertexList[m_Map.m_IndexList[iFace * 3 + 1]].p;
		//	v2 = m_Map.m_VertexList[m_Map.m_IndexList[iFace * 3 + 2]].p;

		//	Vector3 vEnd = vRayOrigin + vRayDir * 1000.0f;
		//	Vector3 vNormal = (v1 - v0).Cross(v2 - v0);
		//	vNormal.Normalize();
		//	if (GetIntersection(vRayOrigin, vEnd,
		//		vNormal, v0, v1, v2))
		//	{
		//		if (PointInPolygon(m_vIntersection,
		//			vNormal, v0, v1, v2))
		//		{
		//			vList[0] = v0;
		//			vList[1] = v1;
		//			vList[2] = v2;
		//			return true;
		//		}
		//	}
		//}
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
	//m_Map.Render(g_pImmediateContext);
	//m_Quadtree.Render(g_pImmediateContext);
	m_SelectNode.clear();
	for (int i = 0; i < m_Quadtree.m_leafList.size(); i++)
	{
		SNode* pNode = m_Quadtree.m_leafList[i];
		if (IntersectBox(&pNode->m_Box, &m_Ray))
		{
			m_SelectNode.push_back(pNode);
			//	//m_Quadtree.DrawNode(pNode, g_pImmediateContext);
		}
		//if (IntersectRayToSphere(&pNode->m_Sphere, &m_Ray))
		//{
		//	//m_SelectNode.push_back(pNode);
		//	m_Quadtree.DrawNode(pNode, g_pImmediateContext);
		//}
	}
	Vector3 pick;
	float fMaxDist = 99999;
	bool update = false;
	for (int iSelect = 0; iSelect < m_SelectNode.size(); iSelect++)
	{
		SNode* pNode = m_SelectNode[iSelect];
		if (GetIntersection(pNode))
		{
			float fDistance = (m_Ray.vOrigin - m_vIntersection).Length();
			if (fMaxDist > fDistance)
			{
				pick = m_vIntersection;
				fMaxDist = fDistance;
				update = true;
			}
		}
	}
	float fRadius = 20.0f;
	if (m_bSelect)
	{
		for (int i = 0; i < m_Map.m_VertexList.size(); i++)
		{
			float fDist = (m_Map.m_VertexList[i].p - pick).Length();
			if (fDist < fRadius)
			{
				Vector3 v = m_Map.m_VertexList[i].p;
				m_Map.m_VertexList[i].p.y = v.y + 1.0f - sinf((fDist / fRadius));
			}
		}
		g_pImmediateContext->UpdateSubresource(
			m_Map.m_pVertexBuffer, 0, NULL, &m_Map.m_VertexList.at(0), 0, 0);
	}


	for (auto node : m_Quadtree.m_leafList)
	{
		m_Quadtree.DrawNode(node, g_pImmediateContext);
	}

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