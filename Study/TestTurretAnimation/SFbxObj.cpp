#include "SFbxObj.h"

FbxManager* SFbxObj::g_pSDKManager = nullptr;
// Material(재질) 설정
std::string SFbxObj::ParseMaterial(FbxSurfaceMaterial* pMtrl)
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
			const CHAR* szFileName = tex->GetFileName();
			CHAR Drive[MAX_PATH];
			CHAR Dir[MAX_PATH];
			CHAR FName[MAX_PATH];
			CHAR Ext[MAX_PATH];
			_splitpath(szFileName, Drive, Dir, FName, Ext);
			std::string texName = FName;
			std::string ext = Ext;

			// tga형식 지원X 바꿔주기
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
void SFbxObj::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv) {

	FbxLayerElementUV *pFbxLayerElementUV = pUVSet;

	if (pFbxLayerElementUV == nullptr)
	{
		return;
	}

	switch (pFbxLayerElementUV->GetMappingMode()) // mapping 방식
	{
	case FbxLayerElementUV::eByControlPoint: // control point의 노말을 control point의 인덱스로 접근 ( 하나당 하나의 노말 )
	{
		switch (pFbxLayerElementUV->GetReferenceMode()) // mapping 정보가 저장되는 방식
		{
		// control point의 인덱스 또는 각 면의 정점의 인덱스를 통해 normal에 접근
		case FbxLayerElementUV::eDirect:
		{
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		// 각 면의 정점의 인덱스를 통해 normal 벡터를 가리킬 수 있는 인덱스를 부여받고, 
		// 이것을 통해 실제 normal에 접근
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
	case FbxLayerElementUV::eByPolygonVertex: // 버텍스의 노말을 버텍스의 인덱스로 접근 ( 하나의 edge에 모이는 정점의 수만큼 노말 )
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
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
SFbxObj::SFbxObj()
{
	m_pFbxImporter = nullptr;
	m_pFBXScene = nullptr;
}
bool SFbxObj::Load(std::string szFileName)
{
	// FBX 파일 정보 불러오기
	if (LoadFBX(szFileName))
	{
		return true;
	}
	return false;
}
bool SFbxObj::Initialize(std::string szFileName)
{
	if (g_pSDKManager == nullptr)
	{
		/// 인스턴스화
		// FBX SDK 관리자 객체 생성
		g_pSDKManager = FbxManager::Create();
		if (g_pSDKManager == nullptr) return false;
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

	// Importer에 FBX 파일 데이터 초기화
	bool bRet = m_pFbxImporter->Initialize(szFileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;
	// FBX 파일 내용을 Scene으로 가져오기
	bRet = m_pFbxImporter->Import(m_pFBXScene);

	//Import 이후 세팅 // Z축 변환
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFBXScene);
	FbxAxisSystem SceneAxisSystem = m_pFBXScene->GetGlobalSettings().GetAxisSystem();

	return true;
}


bool SFbxObj::LoadFBX(std::string szFileName)
{
	if (Initialize(szFileName) == false)
	{
		return false;
	}
	// 트리구조로 되어 있는 Scene의 정보를 가져오기 위함
	FbxNode* pFbxRootNode = m_pFBXScene->GetRootNode();
	ParseNode(pFbxRootNode, Matrix::Identity);
	ParseAnimation(m_pFBXScene);
	return true;
}
void SFbxObj::PreProcess(FbxNode* pNode)
{
	// 카메라와 빛 정보 제외
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
void SFbxObj::ParseMesh(FbxNode* pNode,
	FbxMesh*  pFbxMesh,
	SModelObject* pObj)
{
	std::vector<FbxLayerElementUV*> VertexUVSets;
	std::vector<FbxLayerElementMaterial*> pMaterialSetList;
	std::vector<FbxLayerElementVertexColor*> VertexColorSet;

	int iLayerCount = pFbxMesh->GetLayerCount(); // 모든 Layer를 불러온다 = 1
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer); // iLayer 얻는다
		// 버텍스 컬러
		if (pLayer->GetVertexColors() != NULL)
		{
			VertexColorSet.push_back(pLayer->GetVertexColors());
		}
		// UV
		if (pLayer->GetUVs() != NULL)
		{
			VertexUVSets.push_back(pLayer->GetUVs()); // pLayer에서 UV정보를 가져와서 push_back
		}
		if (pFbxMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
		{
			pMaterialSetList.push_back(pFbxMesh->GetLayer(iLayer)->GetMaterials()); // 레이어에 Material을 push_back
		}
	}

	//std::vector<std::string> fbxMaterialList;
	int iNumMtrl = pNode->GetMaterialCount(); // 모든 재질 정보를 가져온다
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(iMtrl); // iMtrl(재질) 얻기
		if (pMtrl == nullptr)
		{
			continue;
		}
		pObj->fbxMaterialList.push_back(to_mw(ParseMaterial(pMtrl))); // 텍스쳐 + 재질 push_back
	}

	// transform
	FbxAMatrix geom;
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	// 노말 변환 Nomal * (World Matrix^(-1))^T // World 행렬 이미 들어가 있음
	FbxAMatrix normalMat = geom;
	normalMat = normalMat.Inverse();
	normalMat = normalMat.Transpose();

	//pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(pNode->EvaluateGlobalTransform(1.0f)));
	FbxAMatrix globalMatrix = pNode->EvaluateLocalTransform();
	FbxAMatrix matrix = globalMatrix * geom;
	pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(matrix));

	int iPolyCount = pFbxMesh->GetPolygonCount();
	int iVertexCount = pFbxMesh->GetControlPointsCount(); // 정점 갯수
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints(); // 정점 위치

	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly); // Polygon 정점 수를 가져온다
		int iTriangleCount = iPolySize - 2;
		int iCornerIndices[3];
		S_TRIANGLE tri;

		// sub mtrl
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
				auto finalPos = geom.MultT(pVertexPositions[iCornerIndices[iIndex]]);
				v.p.x = finalPos.mData[0]; // x
				v.p.y = finalPos.mData[2]; // z
				v.p.z = finalPos.mData[1]; // y

				FbxColor color = FbxColor(1, 1, 1, 1);
				if (VertexColorSet.size())
				{
					ReadColor(pFbxMesh,
						VertexColorSet.size(),
						VertexColorSet[0],
						iCornerIndices[iIndex],
						iBasePolyIndex + iVertIndex[iIndex]);
				}
				v.c.x = (float)color.mRed;
				v.c.y = (float)color.mGreen;
				v.c.z = (float)color.mBlue;
				v.c.w = 1;

				FbxVector4 normal = ReadNormal(pFbxMesh,
					iCornerIndices[iIndex],
					iBasePolyIndex + iVertIndex[iIndex]);
				finalPos = normalMat.MultT(normal);
				v.n.x = finalPos.mData[0]; // x
				v.n.y = finalPos.mData[2]; // z
				v.n.z = finalPos.mData[1]; // y


				if (VertexUVSets.size())
				{
					// Texture UV
					for (int iUVIndex = 0; iUVIndex < 1/*VertexUVSets.size()*/; ++iUVIndex)
					{
						FbxLayerElementUV* pUVSet = VertexUVSets[iUVIndex];
						FbxVector2 uv(0, 0);
						ReadTextureCoord(
							pFbxMesh,
							pUVSet,
							iCornerIndices[iIndex],
							u[iIndex],
							uv);
						v.t.x = uv.mData[0];
						v.t.y = 1.0f - uv.mData[1];
					}
				}
				tri.vVertex[iIndex] = v;
			}
			if (iNumMtrl > 1)
			{
				pObj->subMesh.resize(iNumMtrl);
				pObj->subMesh[iSubMtrl].m_TriangleList.push_back(tri);
			}
			else
			{
				pObj->m_TriangleList.push_back(tri);
			}
		}
		iBasePolyIndex += iPolySize;
	}
}
Matrix SFbxObj::ParseTransform(FbxNode* pNode, Matrix& matParentWorld)
{
	Matrix matWorld = Matrix::Identity;
	return matWorld;
}
void SFbxObj::ParseNode(
	FbxNode* pNode,
	Matrix  matParent)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}
	SModelObject* obj = new SModelObject;
	obj->m_szName = to_mw(pNode->GetName());
	m_sMeshMap.push_back(obj);

	// world matrix
	Matrix matWorld = ParseTransform(pNode, matParent); // 월드행렬 정보(정규화 상태)
	obj->m_matWorld = matWorld;
	if (pNode->GetMesh() != nullptr)
	{
		ParseMesh(pNode, pNode->GetMesh(), obj); // vb, ib
	}
	// 모든 Child(정보)를 순회하며 원하는 정보 얻기 (노드 탐색)
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		ParseNode(pChildNode, matWorld);
	}
}
void SFbxObj::ParseAnimation(FbxScene* pFbxScene)
{
	FbxArray<FbxString*> AnimStackNameArray;
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);	// 애니메이션 이름을 배열에 채운다
	int iAnimStackCount = AnimStackNameArray.GetCount();	// 애니메이션 갯수
	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i));
	}
}
// 정점 노말을 읽는다
FbxVector4 SFbxObj::ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0); // 노말 얻기

	FbxVector4 result; // 노말 벡터를 저장할 벡터

	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}
FbxColor SFbxObj::ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount,
							FbxLayerElementVertexColor* pVertexColorSet,
							DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
	{
		switch (pVertexColorSet->GetMappingMode())
		{
		case FbxLayerElement::eByControlPoint:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
		case FbxLayerElement::eByPolygonVertex:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				int iColorIndex = dwVertexIndex;
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}break;
		}
	}
	return Value;
}