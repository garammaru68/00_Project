#include "SQuadTree.h"
SNode* SQuadTree::CreateNode(SNode* pParenSNode, UINT TopLeft, UINT TopRight, UINT BottomLeft, UINT BottomRight)
{
	SNode* pNode = 0;
	SAFE_NEW(pNode, SNode);
	if (pParenSNode != nullptr)
	{
		pNode->m_dwDepth = pParenSNode->m_dwDepth + 1;
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
	pNode->m_vIndexList.resize(dwSize);

	int iCurrentIndex = 0;
	DWORD m_dwWidth = m_pMap->m_iNumCols;
	DWORD dwSRow = dwTopLeft / m_dwWidth;
	DWORD dwERow = dwBottomLeft / m_dwWidth;

	DWORD dwSCol = dwTopLeft % m_dwWidth;
	DWORD dwECol = dwTopRight % m_dwWidth;

	for (DWORD dwRow = dwSRow; dwRow < dwERow; dwRow++)
	{
		for (DWORD dwCol = dwSCol; dwCol < dwECol; dwCol++)
		{
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pNode->m_vIndexList[iCurrentIndex++] = dwRow * m_dwWidth + dwCol;
			pNode->m_vIndexList[iCurrentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_vIndexList[iCurrentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_vIndexList[iCurrentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_vIndexList[iCurrentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_vIndexList[iCurrentIndex++] = dwNextRow * m_dwWidth + dwNextCol;
		}
	}

	pNode->m_pIndexBuffer.Attach(
		CreateIndexBuffer(g_pd3dDevice,
			&pNode->m_vIndexList.at(0),
			pNode->m_vIndexList.size(),
			sizeof(DWORD)));

	return pNode;
}
bool SQuadTree::BuildTree()
{
	UINT tr, tl, br, bl;
	tr = 0;
	tl = m_pMap->m_iNumCols - 1;
	br = m_pMap->m_iNumVertices - 1;
	bl = m_pMap->m_iNumVertices - m_pMap->m_iNumCols;

	m_pRootNode = CreateNode(NULL, tr, tl, bl, br);
	m_pRootNode->m_dwDepth = 0;
	BuildQuadTree(m_pRootNode);
	return true;
}
bool   SQuadTree::CreateQuadtree(SMap* pMap)
{
	m_pMap = pMap;
	BuildTree();
	return true;
}
bool   SQuadTree::DivideNode(SNode* pNode)
{
	UINT iDepth = pNode->m_dwDepth + 1;
	pNode->m_blLeaf = FALSE;
	if ((pNode->m_iCornerIndex[1] - pNode->m_iCornerIndex[0]) <= 1)
	{
		pNode->m_blLeaf = TRUE;
		m_leafList.push_back(pNode);
		return false;
	}
	if (iDepth < 4)
	{
		pNode->m_blLeaf = FALSE;
		return true;
	}
	pNode->m_blLeaf = TRUE;
	m_leafList.push_back(pNode);
	return false;
}
bool   SQuadTree::BuildQuadTree(SNode* pNode)
{
	if (DivideNode(pNode))
	{
		float fWidthSplit = (pNode->m_iCornerIndex[1] - pNode->m_iCornerIndex[0]) / 2;
		float fHeightSplit = (pNode->m_iCornerIndex[3] - pNode->m_iCornerIndex[2]) / 2;
		UINT e0 = pNode->m_iCornerIndex[0] + fWidthSplit;
		UINT e1 = pNode->m_iCornerIndex[0] + (m_pMap->m_iNumCols * fHeightSplit);
		UINT e2 = e0 + (m_pMap->m_iNumCols * fHeightSplit);
		UINT e3 = pNode->m_iCornerIndex[1] + (m_pMap->m_iNumCols * fHeightSplit);;
		UINT e4 = pNode->m_iCornerIndex[3] - fHeightSplit;

		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_iCornerIndex[0], e0, e1, e2);
		pNode->m_pChild[1] = CreateNode(pNode, e0, pNode->m_iCornerIndex[1], e2, e3);
		pNode->m_pChild[2] = CreateNode(pNode, e1, e2, pNode->m_iCornerIndex[2], e4);
		pNode->m_pChild[3] = CreateNode(pNode, e2, e3, e4, pNode->m_iCornerIndex[3]);

		BuildQuadTree(pNode->m_pChild[0]);
		BuildQuadTree(pNode->m_pChild[1]);
		BuildQuadTree(pNode->m_pChild[2]);
		BuildQuadTree(pNode->m_pChild[3]);
	}
	return true;
}

bool   SQuadTree::Render(ID3D11DeviceContext* pd3dContext)
{
	Draw(m_pRootNode, pd3dContext);
	return true;
}
void   SQuadTree::Draw(SNode* pNode,
	ID3D11DeviceContext* pd3dContext)
{
	if (pNode == nullptr) return;
	if (pNode->m_blLeaf == TRUE)
	{
		SDxObject* pObject = (SDxObject*)m_pMap;
		pObject->Update(pd3dContext);
		m_pMap->PreRender(pd3dContext);
		UINT iStride = sizeof(PNCT_VERTEX);
		UINT iOffset = 0;
		pd3dContext->IASetVertexBuffers(0, 1, &m_pMap->m_pVertexBuffer, &iStride, &iOffset);
		pd3dContext->IASetIndexBuffer(
			pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		pd3dContext->IASetInputLayout(m_pMap->m_pInputLayout);
		pd3dContext->VSSetConstantBuffers(0, 1, &m_pMap->m_pConstantBuffer);
		pd3dContext->PSSetConstantBuffers(0, 1, &m_pMap->m_pConstantBuffer);
		pd3dContext->VSSetShader(m_pMap->m_pVertexShader, NULL, 0);
		pd3dContext->PSSetShader(m_pMap->m_pPixelShader, NULL, 0);
		pd3dContext->IASetPrimitiveTopology(
			(D3D11_PRIMITIVE_TOPOLOGY)m_pMap->m_iTopology);
		if (m_pMap->m_pTexture != nullptr)
		{
			pd3dContext->PSSetShaderResources(0, 1,
				&m_pMap->m_pTexture->m_pTextureSRV);
		}
		pd3dContext->DrawIndexed(pNode->m_vIndexList.size(), 0, 0);

		return;
	}
	Draw(pNode->m_pChild[0], pd3dContext);
	Draw(pNode->m_pChild[1], pd3dContext);
	Draw(pNode->m_pChild[2], pd3dContext);
	Draw(pNode->m_pChild[3], pd3dContext);
}