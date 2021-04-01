#include "SFbxObj.h"
bool SFbxObj::ParseMeshSkinningMap(const FbxMesh* pFbxMesh, std::vector<SWeight>* skindata)
{
	// eSkin�� �ش��ϴ� �ε���(����) ��������
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	skindata->resize(iVertexCount);
	// Deformer -> Cluster -> Link
	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		DWORD dwClusterCount = pSkin->GetClusterCount();
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			int	 dwClusterSize = pCluster->GetControlPointIndicesCount();
			auto data = m_pNodeMap.find(pCluster->GetLink());
			int  iBoneIndex = data->second;

			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeight();
			for (int i = 0; i < dwClusterSize; i++)
			{
				skindata[pIndices[i]].InsertWeight(iBoneIndex, pWeights[i]);
			}
		}
	}
	return true;
}