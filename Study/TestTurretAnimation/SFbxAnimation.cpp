#include "SFbxObj.h"
//void SFbxObj::ParseAnimation(FbxScene* pFbxScene)
//{
//	FbxArray<FbxString*> AnimStackNameArray;
//	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);	// 애니메이션 이름을 배열에 채운다
//	int iAnimStackCount = AnimStackNameArray.GetCount();	// 애니메이션 갯수
//	for (int i = 0; i < iAnimStackCount; i++)
//	{
//		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i));
//	}
//}
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
		FbxAMatrix self;
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
	float fSoureSamplingInterval = fSampleTime;
	// tick = 1Sec* FrameSpeed * FramePerTick
	// 4800 = 1Sec*30* 160;
	// 0.033 = 1Sec / 30;
	float fStartTime, fEndTime;
	if (info)
	{
		// scene
		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble();
		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);
		fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
	float SCENE_FRAMESPEED = 30;
	float SCENE_TICKSPERFRAME = 160;
	int iFirstFrame = fStartTime * 30.0f;
	int iLastFrame = fEndTime * 30.0f;
	ParseNodeAnimation(pFbxScene->GetRootNode());
}