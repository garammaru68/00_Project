#include "SQuadtree.h"
bool   SQuadtree::CreateQuadtree(SMap* pMap)
{
	m_pMap = pMap;
	//1)  맵 인덱스데이터 통해서 공간분할
	BuildTree();
	return true;
}
bool SQuadtree::BuildTree()
{
	// 1)루트노드  TNode
	UINT tr, tl, br, bl;
	tr = 0;
	tl = m_pMap->m_iNumCols - 1;
	br = m_pMap->m_iNumVertices - 1;
	bl = m_pMap->m_iNumVertices - m_pMap->m_iNumCols;

	m_pRootNode = CreateNode(NULL, tr, tl, bl, br);
	m_pRootNode->dwDepth = 0;
	BuildQuadtree(m_pRootNode);
	return true;
}
bool   SQuadtree::SpliteChild(SNode* pNode)
{
	UINT iDepth = pNode->dwDepth + 1;
	pNode->isLeaf = FALSE;
	if ((pNode->iEdgeIndex[1] - pNode->iEdgeIndex[0]) <= 1)
	{
		pNode->isLeaf = TRUE;
		m_leafList.push_back(pNode);
		return false;
	}
	if (iDepth < 4)
	{
		pNode->isLeaf = FALSE;
		return true;
	}
	pNode->isLeaf = TRUE;
	m_leafList.push_back(pNode);
	return false;
}
// 0  1  e0  3  4
// 5  6  7  8   9 
// e1 11 e2 13 e3
// 15 16 17 18 19 
// 20 21 e4 23 24
bool   SQuadtree::BuildQuadtree(SNode* pNode)
{
	// 분할가능하면
	if (SpliteChild(pNode))
	{
		float fWidthSplit = (pNode->iEdgeIndex[1] - pNode->iEdgeIndex[0]) / 2;
		float fHeightSplit = (pNode->iEdgeIndex[3] - pNode->iEdgeIndex[2]) / 2;
		UINT e0 = pNode->iEdgeIndex[0] + fWidthSplit;
		UINT e1 = pNode->iEdgeIndex[0] + (m_pMap->m_iNumCols * fHeightSplit);
		UINT e2 = e0 + (m_pMap->m_iNumCols * fHeightSplit);
		UINT e3 = pNode->iEdgeIndex[1] + (m_pMap->m_iNumCols * fHeightSplit);;
		UINT e4 = pNode->iEdgeIndex[3] - fHeightSplit;

		pNode->pChild[0] = CreateNode(pNode, pNode->iEdgeIndex[0], e0, e1, e2);
		pNode->pChild[1] = CreateNode(pNode, e0, pNode->iEdgeIndex[1], e2, e3);
		pNode->pChild[2] = CreateNode(pNode, e1, e2, pNode->iEdgeIndex[2], e4);
		pNode->pChild[3] = CreateNode(pNode, e2, e3, e4, pNode->iEdgeIndex[3]);

		BuildQuadtree(pNode->pChild[0]);
		BuildQuadtree(pNode->pChild[1]);
		BuildQuadtree(pNode->pChild[2]);
		BuildQuadtree(pNode->pChild[3]);
	}
	return true;
}
SNode*  SQuadtree::CreateNode(SNode* pParentNode, UINT tl, UINT tr, UINT bl, UINT br)
{
	SNode* pNode = new SNode;
	if (pParentNode != nullptr)
	{
		pNode->dwDepth = pParentNode->dwDepth + 1;
	}
	pNode->iEdgeIndex[0] = tl;
	pNode->iEdgeIndex[1] = tr;
	pNode->iEdgeIndex[2] = bl;
	pNode->iEdgeIndex[3] = br;

	vector<DWORD> Indexlist;
	DWORD dwTL = pNode->iEdgeIndex[0];
	DWORD dwTR = pNode->iEdgeIndex[1];
	DWORD dwBL = pNode->iEdgeIndex[2];
	DWORD dwBR = pNode->iEdgeIndex[3];

	DWORD dwSize = (dwTR - dwTL)*(dwTR - dwTL) * 2 * 3;
	pNode->m_IndexList.resize(dwSize);
	int dwCurentIndex = 0;
	DWORD m_dwWidth = m_pMap->m_iNumCols;
	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;
	//  0      
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			//0	1    4   
			//2	   3 5
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pNode->m_IndexList[dwCurentIndex++] = dwRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurentIndex++] = dwNextRow * m_dwWidth + dwNextCol;
		}
	}

	pNode->pIndexBuffer.Attach(
		CreateIndexBuffer(g_pd3dDevice,
			&pNode->m_IndexList.at(0),
			pNode->m_IndexList.size(),
			sizeof(DWORD)));
	return pNode;
}

bool   SQuadtree::Render(ID3D11DeviceContext* pd3dContext)
{
	Draw(m_pRootNode, pd3dContext);
	return true;
}
void   SQuadtree::Draw(SNode* pNode,
	ID3D11DeviceContext* pd3dContext)
{
	if (pNode == nullptr) return;
	if (pNode->isLeaf == TRUE)
	{
		SDxObject* pObject = (SDxObject*)m_pMap;
		pObject->Update(pd3dContext);
		m_pMap->PreRender(pd3dContext);
		UINT iStride = sizeof(PNCT_VERTEX);
		UINT iOffset = 0;
		pd3dContext->IASetVertexBuffers(0, 1, &m_pMap->m_pVertexBuffer, &iStride, &iOffset);
		pd3dContext->IASetIndexBuffer(
			pNode->pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

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
		pd3dContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
		//pd3dContext->DrawIndexed(m_pMap->m_IndexList.size(), 0, 0);
		return;
	}
	Draw(pNode->pChild[0], pd3dContext);
	Draw(pNode->pChild[1], pd3dContext);
	Draw(pNode->pChild[2], pd3dContext);
	Draw(pNode->pChild[3], pd3dContext);
}

bool SQuadtree::Release()
{
	delete m_pRootNode;
	return true;
}