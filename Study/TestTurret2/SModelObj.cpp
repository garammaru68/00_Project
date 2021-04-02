#include "SModelObj.h"
struct IsTriangleSame
{
	S_TRIANGLE			m_Tri;
	IsTriangleSame(S_TRIANGLE data) : m_Tri(data) {}
	bool operator()(S_TRIANGLE& value)
	{
		return value.iSubIndex == m_Tri.iSubIndex;
	}
};
void SSubMesh::SetUniqueBuffer(S_TRIANGLE& tri)
{
	for (int iVertex = 0; iVertex < 3; iVertex++)
	{
		bool bAdd = true;
		int iPosReturn = -1;
		for (int iIndex = 0; iIndex < m_VertexList.size(); iIndex++)
		{
			if (m_VertexList[iIndex] == tri.vVertex[iVertex])
			{
				m_IndexArray.push_back(iIndex);
				bAdd = false;
				break;
			}
		}
		if (bAdd)
		{
			m_VertexList.push_back(tri.vVertex[iVertex]);
			m_VertexListIW.push_back(tri.vVertexIW[iVertex]);
			m_IndexArray.push_back(m_VertexList.size() - 1);
		}
	}
	m_iNumFace++;
}