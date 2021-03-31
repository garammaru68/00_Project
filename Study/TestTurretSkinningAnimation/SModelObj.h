#pragma once
#include <sstd.h>
#include <fbxsdk.h>
#include <map>
#include <unordered_map>
#include <SimpleMath.h>
#include <SObject.h>
struct SWeight
{
	std::vector<float> Index;
	std::vector<float> Weight;
	// Bone에 Weight(가중치) 넣기
	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < Index.size(); ++i)
		{
			if (fBoneWeight > Weight[i])
			{
				for (DWORD j = (Index.size() - 1); j > i; --j)
				{
					Index[j] = Index[j - 1];
					Weight[j] = Weight[j - 1];
				}
				Index[i] = iBoneIndex;
				Weight[i] = fBoneWeight;
				break;
			}
		}
	}
	SWeight()
	{
		Index.resize(8);
		Weight.resize(8);
	}
};
struct SSubMesh
{
	std::vector<S_TRIANGLE>		m_TriangleList;
	std::vector<PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	STexture*					m_pTexture;
	int iCount = 0;
	~SSubMesh()
	{

	}
};
struct SAnimTrack
{
	int iTick;
	Matrix mat;
};
struct SScene
{
	int iFirstFrame;
	int iLastFrame;
	int iFrameSpeed;
	int iTickPerFrame;
	int iNumMesh;
	int iDeltaTick;
	float fDeltaTime;
	float fFirstTime;
	float fLastTime;
};
class SModelObj : public SObject
{
public:
	std::vector<std::wstring>	fbxMaterilList;
	std::vector<SSubMesh>		subMesh;
	std::vector<SAnimTrack>		animlist;
	std::vector<SWeight>		WeightList;
	bool						m_bSkinnedMesh = false;
	SModelObj()
	{
	}
	virtual ~SModelObj()
	{
	}
};