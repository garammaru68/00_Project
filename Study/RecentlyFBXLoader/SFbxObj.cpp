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
	PreProcess(pFbxRootNode);
	ParseNode(pFbxRootNode, Matrix::Identity);
	ParseAnimation(m_pFBXScene);
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
	SModelObject* pObj)
{


	std::vector<FbxLayerElementUV*> VertexUVSets;
	std::vector<FbxLayerElementMaterial*> pMaterialSetList;
	std::vector<FbxLayerElementVertexColor*> VertexColorSet;

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
		if (pFbxMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
		{
			pMaterialSetList.push_back(pFbxMesh->GetLayer(iLayer)->GetMaterials());
		}
	}

	//std::vector<std::string> fbxMaterialList;
	int iNumMtrl = pNode->GetMaterialCount();
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(iMtrl);
		if (pMtrl == nullptr)
		{
			continue;
		}
		pObj->fbxMaterialList.push_back(to_mw(ParseMaterial(pMtrl)));
	}
	if (iNumMtrl > 1)
	{
		pObj->subMesh.resize(iNumMtrl);
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

	pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(pNode->EvaluateGlobalTransform(1.0f)));
	//FbxAMatrix globalMatrix = pNode->EvaluateLocalTransform();
	//FbxAMatrix matrix = globalMatrix * geom;
	///pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(matrix));

	int iPolyCount = pFbxMesh->GetPolygonCount();
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	FbxVector4* pVertexPosiions = pFbxMesh->GetControlPoints();


	bool bSkinnedMesh = ParseMeshSkinningMap(pFbxMesh, pObj->WeightList);
	pObj->m_bSkinnedMesh = bSkinnedMesh;
	if (bSkinnedMesh == false)
	{
		auto data = m_pNodeMap.find(pNode);
		int  iBoneIndex = data->second;
		pObj->WeightList.resize(iVertexCount);
		for (int iv = 0; iv < iVertexCount; iv++)
		{
			pObj->WeightList[iv].Index.push_back(iBoneIndex);
			pObj->WeightList[iv].Weight.push_back(1.0f);
		}
	}

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

				IW_VERTEX8 iw;
				if (pObj->m_bSkinnedMesh)
				{
					SWeight* pW = &pObj->WeightList[iCornerIndices[iIndex]];
					if (pW != nullptr)
					{
						if (m_iMax < pW->Weight.size())
						{
							m_iMax = pW->Weight.size();
						}
					}

					for (int i = 0; i < pW->Index.size(); i++)
					{
						if (i < 4)
							iw.i1[i] = pW->Index[i];
						else
							iw.i2[i - 4] = pW->Index[i];

					}
					for (int i = 0; i < pW->Weight.size(); i++)
					{
						if (i < 4)
							iw.w1[i] = pW->Weight[i];
						else
							iw.w2[i - 4] = pW->Weight[i];
					}
				}
				else
				{
					iw.i1[0] = 0; // 자기 자신
					iw.w1[0] = 1.0f;
				}
				tri.vVertex[iIndex] = v;
				tri.vVertexIW8[iIndex] = iw;
			}
			if (iNumMtrl > 1)
			{
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
	//shared_ptr<SObject> obj = make_shared<SObject>();	
	SModelObject* obj = new SModelObject;
	obj->m_szName = to_mw(pNode->GetName());
	m_sMeshMap[pNode] = obj;
	m_sMeshList.push_back(obj);
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
		ParseNode(pChildNode, matWorld);
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
/*
/////////////////////////////////////////
*/
int SkinData::iNumMaxWeight = 0;
void SkinData::Alloc(size_t dwCount, DWORD dwStride)
{
	dwVertexCount = dwCount;
	dwVertexStride = dwStride;

	size_t dwBufferSize = dwVertexCount * dwVertexStride;
	pBoneIndices.reset(new int[dwBufferSize]);
	ZeroMemory(pBoneIndices.get(), sizeof(float) * dwBufferSize);

	pBoneWeights.reset(new float[dwBufferSize]);
	ZeroMemory(pBoneWeights.get(), sizeof(float) * dwBufferSize);
}


int* SkinData::GetIndices(size_t dwIndex)
{
	assert(dwIndex < dwVertexCount);
	return pBoneIndices.get() + (dwIndex * dwVertexStride);
}
float* SkinData::GetWeights(size_t dwIndex)
{
	assert(dwIndex < dwVertexCount);
	return pBoneWeights.get() + (dwIndex * dwVertexStride);
}

DWORD SkinData::GetBoneCount() const
{
	return static_cast<DWORD>(InfluenceNodes.size());
}

void SkinData::InsertWeight(size_t dwIndex, DWORD dwBoneIndex, float fBoneWeight)
{
	assert(dwBoneIndex < 256);

	auto pIndices = GetIndices(dwIndex);
	auto pWeights = GetWeights(dwIndex);

	for (DWORD i = 0; i < dwVertexStride; ++i)
	{
		if (fBoneWeight > pWeights[i])
		{
			for (DWORD j = (dwVertexStride - 1); j > i; --j)
			{
				pIndices[j] = pIndices[j - 1];
				pWeights[j] = pWeights[j - 1];
			}
			pIndices[i] = static_cast<int>(dwBoneIndex);
			pWeights[i] = fBoneWeight;
			break;
		}
		// 최대 본 인덱스를 얻는다.
		if (iNumMaxWeight < i)
		{
			iNumMaxWeight = i;
		}
	}
}