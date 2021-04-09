#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <sstd.h>
#include <fbxsdk.h>
#include <map>
#include <unordered_map>
#include <simpleMath.h>
#include <SObject.h>
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
struct SWeight
{
	std::vector<int>   Index;
	std::vector<float> Weight;
	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < Index.size(); ++i)
		{
			if (fBoneWeight > Weight[i])
			{
				for (DWORD j = (Index.size() - 1); j > i; --j)
				{
					Index[j] = Index[j - 1];
					Weight[j] = Weight[j - 1];
				}
				Index[i] = iBoneIndex;
				Weight[i] = fBoneWeight;
				break;
			}
		}
	}
	SWeight()
	{
		Index.resize(8);
		Weight.resize(8);
	}
};
struct SSubMesh
{
	std::vector<S_TRIANGLE>			m_TriangleList;
	std::vector<PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>	    m_pVertexBuffer;
	STexture*				    m_pTexture;
	int iCount = 0;
	~SSubMesh()
	{
		//m_pTexture = nullptr;
	}
};
struct SAnimTrack
{
	int   iTick;
	Matrix mat;
};
struct SScene
{
	int iFirstFrame;
	int iLastFrame;
	int iFrameSpeed; // 30
	int iTickPerFrame;// 160
	int iNumMesh;
	int iDeltaTick; // 1frame
	float fDeltaTime;
	float fFirstTime;
	float fLastTime;
};
class SModelObject : public SObject
{
public:

	std::vector<std::wstring>  fbxMaterialList;
	std::vector<SSubMesh>		subMesh;
	std::vector<SAnimTrack>     animlist;
	std::vector<SWeight>		WeightList;
	bool						m_bSkinnedMesh = false;
	SModelObject()
	{
		//pParent = nullptr;
	}
	virtual ~SModelObject()
	{
		//subMesh.clear();
	}
};

class SkinData
{
public:
	static int				iNumMaxWeight;
	std::vector<FbxNode*>	InfluenceNodes;
	std::unordered_map<T_STR, Matrix> m_matBindPoseMap;

	size_t					dwVertexCount;
	DWORD					dwVertexStride;
	std::unique_ptr<int[]>	pBoneIndices;
	std::unique_ptr<float[]> pBoneWeights;
public:
	void	Alloc(size_t dwCount, DWORD dwStride);
	int*	GetIndices(size_t dwIndex);
	float*	GetWeights(size_t dwIndex);
	DWORD	GetBoneCount() const;
	void	InsertWeight(size_t dwIndex, DWORD dwBoneIndex, float fBoneWeight);
public:
	SkinData() : dwVertexCount(0), dwVertexStride(0) {}
	~SkinData() {}
};

typedef std::unordered_map<FbxNode*, SModelObject*>	sMeshMap;
typedef std::vector<SModelObject*>	sMeshList;
//typedef std::vector<AnimationScanNode>			ScanList;
class SFbxObj
{
public:
	int				m_iMax = 0;
	SScene    m_Scene;
	float	  m_fTick = 0.0f;
	static FbxManager*  g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFBXScene;
	std::unordered_map<std::string, Matrix> m_dxMatrixMap;
	std::unordered_map<FbxNode*, int> m_pNodeMap;
	std::vector<Matrix> m_pMatrixList;
	sMeshList m_sMeshList;
	sMeshMap m_sMeshMap;
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
	FbxVector4  ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	void ParseAnimation(FbxScene*	pFBXScene);
	void ParseAnimStack(FbxScene* pFbxScene, FbxString* strAnimStackName);
	void ParseNodeAnimation(FbxNode* pNode);

	bool ParseMeshSkinning(const FbxMesh* pFbxMesh, SkinData* skindata);
	bool ParseMeshSkinningMap(const FbxMesh* pFbxMesh, std::vector<SWeight>& skindata);
public:
	SFbxObj();
};

