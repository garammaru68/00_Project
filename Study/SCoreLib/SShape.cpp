#include "SShape.h"
SShape::SShape()
{

}
SShape::~SShape()
{

}
bool	SShapeBox::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(24);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));
	// 뒷면
	m_VertexList[4] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));

	// 오른쪽
	m_VertexList[8] = PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

	// 왼쪽
	m_VertexList[12] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));

	// 윗면
	m_VertexList[16] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

	// 아랫면
	m_VertexList[20] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));
	return true;
}
bool    SShapeBox::CreateIndexData()
{
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;
	return true;
}
SShapeBox::SShapeBox()
{

}
SShapeBox::~SShapeBox()
{

}

bool SShapePlane::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(4);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));
	return true;
}

bool SShapePlane::CreateIndexData()
{
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 3;
	return true;
}

SShapePlane::SShapePlane()
{
}

SShapePlane::~SShapePlane()
{
}

//HRESULT SShapeLine::SetInputLayout()
//{
//	HRESULT hr = S_OK;
//
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//	UINT numElements = sizeof(layout) / sizeof(layout[0]);
//	m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice, m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
//		m_dxobj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
//	return hr;
//}
//bool SShapeLine::CreateVertexData()
//{
//	m_LineVertexList.resize(2);
//	m_LineVertexList[0].p = Vector3(0.0f, 0.0f, 0.0f);
//	m_LineVertexList[0].c = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
//	m_LineVertexList[1].p = Vector3(1.0f, 0.0f, 0.0f);
//	m_LineVertexList[1].c = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
//
//	m_dxobj.m_iVertexSize = sizeof(PC_VERTEX);
//	m_dxobj.m_iNumVertex = (UINT)m_LineVertexList.size();
//	return true;
//}
//bool SShapeLine::CreateIndexData()
//{
//	m_IndexList.resize(2);
//	m_IndexList[0] = 0;
//	m_IndexList[1] = 1;
//
//	m_dxobj.m_iNumIndex = (UINT)m_IndexList.size();
//	m_dxobj.m_iIndexSize = sizeof(DWORD);
//	return true;
//}
//HRESULT SShapeLine::CreateVertexBuffer()
//{
//	if (m_dxobj.m_iNumVertex <= 0) return S_OK;
//	m_dxobj.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice,
//		&m_LineVertexList.at(0),
//		m_dxobj.m_iNumVertex,
//		m_dxobj.m_iVertexSize));
//	return S_OK;
//}
//bool SShapeLine::Draw(ID3D11DeviceContext* pContext, Vector3 vStart, Vector3 vEnd, Vector4 vColor)
//{
//	PC_VERTEX vertices[2];
//	vertices[0].p = vStart;
//	vertices[0].c = vColor;
//	vertices[1].p = vEnd;
//	vertices[1].c = vColor;
//	// 동적 리소스 갱신 방법 1 : D3D11_USAGE_DEFAULT 사용
//	pContext->UpdateSubresource(GetVB(), 0, NULL, &vertices, 0, 0);
//	return Render(pContext);
//}
////--------------------------------------------------------------------------------------
//// 
////--------------------------------------------------------------------------------------
//HRESULT SShapeLine::CreateResource()
//{
//	m_dxobj.m_iPrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
//	return S_OK;
//}
////////////////////////////////////////////
bool SShapeLine::Draw(ID3D11DeviceContext* pd3dContext,
	Vector3 p, Vector3 e, Vector4 c)
{
	m_VertexList[0] = PNCT_VERTEX(p, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(e, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(1.0f, 0.0f));
	pd3dContext->UpdateSubresource(
		m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return Render(pd3dContext);
}
bool SShapeLine::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(2);
	m_VertexList[0] = PNCT_VERTEX(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(100.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	return true;
}

bool SShapeLine::CreateIndexData()
{
	m_IndexList.resize(2);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	return true;
}

SShapeLine::SShapeLine()
{
	m_szPixelShader = "PSLine";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

SShapeLine::~SShapeLine()
{
}
