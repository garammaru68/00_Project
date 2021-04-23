#include "SFbxObj.h"
void SFbxObj::AddKey(FbxNode* pNode, FbxAMatrix matGlobal,
	float fCurrentTime)
{
	auto anim = m_pFBXScene->GetAnimationEvaluator();
	auto data = m_sNodeMap.find(pNode);
	// matGlobal = �ڽ� * �θ�
	Matrix pChildGlobal = DxConvertMatrix(
		ConvertMatrixA(matGlobal));
	FbxNode* pParentNode = pNode->GetParent();

	Matrix matInvParentGlobal = Matrix::Identity;
	if (pParentNode)
	{
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix matFbxParent =
			anim->GetNodeGlobalTransform(pParentNode, t); // ����ð�(fCurrentTime)�� ��ȯ ����� ��ȯ�Ѵ�
		matInvParentGlobal = DxConvertMatrix(
			ConvertMatrixA(matFbxParent));
		matInvParentGlobal = matInvParentGlobal.Invert();
	}
	Matrix m = pChildGlobal * matInvParentGlobal; // �ڱ� �ڽ� ���

	// ����
	Vector3 scale;
	Quaternion qRot;
	Vector3 pos;
	// ���� ��� ����
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
	// eSkin�� �ش��ϴ� �ε���(����) ��������
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	skindata.resize(iVertexCount);
	// Deformer -> Cluster -> Link
	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		DWORD dwClusterCount = pSkin->GetClusterCount();
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			auto pCluster = pSkin->GetCluster(dwClusterIndex);

			FbxAMatrix matXBindPose;
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(matXBindPose); // Link Node�� ��ȯ �Ű� ���� ���(matXBindPose)�� �����´�
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

			Matrix matInvBindPos = DxConvertMatrix(ConvertMatrixA(matBindPose));
			matInvBindPos = matInvBindPos.Invert();
			std::string name = pCluster->GetLink()->GetName();
			m_dxMatrixBindPoseMap.insert(make_pair(name, matInvBindPos));
			pObj->m_dxMatrixBindPosMap.insert(make_pair(name, matInvBindPos));

			int  dwClusterSize = pCluster->GetControlPointIndicesCount();
			auto data = m_pFbxNodeMap.find(pCluster->GetLink());
			int  iBoneIndex = data->second;
			// ������ �޴� �������� �ε���
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
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray); // �ִϸ��̼� ���� �̸��� �迭(string)�� ä���
	int iAnimStackCount = AnimStackNameArray.GetCount();   // �ִϸ��̼� ����(Data->Size)
	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i)); // i(�ε���) ��ġ�� ������(Name)�� �ҷ��´�
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
	auto anim = m_pFBXScene->GetAnimationEvaluator(); // ����(��ġ) ��������
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
		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t); // ����ð�(fCurrentTime)�� ��ȯ ����� ��ȯ�Ѵ�
		AddKey(pNode, mat, fCurrentTime);
		fCurrentTime += m_Scene.fDeltaTime;

		//SAnimTrack track;
		//track.iTick = fCurrentTime * 30* 160;
		//track.mat = DxConvertMatrix(ConvertMatrixA(mat));		
		//data->second->animlist.push_back(track);

		// mat 1�� �θ���� ����� ���Ѵ�.
		// self 2�� ��� ����( S, R, T )
		// 3�� ���ϸ��̼� Ʈ�� ���
		// S(����), R(���ʹϾ�), T(����)�� ���ø� �ð� �������� ����
	}
	stopwatch.Output(L"aa");
	// Child ��� ���鼭 ���� ��������
	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		ParseNodeAnimation(pChildNode);
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
	m_Scene.fDeltaTime = fFrameTime * 1.0f;
	m_Scene.fFirstTime = fStartTime;
	m_Scene.fLastTime = fEndTime;
}