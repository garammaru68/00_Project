#include "SFbxObject.h"
FbxManager* SFbxObject::g_pSDKManager = nullptr;
SFbxObject::SFbxObject()
{
	m_pFbxImporter	= nullptr;
	m_pFBXScene		= nullptr;
}
bool SFbxObject::Load(std::string szFileName)
{
	if (LoadFBX(szFileName))
	{
		return true;
	}
	return false;
}
bool SFbxObject::Initialize(std::string szFileName)
{
	if (g_pSDKManager == nullptr)
	{
		g_pSDKManager = FbxManager::Create();
		if (g_pSDKManager == nullptr)return false;
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

	bool bRet = m_pFbxImporter->Initialize(szFileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;
	bRet = m_pFbxImporter->Import(m_pFBXScene);
	return true;
}
bool SFbxObject::LoadFBX(std::string szFileName)
{
	if (Initialize(szFileName) == false)
	{
		return false;
	}
	// INode*
	FbxNode* pFbxRootNode = m_pFBXScene->GetRootNode();
	PreProcess(pFbxRootNode);
	ParseAnimation(m_pFBXScene);
	return true;
}
void SFbxObject::PreProcess(FbxNode* pNode)
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
	}
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		if (pChildNode->GetNodeAttribute() != NULL)
		{
			FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
		}
	}
}