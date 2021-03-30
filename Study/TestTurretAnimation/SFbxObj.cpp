#include "SFbxObj.h"

FbxManager* SFbxObj::g_pSDKManager = nullptr;
// Material(����) ����
std::string SFbxObj::ParseMaterial(FbxSurfaceMaterial* pMtrl)
{
	std::string name = pMtrl->GetName();
	// Material �Ӽ�(Ư��)
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
		// 0�� �ؽ��� ������ ���´�
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

			// tga���� ����X �ٲ��ֱ�
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

	switch (pFbxLayerElementUV->GetMappingMode()) // mapping ���
	{
	case FbxLayerElementUV::eByControlPoint: // control point�� �븻�� control point�� �ε����� ���� ( �ϳ��� �ϳ��� �븻 )
	{
		switch (pFbxLayerElementUV->GetReferenceMode()) // mapping ������ ����Ǵ� ���
		{
		// control point�� �ε��� �Ǵ� �� ���� ������ �ε����� ���� normal�� ����
		case FbxLayerElementUV::eDirect:
		{
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		// �� ���� ������ �ε����� ���� normal ���͸� ����ų �� �ִ� �ε����� �ο��ް�, 
		// �̰��� ���� ���� normal�� ����
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
	case FbxLayerElementUV::eByPolygonVertex: // ���ؽ��� �븻�� ���ؽ��� �ε����� ���� ( �ϳ��� edge�� ���̴� ������ ����ŭ �븻 )
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
	// FBX ���� ���� �ҷ�����
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
		/// �ν��Ͻ�ȭ
		// FBX SDK ������ ��ü ����
		g_pSDKManager = FbxManager::Create();
		if (g_pSDKManager == nullptr) return false;
		// FBX IO ��ü ���� �� ����
		FbxIOSettings* ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
		if (ios == nullptr) return false;
		// ���� ������ �������ų� ������ (camera, light, mesh, texture, material, animation, ����� ���� �Ӽ� ��)
		g_pSDKManager->SetIOSettings(ios);
	}
	if (m_pFbxImporter == nullptr)
	{
		// FbxImporter ��ü ����
		m_pFbxImporter = FbxImporter::Create(g_pSDKManager, "");
		if (m_pFbxImporter == nullptr) return false;
	}
	if (m_pFBXScene == nullptr)
	{
		m_pFBXScene = FbxScene::Create(g_pSDKManager, "");
		if (m_pFBXScene == nullptr) return false;
	}

	// Importer�� FBX ���� ������ �ʱ�ȭ
	bool bRet = m_pFbxImporter->Initialize(szFileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;
	// FBX ���� ������ Scene���� ��������
	bRet = m_pFbxImporter->Import(m_pFBXScene);

	//Import ���� ���� // Z�� ��ȯ
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
	// Ʈ�������� �Ǿ� �ִ� Scene�� ������ �������� ����
	FbxNode* pFbxRootNode = m_pFBXScene->GetRootNode();
	ParseNode(pFbxRootNode, Matrix::Identity);
	ParseAnimation(m_pFBXScene);
	return true;
}
void SFbxObj::PreProcess(FbxNode* pNode)
{
	// ī�޶�� �� ���� ����
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
	// ��� Child(����)�� ��ȸ�ϸ� ���ϴ� ���� ��� (��� Ž��)
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		// ���(����) Ÿ�� ���ϱ�
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

	int iLayerCount = pFbxMesh->GetLayerCount(); // ��� Layer�� �ҷ��´� = 1
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer); // iLayer ��´�
		// ���ؽ� �÷�
		if (pLayer->GetVertexColors() != NULL)
		{
			VertexColorSet.push_back(pLayer->GetVertexColors());
		}
		// UV
		if (pLayer->GetUVs() != NULL)
		{
			VertexUVSets.push_back(pLayer->GetUVs()); // pLayer���� UV������ �����ͼ� push_back
		}
		if (pFbxMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
		{
			pMaterialSetList.push_back(pFbxMesh->GetLayer(iLayer)->GetMaterials()); // ���̾ Material�� push_back
		}
	}

	//std::vector<std::string> fbxMaterialList;
	int iNumMtrl = pNode->GetMaterialCount(); // ��� ���� ������ �����´�
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(iMtrl); // iMtrl(����) ���
		if (pMtrl == nullptr)
		{
			continue;
		}
		pObj->fbxMaterialList.push_back(to_mw(ParseMaterial(pMtrl))); // �ؽ��� + ���� push_back
	}

	// transform
	FbxAMatrix geom;
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	// �븻 ��ȯ Nomal * (World Matrix^(-1))^T // World ��� �̹� �� ����
	FbxAMatrix normalMat = geom;
	normalMat = normalMat.Inverse();
	normalMat = normalMat.Transpose();

	//pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(pNode->EvaluateGlobalTransform(1.0f)));
	FbxAMatrix globalMatrix = pNode->EvaluateLocalTransform();
	FbxAMatrix matrix = globalMatrix * geom;
	pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(matrix));

	int iPolyCount = pFbxMesh->GetPolygonCount();
	int iVertexCount = pFbxMesh->GetControlPointsCount(); // ���� ����
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints(); // ���� ��ġ

	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly); // Polygon ���� ���� �����´�
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
	Matrix matWorld = ParseTransform(pNode, matParent); // ������� ����(����ȭ ����)
	obj->m_matWorld = matWorld;
	if (pNode->GetMesh() != nullptr)
	{
		ParseMesh(pNode, pNode->GetMesh(), obj); // vb, ib
	}
	// ��� Child(����)�� ��ȸ�ϸ� ���ϴ� ���� ��� (��� Ž��)
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
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);	// �ִϸ��̼� �̸��� �迭�� ä���
	int iAnimStackCount = AnimStackNameArray.GetCount();	// �ִϸ��̼� ����
	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i));
	}
}
// ���� �븻�� �д´�
FbxVector4 SFbxObj::ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0); // �븻 ���

	FbxVector4 result; // �븻 ���͸� ������ ����

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