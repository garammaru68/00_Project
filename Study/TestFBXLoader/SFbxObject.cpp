#include "SFbxObject.h"
FbxManager* SFbxObject::g_pSDKManager = nullptr;
SFbxObject::SFbxObject()
{
	m_pFbxImporter	= nullptr;
	m_pFBXScene		= nullptr;
}
bool SFbxObject::Load(std::string szFileName)
{
	if (LoadFBX(szFileName))
	{
		return true;
	}
	return false;
}
bool SFbxObject::Initialize(std::string szFileName)
{
	if (g_pSDKManager == nullptr)
	{
		g_pSDKManager = FbxManager::Create();
		if (g_pSDKManager == nullptr)return false;
		FbxIOSettings* ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
		if (ios == nullptr) return false;
		g_pSDKManager->SetIOSettings(ios);
	}
	if (m_pFbxImporter == nullptr)
	{
		m_pFbxImporter = FbxImporter::Create(g_pSDKManager, "");
		if (m_pFbxImporter == nullptr) return false;
	}
	if (m_pFBXScene == nullptr)
	{
		m_pFBXScene = FbxScene::Create(g_pSDKManager, "");
			if (m_pFBXScene == nullptr) return false;
	}

	bool bRet = m_pFbxImporter->Initialize(szFileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;
	bRet = m_pFbxImporter->Import(m_pFBXScene);

	// »ï°¢ÇüÈ­
	FbxGeometryConverter lGeomConverter(g_pSDKManager);
	lGeomConverter.Triangulate(m_pFBXScene, true);
	return true;
}
bool SFbxObject::LoadFBX(std::string szFileName)
{
	if (Initialize(szFileName) == false)
	{
		return false;
	}
	// INode*
	FbxNode* pFbxRootNode = m_pFBXScene->GetRootNode();
	PreProcess(pFbxRootNode);
	ParseNode(pFbxRootNode, Matrix::Identity);
	ParseAnimation(m_pFBXScene);
	return true;
}
void SFbxObject::PreProcess(FbxNode* pNode)
{
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}
	Matrix mat = mat.Identity;
	auto iter = m_dxMatrixMap.find(pNode->GetName());
	if (iter == m_dxMatrixMap.end())
	{
		m_dxMatrixMap[pNode->GetName()] = mat;
	}
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		if (pChildNode->GetNodeAttribute() != NULL)
		{
			FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
			if (AttributeType != FbxNodeAttribute::eMesh &&
				AttributeType != FbxNodeAttribute::eSkeleton&&
				AttributeType != FbxNodeAttribute::eNull)
			{
				continue;
			}
		}
		PreProcess(pChildNode);
	}
}
void SFbxObject::ParseMesh(FbxNode* pNode, FbxMesh* pFbxMesh, SObject* pObj)
{
	int iNumMtrl = pNode->GetMaterialCount();
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{

	}
	// NumFace
	int iPolyCount = pFbxMesh->GetPolygonCount();
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();

	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;

		int iCornerIndices[3];
		S_TRIANGLE tri;
		for (int iTriangle = 0; iTriangle < iTriangleCount; iTriangle++)
		{
			INT iVertIndex[3] = { 0, (INT)iTriangle + 2, (INT)iTriangle + 1 };

			iCornerIndices[0] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[0]);
			iCornerIndices[1] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[1]);
			iCornerIndices[2] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[2]);
			// 0, 9, 8
			FbxVector4 vNormals[3];
			ZeroMemory(vNormals, 3 * sizeof(FbxVector4));
			INT iPolyIndex = (INT)iPoly;
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[0], vNormals[0]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[1], vNormals[1]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[2], vNormals[2]);

			int u[3];
			u[0] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[0]);
			u[1] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[1]);
			u[2] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[2]);

			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				PNCT_VERTEX v;
				v.p.x = pVertexPositions[iCornerIndices[iIndex]].mData[0]; // x
				v.p.y = pVertexPositions[iCornerIndices[iIndex]].mData[2]; // z
				v.p.z = pVertexPositions[iCornerIndices[iIndex]].mData[1]; // y
				v.c = Vector4(1, 1, 1, 1);
				v.n.x = vNormals[iCornerIndices[iIndex]].mData[0]; // x
				v.n.y = vNormals[iCornerIndices[iIndex]].mData[2]; // z
				v.n.z = vNormals[iCornerIndices[iIndex]].mData[1]; // y
				v.t.x = 0.0f; // x
				v.t.y = 1.0f - 1.0f; // z
				tri.vVertex[iIndex] = v;
			}
			pObj->m_TriangleList.push_back(tri);
		}
	}
}
Matrix SFbxObject::ParseTransform(FbxNode* pNode, Matrix& matParentWorld)
{
	Matrix matWorld = Matrix::Identity;
	return matWorld;
}
void SFbxObject::ParseNode(FbxNode* pNode, Matrix matParent)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}
	//shared_ptr<SObject> obj = make_shared<SObject>();
	SObject* obj = new SObject;
	obj->m_szName = to_mw(pNode->GetName());
	m_sMeshMap[pNode] = obj;
	Matrix matWorld = ParseTransform(pNode, matParent);
	//m_pMeshList.push_back(obj);
}
void SFbxObject::ParseAnimation(FbxScene* pFBXScene)
{

}