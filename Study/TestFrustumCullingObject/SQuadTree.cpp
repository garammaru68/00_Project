#include "SQuadTree.h"
bool SQuadTree::Build(float fWidth, float fHeight)
{
	m_fWidth = fWidth, m_fHeight = fHeight;
	m_pRootNode = CreateNode(NULL, -fWidth / 2, fWidth / 2, -fHeight / 2, fHeight / 2);

	if (BuildTree(m_pRootNode))
	{
		return true;
	}
	return false;
}
bool SQuadTree::BuildTree(SNode* pNode)
{
	// 분할 가능하냐
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
		{
			BuildTree(pNode->m_ChildList[iNode]);
		}
	}
	return true;
}
bool SQuadTree::SubDivide(SNode* pNode)
{
	if (pNode == NULL)
	{
		return false;
	}

	if ((DWORD)m_iMaxDepthLimit <= pNode->m_dwDepth)
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}

	// 현재 노드의 실제 크기를 계산한다.

	float fWidthSplit = (pNode->m_CornerList[tTR].x - pNode->m_CornerList[tTL].x) / 2;
	float fHeightSplit = (pNode->m_CornerList[tTL].z - pNode->m_CornerList[tBL].z) / 2;

	// 자식 노드가 지정된 분할크기보다 작다면 더이상 분할하지 않는다.
	if (fWidthSplit < m_fMinDivideSize || fHeightSplit < m_fMinDivideSize)
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[tTL].x,
		pNode->m_CornerList[tTL].x + fWidthSplit,
		pNode->m_CornerList[tTL].z - fHeightSplit,
		pNode->m_CornerList[tTL].z));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[tTL].x + fWidthSplit,
		pNode->m_CornerList[tTR].x,
		pNode->m_CornerList[tTL].z - fHeightSplit,
		pNode->m_CornerList[tTL].z));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[tTL].x,
		pNode->m_CornerList[tTL].x + fWidthSplit,
		pNode->m_CornerList[tBL].z,
		pNode->m_CornerList[tTL].z - fHeightSplit));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[tTL].x + fWidthSplit,
		pNode->m_CornerList[tTR].x,
		pNode->m_CornerList[tBR].z,
		pNode->m_CornerList[tTL].z - fHeightSplit));


	return true;
}
SNode* SQuadTree::CreateNode(SNode* pParenSNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight)
{
	SNode* pNode = 0;
	SAFE_NEW(pNode, SNode);
	assert(pNode);

	pNode->m_ChildList.reserve(4);
	pNode->m_CornerList.reserve(4);

	pNode->m_tBox.vMin = Vector3(fTopLeft, 0.0f, fBottomLeft);
	pNode->m_tBox.vMax = Vector3(fTopRight, 0.0f, fBottomRight);
	pNode->m_tBox.vCenter = (pNode->m_tBox.vMax + pNode->m_tBox.vMin);
	pNode->m_tBox.vCenter.x /= 2.0f;
	pNode->m_tBox.vCenter.y /= 2.0f;
	pNode->m_tBox.vCenter.z /= 2.0f;
	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;

	pNode->m_CornerList.push_back(Vector3(pNode->m_tBox.vMin.x, 0.0f, pNode->m_tBox.vMax.z));
	pNode->m_CornerList.push_back(pNode->m_tBox.vMax);
	pNode->m_CornerList.push_back(pNode->m_tBox.vMin);
	pNode->m_CornerList.push_back(Vector3(pNode->m_tBox.vMax.x, 0.0f, pNode->m_tBox.vMin.z));

	if (pParenSNode)
	{
		pNode->m_dwDepth = pParenSNode->m_dwDepth + 1;
		if ((DWORD)m_iMaxDepth < pNode->m_dwDepth)
		{
			m_iMaxDepth = pNode->m_dwDepth;
		}
	}
	return pNode;
}

int SQuadTree::AddObject(SBaseObj* pObj)
{
	if (CheckRect(m_pRootNode, pObj))
	{
		SNode* pNode = FindNode(m_pRootNode, pObj);
		if (pNode)
		{
			pNode->m_ObjectList.push_back(pObj);
		}
		return 1;
	}
	return 0;
}
int SQuadTree::CheckRect(SNode* pNode, SBaseObj* pObj)
{
	if (pNode->m_tBox.vMin.x <= pObj->m_tBox.vMin.x && pNode->m_tBox.vMax.x >= pObj->m_tBox.vMax.x)
	{
		if (pNode->m_tBox.vMin.z <= pObj->m_tBox.vMin.z && pNode->m_tBox.vMax.z >= pObj->m_tBox.vMax.z)
		{
			return 1;
		}
	}
	return 0;
}
SNode*	SQuadTree::FindNode(SNode* pNode, SBaseObj* pObj)
{
	assert(pNode);
	do {
		for (DWORD dwChild = 0; dwChild < pNode->m_ChildList.size(); dwChild++)
		{
			if (pNode->m_ChildList[dwChild] &&
				CheckRect(pNode->m_ChildList[dwChild], pObj))
			{
				m_QuadTreeQueue.push(pNode->m_ChildList[dwChild]);
				break;
			}
		}

		if (m_QuadTreeQueue.empty()) break;

		pNode = m_QuadTreeQueue.front();
		m_QuadTreeQueue.pop();
	} while (pNode);
	return pNode;
}
bool SQuadTree::PreFrame()
{
	m_DrawObjList.clear();
	m_DrawNodeList.clear();
	//if (!m_pCamera) return false;
	return true;
}
bool SQuadTree::Frame()
{
	if (!PreFrame()) return false;
	return true;
}

void SQuadTree::SetNeighborNode(SNode* pNode)
{
	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		SetNeighborNode(pNode->m_ChildList[iNode]);
	}
}
DWORD SQuadTree::CheckSize(DWORD dwSize)
{
	float fCount = 0;
	DWORD dwChecker = dwSize / 2;
	if (dwChecker <= 0) return 0;

	while (dwChecker != 0)
	{
		dwChecker >>= 1;
		fCount += 1.0f;
	}
	float fMinimum = pow(2.0f, fCount - 1.0f);
	float fMaximum = pow(2.0f, fCount);
	float fMin = fabs(fMinimum - fabs(((float)dwSize - fMinimum)));
	float fMax = fabs(fMaximum - fabs(((float)dwSize - fMaximum)));

	if (fMin <= fMax)
	{
		return (DWORD)fMinimum;
	}
	return (DWORD)fMaximum;
}
bool SQuadTree::Release()
{
	SAFE_DEL(m_pRootNode);
	return true;
}


SQuadTree::SQuadTree(void)
{
	m_iRenderDepth = 0;
	m_iMaxDepth = 0;
	m_iMaxDepthLimit = 1;
	m_fMinDivideSize = 2;
	m_bUsedIndexList = false;
	g_pd3dDevice = 0;
	m_pRootNode = 0;
}

SQuadTree::~SQuadTree(void)
{
	Release();
}
// 127007  1984   15 434
// 0 31 63
// 0 31 
// 