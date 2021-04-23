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

bool SModelObj::CreateInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	hr = g_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		m_pVSObj->GetBufferPointer(),
		m_pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	if (FAILED(hr)) return false;
	return true;
}
bool SModelObj::AniFrame(FLOAT fCurFrame, FLOAT fElapsedTime, int iFirstFrame, int iLastFrame, Matrix* pMatrix)
{
	if (pMatrix == NULL)
	{
		pMatrix = m_pMatrix;
	}

	bool bResult = false;
	Quaternion	TmpQuat;
	int iCurFrame = (int)fCurFrame;
	int iNextFrame = iCurFrame + 1;

	Vector3 vScale, vTrans;
	Matrix matRotate, matTrans;
	Matrix matScale = Matrix::Identity;
	for (int i = 0; i < m_Scene.iNumMesh; i++)
	{
		if (iNextFrame >= iLastFrame)
		{
			iCurFrame = iFirstFrame;
			Quaternion	CurQuat = m_ppAniQuater[i][iCurFrame];
			Quaternion	NextQuat = m_ppAniQuater[i][iCurFrame + 1]; // iNextFrame
			TmpQuat = Quaternion::Slerp(CurQuat, NextQuat, fElapsedTime);
			matRotate = Matrix::CreateFromQuaternion(TmpQuat);
			
			matScale = Matrix::CreateScale(m_ppScaleVector[i][iCurFrame].x, m_ppScaleVector[i][iCurFrame].y, m_ppScaleVector[i][iCurFrame].z);
			vTrans = m_ppTransVector[i][iCurFrame];

			pMatrix[i] = matScale * matRotate;
			pMatrix[i].Translation(vTrans);

			bResult = true;
		}
		else
		{
			Quaternion	CurQuat = m_ppAniQuater[i][iCurFrame];
			Quaternion	NextQuat = m_ppAniQuater[i][iNextFrame];
			TmpQuat = Quaternion::Slerp(CurQuat, NextQuat, fElapsedTime);
			matRotate = Matrix::CreateFromQuaternion(TmpQuat);

			vScale = Vector3::Lerp(m_ppScaleVector[i][iCurFrame], m_ppScaleVector[i][iNextFrame], fElapsedTime);
			matScale = Matrix::CreateScale(vScale.x, vScale.y, vScale.z);
			vTrans = Vector3::Lerp(m_ppScaleVector[i][iCurFrame], m_ppScaleVector[i][iNextFrame], fElapsedTime);

			pMatrix[i] = matScale * matRotate;
			pMatrix[i].Translation(vTrans);
		}
	}
	return bResult;
}