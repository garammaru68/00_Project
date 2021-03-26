#include "SFbxObj.h"
void SFbxObj::ParseAnimation(FbxScene* pFbxScene)
{
	FbxArray<FbxString*> AnimStackNameArray;
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);	// 애니메이션 이름을 배열에 채운다
	int iAnimStackCount = AnimStackNameArray.GetCount();	// 애니메이션 갯수
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
		// mat	1차 부모행렬 역행렬 곱한다.
		// self 2차 행렬 분해( S, R, T )
		// 3차 에니메이션 트랙 행렬
		// S(벡터), R(쿼터니언), T(벡터)를 샘플링 시간 간격으로 저장
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
	// 애니메이션 스택 정보
	FbxAnimStack* anim = pFbxScene->FindMember<FbxAnimStack>(strAnimStackName->Buffer());
	if (anim == nullptr) return;
	// 애니메이션 정보 가져오기
	FbxTakeInfo* info = pFbxScene->GetTakeInfo(*strAnimStackName);

	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble(); // 1 프레임 시간 계산
	float fSampleTime = fFrameTime * 1.0f;// 1프레임
	float fSourceSamplingInterval = fSampleTime; // 애니메이션 간격
	// tick = 1Sec* FrameSpeed * FramePerTick
	// 4800 = 1Sec*30* 160;
	// 0.033 = 1Sec / 30;
	float fStartTime, fEndTime;

	if (info)
	{
		// info에 있는 정보로 애니메이션 시작 구관과 끝 구간을 얻는다
		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble(); // 애니메이션 시작 구간
		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble(); // 애니메이션 끝 구간
	}
	else
	{
		// 기본 시간 범위 얻는다
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
