#include "SFbxObj.h"

SFbxObj::SFbxObj()
{
	m_pFbxImporter = nullptr;
	m_pFbxScene = nullptr;
}
bool SFbxObj::Load(std::string szFileName)
{
	if (LoadFBX(szFileName))
	{
		return true;
	}
	return false;
}
bool SFbxObj::LoadFBX(std::string szFileName)
{
	if (Initialize(szFileName) == false)
	{
		return false;
	}
	FbxNode* pFbxRootNode = m_pFbxScene->GetRootNode();
	PreProcess(pFbxRootNode);
	ParseNode(pFbxRootNode, Matrix::Identity);
}
FbxManager* SFbxObj::g_pSDKManager = nullptr;
bool SFbxObj::Initialize(std::string szFileName)
{
	if (g_pSDKManager == nullptr)
	{
		g_pSDKManager = FbxManager::Create();
		if (g_pSDKManager == nullptr) return false;
		FbxIOSettings* ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
		if (ios == nullptr) return false;
		g_pSDKManager->SetIOSettings(ios);
	}
	if (m_pFbxImporter == nullptr)
	{
		m_pFbxImporter = FbxImporter::Create(g_pSDKManager, "");
		if (m_pFbxImporter == nullptr) return false;
	}
	if (m_pFbxScene == nullptr)
	{
		m_pFbxScene = FbxScene::Create(g_pSDKManager, "");
		if (m_pFbxScene == nullptr) return false;
	}

	bool bRet = m_pFbxImporter->Initialize(szFileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	// Z좌표 바꾸기
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);
	FbxAxisSystem SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();

	return true;
}
void SFbxObj::PreProcess(FbxNode* pNode)
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
		m_pNodeMap[pNode] = m_pMatrixList.size();
		m_pMatrixList.push_back(mat);
	}
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		//if (pChildNode->GetNodeAttribute() != NULL)
		//{
		//	FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
		//	if (AttributeType != FbxNodeAttribute::eMesh &&
		//		AttributeType != FbxNodeAttribute::eSkeleton&&
		//		AttributeType != FbxNodeAttribute::eNull)
		//	{
		//		continue;
		//	}
		//}
		PreProcess(pChildNode);
	}
}
void SFbxObj::ParseNode(FbxNode* pNode, Matrix matParent)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera()) || (pNode->GetLight()))
	{
		return;
	}
	SModelObj* obj = new SModelObj;
	obj->m_szName = to_mw(pNode->GetName());
	m_sNodeMap[pNode] = obj;
	m_sNodeList.push_back(obj);

	Matrix matWorld = ParseTransform(pNode, matParent);
	obj->m_matWorld = matWorld;
	if (pNode->GetMesh() != nullptr)
	{
		ParseMesh(pNode, pNode->GetMesh(), obj);
	}
}
Matrix SFbxObj::ParseTransform(FbxNode* pNode, Matrix& matParentWorld)
{
	Matrix matWorld = Matrix::Identity;
	return matWorld;
}
void SFbxObj::ParseMesh(FbxNode* pNode, FbxMesh* pFbxMesh, SModelObj* pObj)
{
	std::vector<FbxLayerElementUV*>				VertexUVSets;
	std::vector<FbxLayerElementMaterial*>		pMaterialSetList;
	std::vector<FbxLayerElementVertexColor*>	VertexColorSet;

	int iLayerCount = pFbxMesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
		// 버텍스 컬러
		if (pLayer->GetVertexColors() != NULL)
		{
			VertexColorSet.push_back(pLayer->GetVertexColors());
		}
		// UV
		if (pLayer->GetUVs() != NULL)
		{
			VertexUVSets.push_back(pLayer->GetUVs());
		}
		// Material이 있을 경우 pMaterialSetList에 push_back
		if (pFbxMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
		{
			pMaterialSetList.push_back(pFbxMesh->GetLayer(iLayer)->GetMaterials());
		}
	}
	
	// Material 셋팅
	int iNumMtrl = pNode->GetMaterialCount();
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(iMtrl);
		if (pMtrl == nullptr)
		{
			continue;
		}
		pObj->fbxMaterilList.push_back(to_mw(ParseMaterial(pMtrl)));
	}
	if (iNumMtrl > 1)
	{
		pObj->subMesh.resize(iNumMtrl);
	}

	// transform
	FbxAMatrix geom;
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	// 노말 변환 Nomal * (World Matrix^(-1))^T
	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	// 변환 행렬 반환 후, Z축 Y축 바꾸기 (World 행렬 이미 들어가 있음)
	pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(pNode->EvaluateGlobalTransform(1.0f)));
	//FbxAMatrix globalMatrix = pNode->EvaluateLocalTransform();
	//FbxAMatrix matrix = globalMatrix * geom;
	///pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(matrix));

	int iPolyCount = pFbxMesh->GetPolygonCount();
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints(); // 제어점 포이터 배열 반환

	bool bSkinnedMesh = ParseMeshSkinningMap(pFbxMesh, pObj->WeightList);
	pObj->m_bSkinnedMesh = bSkinnedMesh;

	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;
		int iCornerIndices[3];
		S_TRIANGLE tri;

		// Sub Material
		int iSubMtrl = 0;
		if (pMaterialSetList.size() > 0)
		{
			switch (pMaterialSetList[0]->GetMappingMode())
			{
				case FbxLayerElement::eByPolygon:
				{
					switch (pMaterialSetList[0]->GetReferenceMode())
					{
						case FbxLayerElement::eIndex:
						{
							iSubMtrl = iPoly;
						}break;
						case FbxLayerElement::eIndexToDirect:
						{
							iSubMtrl = pMaterialSetList[0]->GetIndexArray().GetAt(iPoly);
						}break;
					}
				}
				default:
				{
					break;
				}
			}
		}

		for (int iTriangle = 0; iTriangle < iTriangleCount; iTriangle++)
		{
			INT iVertIndex[3] = { 0, (INT)iTriangle + 2, (INT)iTriangle + 1 };

			iCornerIndices[0] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[0]);
			iCornerIndices[1] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[1]);
			iCornerIndices[2] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[2]);

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
		}
	}
}
std::string SFbxObj::ParseMaterial(FbxSurfaceMaterial* pMtrl) // Material Texture 찾아서 저장
{
	std::string name = pMtrl->GetName();
	// 속성 찾아서 저장
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
		// 0번 텍스쳐 정보를 가져온다
		const FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
		if (tex != nullptr)
		{
			const CHAR* szFileName = tex->GetFileName();
			CHAR Drive[MAX_PATH];
			CHAR Dir[MAX_PATH];
			CHAR FName[MAX_PATH];
			CHAR Ext[MAX_PATH];
			_splitpath(szFileName, Drive, Dir, FName, Ext);
			std::string texName = FName;
			std::string ext = Ext;
			// tga 형식 파일 지원 X, dds로 바꿔주기
			if (ext == ".tga" || ext == ".TGA")
			{
				ext.clear();
				ext = ".dds";
			}
			texName += ext;
			return texName;
		}
	}
	return std::string("");
}