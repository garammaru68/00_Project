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
	if (pNode && (pNode->GetCamera()) || pNode->GetLight())
	{
		return;
	}
	if (pNode->GetMesh() == nullptr)
	{
		return;
	}
#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR == 2014) && (FBXDK_VERSION_MINOR > 1)))
	auto anim = m_pFBXScene->GetAnimationEvaluator();
#else
	auto anim = m_pFBXScene->GetEvalutor();
#endif
	float fCurrentTime = 0.0f;
	while (fCurrentTime <= m_Scene.fLastTime)
	{
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t);
		SAnimTrack track;
		track.iTick = fCurrentTime * 30 * 160;
		FbxAMatrix self;
		// mat	1�� �θ���� ����� ���Ѵ�.
		// self 2�� ��� ����( S, R, T )
		// 3�� ���ϸ��̼� Ʈ�� ���
		// S(����), R(���ʹϾ�), T(����)�� ���ø� �ð� �������� ����
	}
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		ParseNodeAnimation(pChildNode);
	}
}
void SFbxObj::ParseAnimStack(FbxScene* pFbxScene, FbxString* strAnimStackName)
{
	// �ִϸ��̼� ���� ����
	FbxAnimStack* anim = pFbxScene->FindMember<FbxAnimStack>(strAnimStackName->Buffer());
	if (anim == nullptr) return;
	// �ִϸ��̼� ���� ��������
	FbxTakeInfo* info = pFbxScene->GetTakeInfo(*strAnimStackName);

	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble(); // 1 ������ �ð� ���
	float fSampleTime = fFrameTime * 1.0f;// 1������
	float fSourceSamplingInterval = fSampleTime; // �ִϸ��̼� ����
	// tick = 1Sec* FrameSpeed * FramePerTick
	// 4800 = 1Sec*30* 160;
	// 0.033 = 1Sec / 30;
	float fStartTime, fEndTime;

	if (info)
	{
		// info�� �ִ� ������ �ִϸ��̼� ���� ������ �� ������ ��´�
		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble(); // �ִϸ��̼� ���� ����
		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble(); // �ִϸ��̼� �� ����
	}
	else
	{
		// �⺻ �ð� ���� ��´�
		FbxTimeSpan slTimeSpan;
		pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(slTimeSpan);

		fStartTime = (float)slTimeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)slTimeSpan.GetEnd().GetSecondDouble();
	}

	float SCENE_FRAMESPEED = 30;
	float SCENE_TICKSPERFRAME = 160;
	int iFristFrame = fStartTime * 30.0f;
	int iLastFrame = fEndTime * 30.0f;

	ParseNodeAnimation(pFbxScene->GetRootNode());
}
