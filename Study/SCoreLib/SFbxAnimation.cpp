#include "SFbxObj.h"
void SFbxObj::AddKey(FbxNode* pNode, FbxAMatrix matGlobal,
	float fCurrentTime)
{
	auto anim = m_pFBXScene->GetAnimationEvaluator();
	auto data = m_sNodeMap.find(pNode);
	// matGlobal = 자식 * 부모
	Matrix pChildGlobal = DxConvertMatrix(
		ConvertMatrixA(matGlobal));
	FbxNode* pParentNode = pNode->GetParent();

	Matrix matInvParentGlobal = Matrix::Identity;
	if (pParentNode)
	{
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix matFbxParent =
			anim->GetNodeGlobalTransform(pParentNode, t);
		matInvParentGlobal = DxConvertMatrix(
			ConvertMatrixA(matFbxParent));
		matInvParentGlobal = matInvParentGlobal.Invert();
	}
	Matrix m = pChildGlobal * matInvParentGlobal;

	// 분해
	Vector3 scale;
	Quaternion qRot;
	Vector3 pos;
	if (m.Decompose(scale, qRot, pos))
	{
		SAnimTrack track;
		track.iTick = fCurrentTime * 30 * 160;
		track.s = scale;
		track.q = qRot;
		track.p = pos;
		track.mat = pChildGlobal;
		data->second->animlist.push_back(track);
	}
	else
	{
		int k = 0;
	}
}
bool SFbxObj::ParseMeshSkinningMap(const FbxMesh* pFbxMesh,
	std::vector<SWeight>& skindata, SModelObj* pObj)
{
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	skindata.resize(iVertexCount);
	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		DWORD dwClusterCount = pSkin->GetClusterCount();
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			////
			FbxAMatrix matXBindPose;
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

			Matrix matInvBindPos = DxConvertMatrix(ConvertMatrixA(matBindPose));
			matInvBindPos = matInvBindPos.Invert();
			std::string name = pCluster->GetLink()->GetName();
			m_dxMatrixBindPoseMap.insert(make_pair(name, matInvBindPos));
			pObj->m_dxMatrixBindPoseMap.insert(make_pair(name, matInvBindPos));

			int  dwClusterSize = pCluster->GetControlPointIndicesCount();
			auto data = m_pFbxNodeMap.find(pCluster->GetLink());
			int  iBoneIndex = data->second;
			// 영향을 받는 정점들의 인덱스
			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			for (int i = 0; i < dwClusterSize; i++)
			{
				skindata[pIndices[i]].InsertWeight(iBoneIndex, pWeights[i]);
			}
		}
	}
	return true;
}
void SFbxObj::ParseAnimation(FbxScene*	pFbxScene)
{
	FbxArray<FbxString*>  AnimStackNameArray;
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
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
	//if (pNode->GetMesh() != nullptr)
	//{
	//	return;
	//}
#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
	auto anim = m_pFBXScene->GetAnimationEvaluator();
#else
	auto anim = m_pFBXScene->GetEvaluator();
#endif
	CStopwatch stopwatch;
	float fCurrentTime = 0.0f;
	auto data = m_sNodeMap.find(pNode);
	while (fCurrentTime <= m_Scene.fLastTime)
	{
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t);
		AddKey(pNode, mat, fCurrentTime);
		//SAnimTrack track;
		//track.iTick = fCurrentTime * 30* 160;
		//track.mat = DxConvertMatrix(ConvertMatrixA(mat));		
		//data->second->animlist.push_back(track);
		fCurrentTime += m_Scene.fDeltaTime;
		// mat 1차 부모행렬 역행렬 곱한다.
		FbxAMatrix self;
		// self 2차 행렬 분해( S, R, T )
		// 3차 에니메이션 트랙 행렬
		// S(벡터), R(쿼터니언), T(벡터)를 샘플링 시간 간격으로 저장
	}
	stopwatch.Output(L"aa");
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

	m_Scene.iFirstFrame = fStartTime * 30.0f;
	m_Scene.iLastFrame = fEndTime * 30.0f;
	m_Scene.iFrameSpeed = 30;
	m_Scene.iTickPerFrame = 160;
	m_Scene.iDeltaTick = 1;
	m_Scene.fDeltaTime = fFrameTime * 1.0f;
	m_Scene.fFirstTime = fStartTime;
	m_Scene.fLastTime = fEndTime;
}