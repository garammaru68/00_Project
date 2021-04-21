#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SModelObj.h"
#include "STimer.h"
using namespace DirectX::SimpleMath;
static Matrix DxConvertMatrix(Matrix m)
{
	Matrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;

	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}
static Matrix ConvertMatrixA(const FbxMatrix& matrix)
{
	Matrix matResult;
	auto fData = reinterpret_cast<float*>(&matResult);
	auto pSrcData = reinterpret_cast<const DOUBLE*>(&matrix);
	for (DWORD i = 0; i < 16; ++i)
	{
		fData[i] = (float)pSrcData[i];
	}
	return matResult;
}

typedef std::unordered_map<FbxNode*, SModelObj*>	sNodeMap;
typedef std::vector<SModelObj*>	sNodeList;
class SFbxObj : public SObject
{
public:
	int		  m_iMax = 0;
	SScene    m_Scene;
	//추가
	std::map<wstring, SScene> m_SceneMap;
	SScene*				m_pCurrentScene;

	float	  m_fTick = 0.0f;
	static FbxManager*  g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFBXScene;
	std::unordered_map<std::string, Matrix> m_dxMatrixBindPoseMap;
	std::unordered_map<FbxNode*, int> m_pFbxNodeMap;
	std::vector<FbxNode*> m_pFbxNodeList;
	std::vector<Matrix> m_pMatrixList;
	sNodeList m_sNodeList;
	sNodeMap  m_sNodeMap;

	ComPtr<ID3D11Buffer>     m_pBoneBuffer;
public:
	//추가
	void CuttingScene(std::wstring strSceneName, int iFirstFrame, int iLastFrame);
	bool ChangeScene(std::wstring strSceneName);
public:
	bool Load(std::string szFileName);
	bool LoadFBX(std::string szFileName);
	bool Initialize(std::string szFileName);
	void PreProcess(FbxNode* pNode);
	void ParseNode(FbxNode* pNode, Matrix  matParent, SModelObj* pParentObj);
	void ParseMesh(FbxNode* pNode, FbxMesh*  pMesh, SModelObj* pObj);
	Matrix ParseTransform(FbxNode* pNode, Matrix& matParentWorld);
	void ReadTextureCoord(FbxMesh* pFbxMesh,
		FbxLayerElementUV* pUVSet,
		int vertexIndex,
		int uvIndex,
		FbxVector2& uv);
	std::string ParseMaterial(FbxSurfaceMaterial* pMtrl);
	FbxVector4  ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	void ParseAnimation(FbxScene*	pFBXScene);
	void ParseAnimStack(FbxScene* pFbxScene, FbxString* strAnimStackName);
	void ParseNodeAnimation(FbxNode* pNode);
	bool ParseMeshSkinningMap(const FbxMesh* pFbxMesh, std::vector<SWeight>& skindata, SModelObj* pObj);

	void AddKey(FbxNode* pNode, FbxAMatrix mat, float fCurentTime);
	bool	CreateInputLayout() override;
public:
	SFbxObj();
};

