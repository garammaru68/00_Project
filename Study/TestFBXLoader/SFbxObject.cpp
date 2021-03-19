#include "SFbxObject.h"
FbxManager* SFbxObject::g_pSDKManager = nullptr;
// Material(재질) 설정
std::string SFbxObject::ParseMaterial(FbxSurfaceMaterial* pMtrl)
{
	std::string name = pMtrl->GetName();
	// Material 속성(특성)
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
		// 0번 텍스쳐 정보를 들고온다
		const FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
		if (tex != nullptr)
		{
			return tex->GetFileName();
		}
	}
	return std::string("");
}
void SFbxObject::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv)
{
	FbxLayerElementUV *pFbxLayerElementUV = pUVSet;
	if (pFbxLayerElementUV == nullptr)
	{
		return;
	}
	switch (pFbxLayerElementUV->GetMappingMode()) // mapping 방식
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (pFbxLayerElementUV->GetReferenceMode()) // mapping 정보가 저장되는 방식
		{
		case FbxLayerElementUV::eDirect:
		{
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = pFbxLayerElementUV->GetIndexArray().GetAt(vertexIndex);
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(id);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		}
		break;
	}
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
			// Always enters this part for the example model
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[1];
			break;
		}
		}
		break;
	}
	}
}
SFbxObject::SFbxObject()
{
	m_pFbxImporter	= nullptr;
	m_pFBXScene		= nullptr;
}
bool SFbxObject::Load(std::string szFileName)
{
	// FBX 파일 정보 불러오기
	if (LoadFBX(szFileName))
	{
		return true;
	}
	return false;
}
bool SFbxObject::LoadFBX(std::string szFileName)
{
	// SDK
	if (Initialize(szFileName) == false)
	{
		return false;
	}
	// INode*
	FbxNode* pFbxRootNode = m_pFBXScene->GetRootNode(); // 트리구조로 되어 있는 Scene의 정보를 가져오기 위함
	PreProcess(pFbxRootNode);
	ParseNode(pFbxRootNode, Matrix::Identity);
	return true;
}
bool SFbxObject::Initialize(std::string szFileName)
{
	if (g_pSDKManager == nullptr)
	{
		/// 인스턴스화
		// FBX SDK 관리자 객체 생성
		g_pSDKManager = FbxManager::Create();
		if (g_pSDKManager == nullptr)return false;
		// FBX IO 객체 설정 및 셋팅
		FbxIOSettings* ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
		if (ios == nullptr) return false;
		// 씬의 정보를 가져오거나 내보냄 (camera, light, mesh, texture, material, animation, 사용자 정의 속성 등)
		g_pSDKManager->SetIOSettings(ios);
	}
	if (m_pFbxImporter == nullptr)
	{
		// FbxImporter 객체 생성
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
	// FBX 파일 내용을 Scene으로 가져오기
	bRet = m_pFbxImporter->Import(m_pFBXScene);

	// 삼각형화
	//FbxGeometryConverter lGeomConverter(g_pSDKManager);
	//lGeomConverter.Triangulate(m_pFBXScene, true);
	return true;
}
void SFbxObject::PreProcess(FbxNode* pNode)
{
	// 카메라와 빛 정보 제외
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}
	// 모든 Child(정보)를 순회하면 원하는 정보 얻기 (노드 탐색)
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		// 노드(도형) 타입 정하기
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
void SFbxObject::ParseNode(FbxNode* pNode, Matrix matParent)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera() || pNode->GetLight())) // Camera와 Light 정보 제외
	{
		return;
	}

	SObject* obj = new SObject;
	obj->m_szName = to_mw(pNode->GetName());
	m_sMeshMap[pNode] = obj;

	// world matrix
	Matrix matWorld = ParseTransform(pNode, matParent); // 월드행렬 정보(정규화 상태)
	obj->m_matWorld = matWorld;

	if (pNode->GetMesh() != nullptr)
	{
		ParseMesh(pNode, pNode->GetMesh(), obj); // vb, ib
	}
	// 모든 Child(정보)를 순회하면 원하는 정보 얻기 (노드 탐색)
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		ParseNode(pChildNode, matWorld);
	}
}
Matrix SFbxObject::ParseTransform(FbxNode* pNode, Matrix& matParentWorld)
{
	Matrix matWorld = Matrix::Identity;
	return matWorld;
}
void SFbxObject::ParseMesh(FbxNode* pNode, FbxMesh* pFbxMesh, SObject* pObj)
{
	std::vector<FbxLayerElementUV*> VertexUVSets;
	int iLayerCount = pFbxMesh->GetLayerCount();	// 모든 Layer를 불러온다 = 1
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);	// iLayer 얻는다

		if (pLayer->GetVertexColors() != NULL)
		{

		}
		if (pLayer->GetUVs() != NULL)
		{
			VertexUVSets.push_back(pLayer->GetUVs());	// pLayer에서 UV정보를 가져와서 push_back
		}
	}

	std::vector<std::string> fbxMaterialList;
	int iNumMtrl = pNode->GetMaterialCount();	// 모든 재질 정보를 가져온다
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(iMtrl);	// iMtrl(재질) 얻기
		if (pMtrl == nullptr)
		{
			continue;
		}
		fbxMaterialList.push_back(ParseMaterial(pMtrl)); // 텍스쳐 + 재질 push_back
	}
	// NumFace
	int iPolyCount = pFbxMesh->GetPolygonCount();
	int iVertexCount = pFbxMesh->GetControlPointsCount(); // 정점 갯수
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints(); // 정점 위치

	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);	// Polygon 정점 수를 가져온다
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
				for (int iUVIndex = 0; iUVIndex < VertexUVSets.size(); ++iUVIndex)
				{
					FbxLayerElementUV* pUVSet = VertexUVSets[iUVIndex];
					FbxVector2 uv(0, 0);
					ReadTextureCoord(pFbxMesh, pUVSet,
									 iCornerIndices[iIndex], u[iIndex],
									 uv);
					v.t.x = uv.mData[0]; // x
					v.t.y = 1.0f - uv.mData[1]; // z

				}
				tri.vVertex[iIndex] = v;
			}
			pObj->m_TriangleList.push_back(tri);
		}
	}
}