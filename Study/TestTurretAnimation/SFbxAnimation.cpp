#include "SFbxObj.h"
void SFbxObj::ParseAnimation(FbxScene* pFbxScene)
{
	FbxArray<FbxString*> AnimStackNameArray;
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);	// 애니메이션 스택 이름을 배열(string)에 채운다
	int iAnimStackCount = AnimStackNameArray.GetCount();	// 애니메이션 갯수(Data->Size)
	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i)); // i(인덱스) 위치의 데이터(Name)를 불러온다
	}
}
// Animation Frame 셋팅
void SFbxObj::ParseAnimStack(FbxScene* pFbxScene,
	FbxString* strAnimStackName)
{
	// Scene에 있는 AnimationStackName 찾기 ( 유무 확인 )
	FbxAnimStack* anim = pFbxScene->FindMember<FbxAnimStack>(strAnimStackName->Buffer());
	if (anim == nullptr) return;
	// FBX Animation Stack 정보를 얻어온다
	FbxTakeInfo* info = pFbxScene->GetTakeInfo(*strAnimStackName);

	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble(); // 1프레임당 시간
	float fStartTime, fEndTime;
	if (info)
	{
		// scene
		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble(); // 시작 시간
		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble();	// 끝 시간
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan); // 글로벌 Default 값 셋팅
		fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
	m_Scene.iFirstFrame = fStartTime * 30.0f;
	m_Scene.iLastFrame = fEndTime * 30.0f;
	m_Scene.iFrameSpeed = 30;
	m_Scene.iTickPerFrame = 160;
	m_Scene.iDeltaTick = 1;
	m_Scene.fDeltaTime = fFrameTime * 1.0f; // 1프레임 간격
	m_Scene.fFirstTime = fStartTime;
	m_Scene.fLastTime = fEndTime;
	ParseNodeAnimation(pFbxScene->GetRootNode());
}
void SFbxObj::ParseNodeAnimation(FbxNode* pNode)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}
	//if (pNode->GetMesh() == nullptr)
	//{
	//	return;
	//}
	// 버전 확인
#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
	auto anim = m_pFBXScene->GetAnimationEvaluator(); // 도구(장치) 가져오기
#else
	auto anim = m_pFBXScene->GetEvaluator();
#endif
	float fCurrentTime = 0.0f;
	while (fCurrentTime <= m_Scene.fLastTime)
	{
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t); // 현재시간(fCurrentTime)에 변환 행렬을 반환한다
		SAnimTrack track;
		track.iTick = fCurrentTime * 30 * 160;
		track.mat = DxConvertMatrix(ConvertMatrixA(mat)); // y축 Z축 변환


		auto data = m_sMeshMap.find(pNode);
		data->second->animlist.push_back(track); // 변환 행렬과 시간(틱) 넣기
		fCurrentTime += m_Scene.fDeltaTime;
		//FbxAMatrix self;
	}
	// 노드 돌면서 정보 찾기
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		ParseNodeAnimation(pChildNode); // 재귀 호출
	}
}