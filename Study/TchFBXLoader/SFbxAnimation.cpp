//#include "SFbxObj.h"
//void SFbxObj::ParseAnimation(FbxScene* pFbxScene)
//{
//	FbxArray<FbxString*> AnimStackNameArray;
//	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);	// �ִϸ��̼� �̸��� �迭�� ä���
//	int iAnimStackCount = AnimStackNameArray.GetCount();	// �ִϸ��̼� ����
//	for (int i = 0; i < iAnimStackCount; i++)
//	{
//		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i));
//	}
//}
//void SFbxObj::ParseNodeAnimation(FbxNode* pNode)
//{
//	if (pNode == nullptr) return;
//	if (pNode && (pNode->GetCamera()) || pNode->GetLight())
//	{
//		return;
//	}
//	if (pNode->GetMesh() == nullptr)
//	{
//		return;
//	}
//#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR == 2014) && (FBXDK_VERSION_MINOR > 1)))
//	auto anim = m_pFBXScene->GetAnimationEvaluator();
//#else
//	auto anim = m_pFBXScene->GetEvalutor();
//#endif
//	float fCurrentTime = 0.0f;
//	while (fCurrentTime <= m_Scene.fLastTime)
//	{
//		FbxTime t;
//		t.SetSecondDouble(fCurrentTime);
//		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t);
//		SAnimTrack track;
//		track.iTick = fCurrentTime * 30 * 160;
//		track.mat = DxConvertMatrix(ConvertMatrixA(mat));	
//
//		auto data = m_sMeshMap.find(pNode);
//		data->second->animlist.push_back(track);
//		fCurrentTime += m_Scene.fDeltaTime;
//
//		FbxAMatrix self;
//		// mat	1�� �θ���� ����� ���Ѵ�.
//		// self 2�� ��� ����( S, R, T )
//		// 3�� ���ϸ��̼� Ʈ�� ���
//		// S(����), R(���ʹϾ�), T(����)�� ���ø� �ð� �������� ����
//	}
//	int dwChild = pNode->GetChildCount();
//	for (int dwObj = 0; dwObj < dwChild; dwObj++)
//	{
//		FbxNode* pChildNode = pNode->GetChild(dwObj);
//		ParseNodeAnimation(pChildNode);
//	}
//}
//void SFbxObj::ParseAnimStack(FbxScene* pFbxScene, FbxString* strAnimStackName)
//{
//	// �ִϸ��̼� ���� ����
//	FbxAnimStack* anim = pFbxScene->FindMember<FbxAnimStack>(strAnimStackName->Buffer());
//	if (anim == nullptr) return;
//	// �ִϸ��̼� ���� ��������
//	FbxTakeInfo* info = pFbxScene->GetTakeInfo(*strAnimStackName);
//
//	FbxTime FrameTime;
//	FrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());
//	float fFrameTime = FrameTime.GetSecondDouble(); // 1 ������ �ð� ���
//	// tick = 1Sec * FrameSpeed * FramePerTick
//	// 4800 = 1Sec * 30	* 160;
//	// 0.033 = 1Sec / 30;
//	float fStartTime, fEndTime;
//
//	if (info)
//	{
//		// info�� �ִ� ������ �ִϸ��̼� ���� ������ �� ������ ��´�
//		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble(); // �ִϸ��̼� ���� ����
//		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble(); // �ִϸ��̼� �� ����
//	}
//	else
//	{
//		// �⺻ �ð� ���� ��´�
//		FbxTimeSpan slTimeSpan;
//		pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(slTimeSpan);
//
//		fStartTime = (float)slTimeSpan.GetStart().GetSecondDouble();
//		fEndTime = (float)slTimeSpan.GetStop().GetSecondDouble();
//	}
//
//	m_Scene.iFirstFrame = fStartTime * 30.0f;
//	m_Scene.iLastFrame = fEndTime * 30.0f;
//	m_Scene.iFrameSpeed = 30;
//	m_Scene.iTickPerFrame = 160;
//	m_Scene.iDeltaTick = 1;
//	m_Scene.fDeltaTime = fFrameTime * 1.0f; // 1 ������ ����
//	m_Scene.fFirstTime = fStartTime;
//	m_Scene.fLastTime = fEndTime;
//	ParseNodeAnimation(pFbxScene->GetRootNode());
//}
#include "SFbxObj.h"
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
void SFbxObj::ParseNodeAnimation(FbxNode* pNode)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}
	FbxAnimEvaluator* anim = m_pFBXScene->GetAnimationEvaluator();
	float fCurrentTime = 0.0f;
	float fEndTime = 1.666f;
	float fDeltaTime = 0.0333f;
	while (fCurrentTime <= fEndTime)
	{
		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, fCurrentTime);
		fCurrentTime += fDeltaTime;
		// mat 1?? ?��???? ????? ?????.
		FbxAMatrix self;
		// self 2?? ??? ????( S, R, T )
		// 3?? ???????? ??? ???
		// S(????), R(??????), T(????)?? ???��? ?��? ???????? ????
	}
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		ParseNodeAnimation(pChildNode);
	}
}
void SFbxObj::ParseAnimStack(FbxScene* pFbxScene,
	FbxString* strAnimStackName)
{
	FbxAnimStack* anim = pFbxScene->FindMember<FbxAnimStack>(strAnimStackName->Buffer());
	if (anim == nullptr) return;
	FbxTakeInfo* info = pFbxScene->GetTakeInfo(*strAnimStackName);

	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fSampleTime = fFrameTime * 1.0f;
}
// ������(�ִϸ��̼� ��� ���) ���� ������, �����ӿ��� ����� �ε��� ����� �迭�� ����, ���̴��� ����