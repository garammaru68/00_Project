#include "SFbxObj.h"

bool SFbxObj::Load(std::string szFileName)
{
	if (LoadFBX(szFileName))
	{
		return true;
	}
	return false;
}
bool SFbxObj::LoadFBX(std::string szFileName)
{
	if (Initialize(szFileName) == false)
	{
		return false;
	}
	FbxNode* pFbxRootNode = m_pFbxScene->GetRootNode();
	PreProcess(pFbxRootNode);
}
FbxManager* SFbxObj::g_pSDKManager = nullptr;
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
	if (m_pFbxScene == nullptr)
	{
		m_pFbxScene = FbxScene::Create(g_pSDKManager, "");
		if (m_pFbxScene == nullptr) return false;
	}

	bool bRet = m_pFbxImporter->Initialize(szFileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	// ZÁÂÇ¥ ¹Ù²Ù±â
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);
	FbxAxisSystem SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();

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
		//if (pChildNode->GetNodeAttribute() != NULL)
		//{
		//	FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
		//	if (AttributeType != FbxNodeAttribute::eMesh &&
		//		AttributeType != FbxNodeAttribute::eSkeleton&&
		//		AttributeType != FbxNodeAttribute::eNull)
		//	{
		//		continue;
		//	}
		//}
		PreProcess(pChildNode);
	}
}
void SFbxObj::ParseNode(FbxNode* pNode, Matrix matParent)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera()) || (pNode->GetLight()))
	{
		return;
	}
	SModelObj* obj = new SModelObj;
	obj->m_szName = to_mw(pNode->GetName());
	m_sNodeMap[pNode] = obj;
	m_sNodeList.push_back(obj);

}