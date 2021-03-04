#include "SQuadTree.h"
SNode* SQuadTree::CreateNode(SNode* pParentNode, UINT TopLeft, UINT TopRight, UINT BottomLeft, UINT BottomRight)
{
	SNode* pNode;
	SAFE_NEW(pNode, SNode);
	if (pParentNode != nullptr)
	{
		pNode->m_dwDepth = pParentNode->m_dwDepth + 1;
	}
	pNode->m_iCornerIndex[0] = TopLeft;
	pNode->m_iCornerIndex[1] = TopRight;
	pNode->m_iCornerIndex[2] = BottomLeft;
	pNode->m_iCornerIndex[3] = BottomRight;

	DWORD dwTopLeft		= pNode->m_iCornerIndex[0];
	DWORD dwTopRight	= pNode->m_iCornerIndex[1];
	DWORD dwBottomLeft	= pNode->m_iCornerIndex[2];
	DWORD dwBottomRight = pNode->m_iCornerIndex[3];

	DWORD dwSize = (dwTopRight - dwTopLeft) * (dwTopRight - dwTopLeft);
	pNode->m_IndexList.resize(dwSize);

	int dwCurrentIndex = 0;
	DWORD m_dwWidth = m_pMap->m_iNumCols;
	DWORD dwSRow = dwTopLeft / m_dwWidth;
	DWORD dwERow = dwBottomLeft / m_dwWidth;

	DWORD dwSCol = dwTopLeft % m_dwWidth;
	DWORD dwECol = dwTopRight % m_dwWidth;

	for (DWORD dwRow = dwSRow; dwRow < dwERow; dwRow++)
	{
		for (DWORD dwCol = dwCol; dwCol < dwECol; dwCol++)
		{
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pNode->m_IndexList[dwCurrentIndex++] = dwRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurrentIndex++] = 
		}
	}
	return pNode;
	//DWORD dwSize = (dwTopRight - dwTopLeft) * (dwTopRight - dwTopLeft);
}