#include "Sample.h"

bool Sample::Init()
{
	m_fbxObj.Load("../../data/object/box.fbx");

	for (auto data : m_fbxObj.m_sMeshMap)
	{
		SObject* pObject = (SObject*)data.second;
		if (pObject->m_TriangleList.size() <= 0) continue;

		pObject->m_VertexList.resize(pObject->m_TriangleList.size() * 3);
		for (int iFace = 0; iFace < pObject->m_TriangleList.size(); iFace++)
		{
			int iIndex = iFace * 3;
			pObject->m_VertexList[iIndex + 0] = pObject->m_TriangleList[iFace].vVertex[0];
			pObject->m_VertexList[iIndex + 1] = pObject->m_TriangleList[iFace].vVertex[1];
			pObject->m_VertexList[iIndex + 2] = pObject->m_TriangleList[iFace].vVertex[2];
		}
		if (!pObject->Create(SBASIS_CORE_LIB::g_pd3dDevice,
							 L"vs.txt",
							 L"ps.txt", 
							 L"../../data/titleA.jpg"))
		{
			return false;
		}
	}
}
bool Sample::Frame()
{

}
bool Sample::Render()
{

}
bool Sample::Release()
{

}