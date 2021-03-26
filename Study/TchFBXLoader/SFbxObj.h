#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <sstd.h>
#include <fbxsdk.h>
#include <map>
#include <unordered_map>
#include <simpleMath.h>
#include <SObject.h>
using namespace DirectX::SimpleMath;

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
struct SSubMesh
{
	std::vector<S_TRIANGLE>		m_TriangleList;
	std::vector<PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>	    m_pVertexBuffer;
	STexture*				    m_pTexture;
	int iCount = 0;
	~SSubMesh()
	{

	}
};
struct SAnimTrack
{
	int iTick;
	Matrix mat;
};
struct SScene
{
	int iFirstFrame;
	int iLastFrame;
	int iframeSpeed;
	int iTickPerFrame;
	int iNumMesh;
	int iDeltaTick;
	float fDeltaTime;
	float fFirstTime;
	float fLastTime;
};
class SModelObject : public SObject
{
public:
	std::vector<std::wstring>	fbxMaterialList;
	std::vector<SSubMesh>		subMesh;
	std::vector<SAnimTrack>		animlist;
	SModelObject* pParent;
	SModelObject()
	{
		// pParent = nullptr;
	}
	virtual ~SModelObject()
	{
	}
};
typedef std::unordered_map<FbxNode*, SModelObject*>	sMeshMap;
typedef std::vector<SModelObject*> sMeshList;
class SFbxObj
{
public:
	SScene		m_Scene;
	float		m_fTick = 0.0f;
	static FbxManager*  g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFBXScene;
	std::unordered_map<std::string, Matrix> m_dxMatrixMap;
	sMeshMap	m_sMeshMap;
	sMeshList	m_sMeshList;
public:
	bool Load(std::string szFileName);
	bool LoadFBX(std::string szFileName);
	bool Initialize(std::string szFileName);
	void PreProcess(FbxNode* pNode);
	void ParseNode(FbxNode* pNode, Matrix  matParent);
	void ParseMesh(FbxNode* pNode, FbxMesh*  pMesh, SModelObject* pObj);
	Matrix ParseTransform(FbxNode* pNode, Matrix& matParentWorld);
	void ReadTextureCoord(FbxMesh* pFbxMesh,
		FbxLayerElementUV* pUVSet,
		int vertexIndex,
		int uvIndex,
		FbxVector2& uv);
	std::string ParseMaterial(FbxSurfaceMaterial* pMtrl);
	FbxVector4	ReadNormal(const FbxMesh* mesh, int controlPoinIndex, int vertexCounter);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount,
						  FbxLayerElementVertexColor* pVertexColorSet,
						  DWORD dwDCCIndex, DWORD dwVertexIndex);
	void ParseAnimation(FbxScene* pFbxScene);
	void ParseAnimStack(FbxScene* pFbxScene, FbxString* strAnimStackName);
	void ParseNodeAnimation(FbxNode* pNode);
public:
	SFbxObj();
};