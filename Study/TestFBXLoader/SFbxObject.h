#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SStd.h>
#include <fbxsdk.h>
#include <map>
#include <unordered_map>
#include <SimpleMath.h>
#include <SObject.h>
using namespace DirectX::SimpleMath;
// 3DMAX의 Y-Up을 Z-Up으로 변환 ( Y축 Z축 변환 )
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
// 매트릭스 변환
static Matrix ConvertMatrixA(const FbxMatrix& matrix)
{
	Matrix matResult;
	auto fData = reinterpret_cast<float*>(&matResult);
	auto fSrcData = reinterpret_cast<const DOUBLE*>(&matrix);
	for (DWORD i = 0; i < 16; i++)
	{
		fData[i] = (float)fSrcData[i];
	}
	return matResult;
}

class ExportMtrlParameter
{
public:
	enum ExportMtrlParmeterFlags
	{
		EMPF_NONE = 0,
		EMPF_BUMPMP = 1,
		EMPF_DIFFUSEMAP = 2,
		EMPF_NORMALMAP = 4,
		EMPF_SPECULARMAP = 8,
		EMPF_LPHCHNNEL = 16,
		EMPF_AOMAP = 32,
	};
	ExportMtrlParameter()
	{
		ZeroMemory(this, sizeof(ExportMtrlParameter));
	}
};

class SModelObject : public SObject
{
public:
	std::vector<std::wstring> fbxMaterialList;
};
typedef std::unordered_map<FbxNode*, SObject*>	sMeshMap;

class SFbxObject
{
public:
	static FbxManager*	g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFBXScene;
	std::unordered_map<std::string, Matrix> m_dxMatrixMap;
	sMeshMap m_sMeshMap;
public:
	bool Load(std::string szFileName);
	bool LoadFBX(std::string szFileName);
	bool Initialize(std::string szFileName);
	void PreProcess(FbxNode* pNode);
	void ParseNode(FbxNode* pNode, Matrix matParent);
	void ParseMesh(FbxNode* pNode, FbxMesh* pMesh, SModelObject* pObj);
	Matrix ParseTransform(FbxNode* pNode, Matrix& matParentWorld);
	void ReadTextureCoord(	FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
							int vertexIndex, int uvIndex, FbxVector2& uv);
	std::string ParseMaterial(FbxSurfaceMaterial* pMtrl);
public:
	SFbxObject();
};

