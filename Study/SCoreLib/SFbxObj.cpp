#include "SFbxObj.h"

class ExportMaterialParameter
{
public:
	enum ExportMaterialParameterFlags
	{
		EMPF_NONE = 0,
		EMPF_BUMPMAP = 1,
		EMPF_DIFFUSEMAP = 2,
		EMPF_NORMALMAP = 4,
		EMPF_SPECULARMAP = 8,
		EMPF_ALPHACHANNEL = 16,
		EMPF_AOMAP = 32,
	};
	ExportMaterialParameter()
	{
		ZeroMemory(this, sizeof(ExportMaterialParameter));
	}
};

FbxManager* SFbxObj::g_pSDKManager = nullptr;


std::string SFbxObj::ParseMaterial(FbxSurfaceMaterial* pMtrl)
{
	std::string name = pMtrl->GetName();
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
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

	if (pFbxLayerElementUV == nullptr) {
		return;
	}

	switch (pFbxLayerElementUV->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
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
SFbxObj::SFbxObj()
{
	m_pFbxImporter = nullptr;
	m_pFBXScene = nullptr;
}
void SFbxObj::CuttingScene(std::wstring strSceneName, int iFirstFrame, int iLastFrame)
{
	SScene scene = SScene(m_Scene);
	scene.iFirstFrame = min(max(iFirstFrame, scene.iFirstFrame), scene.iLastFrame);
	scene.iLastFrame = max(min(iLastFrame, scene.iLastFrame), scene.iFirstFrame);
	auto iter = m_SceneMap.find(strSceneName);
	if (iter == m_SceneMap.end())
	{
		m_SceneMap.insert(make_pair(strSceneName, scene));
		return;
	}
	iter->second = scene;
}
bool SFbxObj::ChangeScene(std::wstring strSceneName)
{
	auto iter = m_SceneMap.find(strSceneName);
	if (iter == m_SceneMap.end()) return false;
	m_pCurrentScene = &iter->second;
	m_fTick = m_pCurrentScene->iFirstFrame * m_pCurrentScene->iTickPerFrame;
	return false;
}
bool SFbxObj::Load(std::string szFileName)
{
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
	if (m_pFBXScene == nullptr)
	{
		m_pFBXScene = FbxScene::Create(g_pSDKManager, "");
		if (m_pFBXScene == nullptr) return false;
	}
	//FbxAxisSystem m_SceneAxisSystem = m_pFBXScene->GetGlobalSettings().GetAxisSystem();
	////FbxAxisSystem::Max.ConvertScene(m_pFBXScene);
	//m_SceneAxisSystem = m_pFBXScene->GetGlobalSettings().GetAxisSystem();
	//FbxAxisSystem OurAxisSystem(FbxAxisSystem::eZAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
	////if (m_SceneAxisSystem != OurAxisSystem)
	////{
	//OurAxisSystem.ConvertScene(m_pFBXScene);
	////}


	bool bRet = m_pFbxImporter->Initialize(szFileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;



	bRet = m_pFbxImporter->Import(m_pFBXScene);
	//Import 이후 세팅
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFBXScene);
	FbxAxisSystem SceneAxisSystem = m_pFBXScene->GetGlobalSettings().GetAxisSystem();
	// 삼각형화
	//FbxGeometryConverter lGeomConverter(g_pSDKManager);
	//lGeomConverter.Triangulate(m_pFBXScene, true);
	return true;
}
bool SFbxObj::LoadFBX(std::string szFileName)
{
	if (Initialize(szFileName) == false)
	{
		return false;
	}

	FbxNode* pFbxRootNode = m_pFBXScene->GetRootNode();
	CStopwatch stopwatch;
	PreProcess(pFbxRootNode);
	for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
	{
		FbxNode* pNode = m_pFbxNodeList[iNode];
		//shared_ptr<TObject> obj = make_shared<TObject>();	
		SModelObj* obj = new SModelObj;
		obj->m_szName = to_mw(pNode->GetName());
		obj->m_pParentObject = m_sNodeMap[pNode->GetParent()];
		m_sNodeMap[pNode] = obj;
		m_sNodeList.push_back(obj);
		if (pNode->GetMesh() != nullptr)
		{
			ParseMesh(pNode, pNode->GetMesh(), obj);
		}
	}
#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
	auto anim = m_pFBXScene->GetAnimationEvaluator();
#else
	auto anim = m_pFBXScene->GetEvaluator();
#endif
	ParseAnimation(m_pFBXScene);

	float fCurrentTime = 0.0f;
	while (fCurrentTime <= m_Scene.fLastTime)
	{
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
		{
			FbxNode* pNode = m_pFbxNodeList[iNode];
			auto data = m_sNodeMap.find(pNode);
			FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t);
			AddKey(pNode, mat, fCurrentTime);
			/*TAnimTrack track;
			track.iTick = fCurrentTime * 30 * 160;
			track.mat = DxConvertMatrix(ConvertMatrixA(mat));*/
			//data->second->animlist.push_back(track);				
		}
		fCurrentTime += m_Scene.fDeltaTime * 1;
	}
	stopwatch.Output(L"Load");

	m_pCurrentScene = &m_Scene;
	return true;
}
void SFbxObj::PreProcess(FbxNode* pNode)
{
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}
	Matrix mat = mat.Identity;
	m_pFbxNodeMap.insert(make_pair(pNode, m_pMatrixList.size()));
	m_pMatrixList.push_back(mat);
	m_pFbxNodeList.push_back(pNode);

	//shared_ptr<TObject> obj = make_shared<TObject>();	
	//SModelObj* obj = new SModelObj;
	//obj->m_szName = to_mw(pNode->GetName());
	//obj->m_pParentObject = pParentObj;
	//m_sNodeMap[pNode] = obj;
	//m_sNodeList.push_back(obj);
	//if (pNode->GetMesh() != nullptr)
	//{
	//	ParseMesh(pNode, pNode->GetMesh(), obj);
	//}
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		/*if (pChildNode->GetNodeAttribute() != NULL)
		{
			FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
			if (AttributeType != FbxNodeAttribute::eMesh &&
				AttributeType != FbxNodeAttribute::eSkeleton&&
				AttributeType != FbxNodeAttribute::eNull)
			{
				continue;
			}
		}*/
		PreProcess(pChildNode);
	}
}
void SFbxObj::ParseMesh(FbxNode* pNode,
	FbxMesh*  pFbxMesh,
	SModelObj* pObj)
{


	std::vector<FbxLayerElementUV*> VertexUVSets;
	std::vector<FbxLayerElementMaterial*> pMaterialSetList;
	std::vector<FbxLayerElementVertexColor*> VertexColorSet;

	int iLayerCount = pFbxMesh->GetLayerCount();
	if (iLayerCount == 0 || pFbxMesh->GetLayer(0)->GetNormals() == nullptr)
	{
		pFbxMesh->InitNormals();
#if (FBXSDK_VERSION_MAJOR >= 2015)
		pFbxMesh->GenerateNormals();
#else
		pFbxMesh->ComputeVertexNormals();
#endif
	}

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
		if (pFbxMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
		{
			pMaterialSetList.push_back(pFbxMesh->GetLayer(iLayer)->GetMaterials());
		}
	}
	int iPolyCount = pFbxMesh->GetPolygonCount();
	int iVertexCount = pFbxMesh->GetControlPointsCount();

	// 트라이앵글 최대치 계산
	int iMaxTriangleCount = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		iMaxTriangleCount += iPolySize - 2;
	}

	int iNumMtrl = pNode->GetMaterialCount();
	if (iNumMtrl > 1)
	{
		pObj->subMesh.resize(iNumMtrl);
	}
	else
	{
		pObj->subMesh.resize(1);
		pObj->subMesh[0].m_VertexList.reserve(iMaxTriangleCount * 3);
	}

	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(iMtrl);
		if (pMtrl == nullptr)
		{
			continue;
		}
		pObj->fbxMaterialList.push_back(to_mw(ParseMaterial(pMtrl)));
		pObj->subMesh[iMtrl].m_VertexList.reserve(iMaxTriangleCount * 3);
	}


	// 정점 당 영향을 미치는 행렬 및 가중치 검색


	// transform
	FbxAMatrix geom;
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();


	Matrix matWorld;
	Matrix matLocal;
	Matrix matGeom;
	FbxAMatrix globalMatrix = pNode->EvaluateGlobalTransform();
	FbxAMatrix localMatrix = pNode->EvaluateLocalTransform();
	FbxVector4 Translation;
	if (pNode->LclTranslation.IsValid())
		Translation = pNode->LclTranslation.Get();

	FbxVector4 Rotation;
	if (pNode->LclRotation.IsValid())
		Rotation = pNode->LclRotation.Get();

	FbxVector4 Scale;
	if (pNode->LclScaling.IsValid())
		Scale = pNode->LclScaling.Get();

	FbxMatrix matTransform(Translation, Rotation, Scale);
	matGeom = DxConvertMatrix(ConvertMatrixA(geom));
	matLocal = DxConvertMatrix(ConvertMatrixA(matTransform));

	FbxNode* pFbxRootNode = m_pFBXScene->GetRootNode();
	if (pFbxRootNode->LclTranslation.IsValid())
		Translation = pFbxRootNode->LclTranslation.Get();
	if (pFbxRootNode->LclRotation.IsValid())
		Rotation = pFbxRootNode->LclRotation.Get();
	if (pFbxRootNode->LclScaling.IsValid())
		Scale = pFbxRootNode->LclScaling.Get();
	FbxMatrix matParentTransform(Translation, Rotation, Scale);
	Matrix matParent = DxConvertMatrix(ConvertMatrixA(matParentTransform.Inverse()));
	matWorld = matLocal * matParent;


	FbxVector4* pVertexPosiions = pFbxMesh->GetControlPoints();


	bool bSkinnedMesh = ParseMeshSkinningMap(pFbxMesh, pObj->WeightList, pObj);
	pObj->m_bSkinnedMesh = bSkinnedMesh;

	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
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
				auto finalPos = geom.MultT(pVertexPosiions[iCornerIndices[iIndex]]);

				v.p.x = finalPos.mData[0]; // x
				v.p.y = finalPos.mData[2]; // z
				v.p.z = finalPos.mData[1]; // y

				FbxColor color = FbxColor(1, 1, 1, 1);
				if (VertexColorSet.size())
				{
					color = ReadColor(pFbxMesh,
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
				finalPos = normalMatrix.MultT(normal);
				v.n.x = finalPos.mData[0]; // x
				v.n.y = finalPos.mData[2]; // z
				v.n.z = finalPos.mData[1]; // y
				if (VertexUVSets.size())
				{
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

				IW_VERTEX iw;
				if (pObj->m_bSkinnedMesh)
				{
					SWeight* pW = &pObj->WeightList[iCornerIndices[iIndex]];
					for (int i = 0; i < 4; i++)
					{
						iw.i1[i] = pW->Index[i];
						iw.w1[i] = pW->Weight[i];
					}
				}
				else
				{
					auto data = m_pFbxNodeMap.find(pNode);
					iw.i1[0] = data->second; // 자기 자신
					iw.w1[0] = 1.0f;
				}
				tri.vVertex[iIndex] = v;
				tri.vVertexIW[iIndex] = iw;
			}
			if (iNumMtrl > 1)
			{
				pObj->subMesh[iSubMtrl].SetUniqueBuffer(tri);
				//pObj->subMesh[iSubMtrl].m_TriangleList.push_back(tri);				
			}
			else
			{
				//pObj->subMesh[0].m_TriangleList.push_back(tri);
				pObj->subMesh[0].SetUniqueBuffer(tri);
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
	Matrix  matParent, SModelObj* pParentObj)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}
	//shared_ptr<TObject> obj = make_shared<TObject>();	
	SModelObj* obj = new SModelObj;
	obj->m_szName = to_mw(pNode->GetName());
	obj->m_pParentObject = pParentObj;
	m_sNodeMap[pNode] = obj;
	m_sNodeList.push_back(obj);
	Matrix matWorld = ParseTransform(pNode, matParent);
	// world matrix
	obj->m_matWorld = matWorld;
	if (pNode->GetMesh() != nullptr)
	{
		// vb, ib
		ParseMesh(pNode, pNode->GetMesh(), obj);
	}

	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		ParseNode(pChildNode, matWorld, obj);
	}
}


FbxVector4 SFbxObj::ReadNormal(const FbxMesh* mesh,
	int controlPointIndex,
	int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	// 노말 획득 
	FbxVector4 result;
	// 노말 벡터를 저장할 벡터 
	switch (vertexNormal->GetMappingMode()) 	// 매핑 모드 
	{
		// 제어점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// 정점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}
FbxColor SFbxObj::ReadColor(const FbxMesh* mesh,
	DWORD dwVertexColorCount,
	FbxLayerElementVertexColor* pVertexColorSet,
	DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
	{
		// Crack apart the FBX dereferencing system for Color coordinates		
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
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
			break;
		}
	}
	return Value;
}


bool SFbxObj::CreateInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	hr = g_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		m_pVSObj->GetBufferPointer(),
		m_pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	if (FAILED(hr)) return false;
	return true;
}