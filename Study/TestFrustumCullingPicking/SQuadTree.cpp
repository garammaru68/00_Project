#include "SQuadTree.h"
SNode* SQuadTree::CreateNode(SNode* pParenSNode, UINT TopLeft, UINT TopRight, UINT BottomLeft, UINT BottomRight)
{
	SNode* pNode = new SNode;
	if (pParenSNode != nullptr)
	{
		pNode->m_dwDepth = pParenSNode->m_dwDepth + 1;
	}
	pNode->m_iCornerIndex[0] = TopLeft;
	pNode->m_iCornerIndex[1] = TopRight;
	pNode->m_iCornerIndex[2] = BottomLeft;
	pNode->m_iCornerIndex[3] = BottomRight;

	DWORD dwTopLeft = pNode->m_iCornerIndex[0];
	DWORD dwTopRight = pNode->m_iCornerIndex[1];
	DWORD dwBottomLeft = pNode->m_iCornerIndex[2];
	DWORD dwBottomRight = pNode->m_iCornerIndex[3];

	DWORD dwSize = (dwTopRight - dwTopLeft) * (dwTopRight - dwTopLeft) * 2 * 3;
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
SNode* SQuadTree::CreateObjectNode(SNode* pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight)
{
	SNode* pNode = 0;
	SAFE_NEW(pNode, SNode);
	assert(pNode);

	pNode->m_ChildList.reserve(4);
	pNode->m_vCornerList.reserve(4);

	pNode->m_Box.vMin = Vector3(fTopLeft, 0.0f, fBottomLeft);
	pNode->m_Box.vMax = Vector3(fTopRight, 0.0f, fBottomRight);
	pNode->m_Box.vCenter = (pNode->m_Box.vMax + pNode->m_Box.vMin);
	pNode->m_Box.vCenter.x /= 2.0f;
	pNode->m_Box.vCenter.y /= 2.0f;
	pNode->m_Box.vCenter.z /= 2.0f;
	pNode->m_Box.fExtent[0] = pNode->m_Box.vMax.x - pNode->m_Box.vCenter.x;
	pNode->m_Box.fExtent[1] = pNode->m_Box.vMax.y - pNode->m_Box.vCenter.y;
	pNode->m_Box.fExtent[2] = pNode->m_Box.vMax.z - pNode->m_Box.vCenter.z;

	pNode->m_vCornerList.push_back(Vector3(pNode->m_Box.vMin.x, 0.0f, pNode->m_Box.vMax.z));
	pNode->m_vCornerList.push_back(pNode->m_Box.vMax);
	pNode->m_vCornerList.push_back(pNode->m_Box.vMin);
	pNode->m_vCornerList.push_back(Vector3(pNode->m_Box.vMax.x, 0.0f, pNode->m_Box.vMin.z));

	if (pParentNode)
	{
		pNode->m_dwDepth = pParentNode->m_dwDepth + 1;
		if ((DWORD)m_iMaxDepth < pNode->m_dwDepth)
		{
			m_iMaxDepth = pNode->m_dwDepth;
		}
	}
	return pNode;
}
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
	// ���� �����ϳ�
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
		{
			/*		if (m_bUsedIndexList)
					{
						DWORD dwIndex =
							pNode->m_ChildList[iNode]->m_dwPositionIndex[1] * pow(2.0f, (float)pNode->m_ChildList[iNode]->m_dwDepth) + pNode->m_ChildList[iNode]->m_dwPositionIndex[0];
						DWORD dwValue = pNode->m_ChildList[iNode]->m_dwDepth;
						m_LevelList[dwValue][dwIndex] =
							pNode->m_ChildList[iNode];
					}*/
			BuildTree(pNode->m_ChildList[iNode]);
		}
	}
	return true;
}
bool SQuadTree::SubDivide(SNode* pNode)
{
	// 4��� �Ҽ� ������ ���̻� �������� �ʴ´�.
	if (pNode == NULL)
	{
		return false;
	}

	// �ִ� ���� �ѵ� �ʰ��� ���� ������� ����
	if ((DWORD)m_iMaxDepthLimit <= pNode->m_dwDepth)
	{
		pNode->m_blLeaf = TRUE;
		return false;
	}

	// ���� ����� ���� ũ�⸦ ����Ѵ�.

	float fWidthSplit = (pNode->m_vCornerList[tTR].x - pNode->m_vCornerList[tTL].x) / 2;
	float fHeightSplit = (pNode->m_vCornerList[tTL].z - pNode->m_vCornerList[tBL].z) / 2;

	// �ڽ� ��尡 ������ ����ũ�⺸�� �۴ٸ� ���̻� �������� �ʴ´�.
	if (fWidthSplit < m_fMinDivideSize || fHeightSplit < m_fMinDivideSize)
	{
		pNode->m_blLeaf = TRUE;
		return false;
	}

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_vCornerList[tTL].x,
		pNode->m_vCornerList[tTL].x + fWidthSplit,
		pNode->m_vCornerList[tTL].z - fHeightSplit,
		pNode->m_vCornerList[tTL].z));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_vCornerList[tTL].x + fWidthSplit,
		pNode->m_vCornerList[tTR].x,
		pNode->m_vCornerList[tTL].z - fHeightSplit,
		pNode->m_vCornerList[tTL].z));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_vCornerList[tTL].x,
		pNode->m_vCornerList[tTL].x + fWidthSplit,
		pNode->m_vCornerList[tBL].z,
		pNode->m_vCornerList[tTL].z - fHeightSplit));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_vCornerList[tTL].x + fWidthSplit,
		pNode->m_vCornerList[tTR].x,
		pNode->m_vCornerList[tBR].z,
		pNode->m_vCornerList[tTL].z - fHeightSplit));


	return true;
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

bool SQuadTree::SBuildTree()
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
	SBuildTree();
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

int SQuadTree::AddObject(Object* pObj)
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
int SQuadTree::CheckRect(SNode* pNode, Object* pObj)
{
	if (pNode->m_Box.vMin.x <= pObj->m_Box.vMin.x && pNode->m_Box.vMax.x >= pObj->m_Box.vMax.x)
	{
		if (pNode->m_Box.vMin.z <= pObj->m_Box.vMin.z && pNode->m_Box.vMax.z >= pObj->m_Box.vMax.z)
		{
			return 1;
		}
	}
	return 0;
}
SNode*	SQuadTree::FindNode(SNode* pNode, Object* pObj)
{
	assert(pNode);
	do {
		for (DWORD dwChild = 0; dwChild < pNode->m_ChildList.size(); dwChild++)
		{
			if (pNode->m_ChildList[dwChild] &&
				CheckRect(pNode->m_ChildList[dwChild], pObj))
			{
				m_qQuadTree.push(pNode->m_ChildList[dwChild]);
				break;
			}
		}

		if (m_qQuadTree.empty()) break;

		pNode = m_qQuadTree.front();
		m_qQuadTree.pop();
	} while (pNode);
	return pNode;
}
bool SQuadTree::Release()
{
	SAFE_DEL(m_pRootNode);
	return true;
}