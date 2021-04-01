#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SModelObj.h"

using namespace DirectX::SimpleMath;
static Matrix DxConvertMatrix(Matrix m)
{
	Matrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;

	mat._11 = mat._24 = mat._34 = 0.0f;
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

typedef std::unordered_map<FbxNode*, SModelObj*> sNodeMap;
typedef std::vector<SModelObj*> sNodeList;
class SFbxObj
{
public:
	static FbxManager*	g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFbxScene;
	std::unordered_map<std::string, Matrix>	m_dxMatrixMap;
	std::unordered_map<FbxNode*, int>		m_pNodeMap;
	std::vector<Matrix>						m_pMatrixList;
	sNodeMap	m_sNodeMap;
	sNodeList	m_sNodeList;
public:
	bool Load(std::string szFileName);
	bool LoadFBX(std::string szFileName);
	bool Initialize(std::string szFileName);
	void PreProcess(FbxNode* pNode);
	void		ParseNode(FbxNode* pNode, Matrix matParent);
	Matrix		ParseTransform(FbxNode* pNode, Matrix& matParentWorld);
	void		ParseMesh(FbxNode* pNode, FbxMesh* pFbxMesh, SModelObj* pObj);
	std::string ParseMaterial(FbxSurfaceMaterial* pMtrl);
	bool		ParseMeshSkinningMap(const FbxMesh* pFbxMesh, std::vector<SWeight>* skindata);
public:
	SFbxObj();
};

