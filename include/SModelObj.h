#pragma once
#include "sstd.h"
#include <fbxsdk.h>
#include <map>
#include <unordered_map>
#include "simpleMath.h"
#include "SObject.h"
struct SWeight
{
	std::vector<int>   Index;
	std::vector<float> Weight;
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
	std::vector<S_TRIANGLE>			m_TriangleList;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<IW_VERTEX>		m_VertexListIW;
	ComPtr<ID3D11Buffer>	    m_pVertexBuffer;
	ComPtr<ID3D11Buffer>	    m_pVertexBufferIW;
	ComPtr<ID3D11Buffer>	    m_pIndexBuffer;
	STexture*				    m_pTexture;

	//vector<PNCT_VERTEX>	m_VertexArray;
	vector<DWORD>		m_IndexArray;

	int		m_iNumFace = 0;

	void    SetUniqueBuffer(S_TRIANGLE& tri);

	~SSubMesh()
	{
		m_iNumFace = 0;
	}
};
struct SAnimTrack
{
	int   iTick;
	Vector3 p;
	Vector3 s;
	Quaternion q;
	Matrix mat;
};
struct SScene
{
	int iFirstFrame;
	int iLastFrame;
	int iFrameSpeed; // 30
	int iTickPerFrame;// 160
	int iNumMesh;
	int iDeltaTick; // 1frame
	float fDeltaTime;
	float fFirstTime;
	float fLastTime;
};
class SModelObj : public SObject
{
public:
	SModelObj*  m_pParentObject = nullptr;
	Matrix    m_matAnim;
	std::unordered_map<std::string, Matrix> m_dxMatrixBindPoseMap;
	std::vector<std::wstring>   fbxMaterialList;
	std::vector<SSubMesh>		subMesh;
	std::vector<SAnimTrack>     animlist;
	std::vector<SWeight>		WeightList;
	bool						m_bSkinnedMesh = false;
public:

	bool	CreateInputLayout() override;

	SModelObj()
	{
	}
	virtual ~SModelObj()
	{
	}
};
