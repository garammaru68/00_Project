#include "TFrustum.h"

bool TFrustum::Create(ID3D11Device* pd3dDevice)
{
	if (!m_FrustumObj.Create(pd3dDevice, L"vs.txt",
		L"ps.txt",
		L""))
	{
		return false;
	}
	
	for (auto v : m_FrustumObj.m_VertexList)
	{
		m_VertexList.push_back(v);
	}
	m_VertexList[0].p = Vector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[1].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[2].p = Vector3(1.0f, -1.0f, 0.0f);
	m_VertexList[3].p = Vector3(-1.0f, -1.0f, 0.0f);	
	
	m_VertexList[8].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[11].p = Vector3(1.0f, -1.0f, 0.0f);
	
	m_VertexList[13].p = Vector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[14].p = Vector3(-1.0f, -1.0f, 0.0f);
	
	m_VertexList[18].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[19].p = Vector3(-1.0f, 1.0f, 0.0f);

	
	m_VertexList[20].p = Vector3(-1.0f, -1.0f, 0.0f);
	m_VertexList[21].p = Vector3(1.0f, -1.0f, 0.0f);


	return true;
}