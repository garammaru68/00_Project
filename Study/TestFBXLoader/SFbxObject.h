#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SStd.h>
#include <fbxsdk.h>
#include <map>
#include <unordered_map>
#include <SimpleMath.h>
#include <SObject.h>
using namespace DirectX::SimpleMath;
typedef std::unordered_map<FbxNode*, SObject*>	sMeshMap;

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
	void ParseMesh(FbxNode* pNode, FbxMesh* pMesh, SObject* pObj);
	Matrix ParseTransform(FbxNode* pNode, Matrix& matParentWorld);
	void ReadTextureCoord(	FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
							int vertexIndex, int uvIndex, FbxVector2& uv);
	std::string ParseMaterial(FbxSurfaceMaterial* pMtrl);
public:
	SFbxObject();
};

