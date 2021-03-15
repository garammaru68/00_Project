#pragma once
#include <SStd.h>
#include <fbxsdk.h>
#include <map>
#include <unordered_map>
#include <SimpleMath.h>
#include <SObject.h>
using namespace DirectX::SimpleMath;
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
	void ParseNode(FbxNode* pNode, FbxScene* pFBXScene);
	void ParseAnimation(FbxScene* pFBXScene);
public:
	SFbxObject();
};

