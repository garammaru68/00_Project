#include "SFbxObj.h"
void SFbxObj::ParseAnimation(FbxScene* pFbxScene)
{
	FbxArray<FbxString*> AnimStackNameArray;
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);	// �ִϸ��̼� ���� �̸��� �迭(string)�� ä���
	int iAnimStackCount = AnimStackNameArray.GetCount();	// �ִϸ��̼� ����(Data->Size)
	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i)); // i(�ε���) ��ġ�� ������(Name)�� �ҷ��´�
	}
}
// Animation Frame ����
void SFbxObj::ParseAnimStack(FbxScene* pFbxScene,
	FbxString* strAnimStackName)
{
	// Scene�� �ִ� AnimationStackName ã�� ( ���� Ȯ�� )
	FbxAnimStack* anim = pFbxScene->FindMember<FbxAnimStack>(strAnimStackName->Buffer());
	if (anim == nullptr) return;
	// FBX Animation Stack ������ ���´�
	FbxTakeInfo* info = pFbxScene->GetTakeInfo(*strAnimStackName);

	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble(); // 1�����Ӵ� �ð�
	float fStartTime, fEndTime;
	if (info)
	{
		// scene
		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble(); // ���� �ð�
		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble();	// �� �ð�
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan); // �۷ι� Default �� ����
		fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
	m_Scene.iFirstFrame = fStartTime * 30.0f;
	m_Scene.iLastFrame = fEndTime * 30.0f;
	m_Scene.iFrameSpeed = 30;
	m_Scene.iTickPerFrame = 160;
	m_Scene.iDeltaTick = 1;
	m_Scene.fDeltaTime = fFrameTime * 1.0f; // 1������ ����
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
	// ���� Ȯ��
#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
	auto anim = m_pFBXScene->GetAnimationEvaluator(); // ����(��ġ) ��������
#else
	auto anim = m_pFBXScene->GetEvaluator();
#endif
	float fCurrentTime = 0.0f;
	while (fCurrentTime <= m_Scene.fLastTime)
	{
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t); // ����ð�(fCurrentTime)�� ��ȯ ����� ��ȯ�Ѵ�
		SAnimTrack track;
		track.iTick = fCurrentTime * 30 * 160;
		track.mat = DxConvertMatrix(ConvertMatrixA(mat)); // y�� Z�� ��ȯ


		auto data = m_sMeshMap.find(pNode);
		data->second->animlist.push_back(track); // ��ȯ ��İ� �ð�(ƽ) �ֱ�
		fCurrentTime += m_Scene.fDeltaTime;
		//FbxAMatrix self;
	}
	// ��� ���鼭 ���� ã��
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		ParseNodeAnimation(pChildNode); // ��� ȣ��
	}
}