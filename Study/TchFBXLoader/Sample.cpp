#include "Sample.h"
bool Sample::Init()
{	
	m_Camera.CreateViewMatrix({ 0,50,-50 }, { 0,0,0 });
	m_fbxObj.Load("../../data/object/sphereBox.fbx");


	for (auto data : m_fbxObj.m_tMeshMap)
	{
		TModelObject* pObject = (TModelObject*)data.second;
		if (pObject->m_TriangleList.size() <= 0) continue;
		pObject->m_VertexList.resize(pObject->m_TriangleList.size()*3);
		for (int iFace = 0; iFace < pObject->m_TriangleList.size(); iFace++)
		{
			int iIndex = iFace * 3;
			pObject->m_VertexList[iIndex + 0] = 
				pObject->m_TriangleList[iFace].vVertex[0];
			pObject->m_VertexList[iIndex + 1] = 
				pObject->m_TriangleList[iFace].vVertex[1];
			pObject->m_VertexList[iIndex + 2] = 
				pObject->m_TriangleList[iFace].vVertex[2];
		}
		T_STR loadTexName = L"../../data/object/";
		loadTexName += pObject->fbxMaterialList[0];
		if (!pObject->Create(SBASIS_CORE_LIB::g_pd3dDevice,
			L"vs.txt",
			L"ps.txt",
			loadTexName))
		{
			return false;
		}
	}
	
	return true;
}
bool Sample::Frame()
{
	
	return true;
}
bool Sample::Render() 
{
	for (auto data : m_fbxObj.m_tMeshMap)
	{
		SObject* pObject = (SObject*)data.second;
		if (pObject->m_TriangleList.size() <= 0) continue;

		pObject->SetMatrix(&pObject->m_matWorld, &m_pMainCamera->m_matView,
			&m_pMainCamera->m_matProj);
		pObject->Render(g_pImmediateContext);
	}
	return true;
}
bool Sample::Release()
{
	return true;
}