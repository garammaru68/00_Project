#include "SNode.h"

S_BoxObject::S_BoxObject()
{
	//srand(time(NULL));

	m_Box.vCenter = Vector3(0.0f, 0.0f, 0.0f);
	m_Box.vMax = Vector3(1.0f, 1.0f, 1.0f);
	m_Box.vMin = Vector3(-1.0f, -1.0f, -1.0f);

	m_vPosition = Vector3(31 - rand() % 62, 0, 31 - rand() % 62);
	m_vColor = Vector4((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);

	// g_matWorld Matrix = S * R
	Matrix matScale, matRotation, matWorld;
	matScale._11 = (rand() % 256) / 255.0f * 2.0f;
	matScale._22 = (rand() % 256) / 255.0f * 2.0f;
	matScale._33 = (rand() % 256) / 255.0f * 2.0f;
	matRotation = Matrix::CreateRotationX((rand() % 256) / 255.0f);
	matWorld = matScale * matRotation;

	// OBB 기저벡터 변환( 회전 변환 이후 정규화 )
	m_Box.vCenter = m_vPosition;
	Vector3 vMax, vMin, vHalf;
	m_Box.vAxis[0] = Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f), matWorld);
	m_Box.vAxis[1] = Vector3::Transform(Vector3(0.0f, 1.0f, 0.0f), matWorld);
	m_Box.vAxis[2] = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), matWorld);

	m_Box.vAxis[0].Normalize();
	m_Box.vAxis[1].Normalize();
	m_Box.vAxis[2].Normalize();
	// g_matWorld Matrix = S * R * T
	matWorld._41 = m_vPosition.x;
	matWorld._42 = m_vPosition.y;
	matWorld._43 = m_vPosition.z;

	// OBB 기저벡터의 크기 계산
	m_Box.vMax = Vector3::Transform(m_Box.vMax, matWorld);
	m_Box.vMin = Vector3::Transform(m_Box.vMin, matWorld);
	vHalf = vMax - m_Box.vCenter;

	m_Box.fExtent[0] = m_Box.vAxis[0].Dot(vHalf);
	m_Box.fExtent[1] = m_Box.vAxis[1].Dot(vHalf);
	m_Box.fExtent[2] = m_Box.vAxis[2].Dot(vHalf);

	// AABB
	Vector3 vVertex[8];
	vVertex[0] = Vector3(m_Box.vMin.x, m_Box.vMax.y, m_Box.vMin.z);
	vVertex[1] = Vector3(m_Box.vMax.x, m_Box.vMax.y, m_Box.vMin.z);
	vVertex[2] = Vector3(m_Box.vMax.x, m_Box.vMin.y, m_Box.vMin.z);
	vVertex[3] = Vector3(m_Box.vMin.x, m_Box.vMin.y, m_Box.vMin.z);

	vVertex[4] = Vector3(m_Box.vMin.x, m_Box.vMax.y, m_Box.vMax.z);
	vVertex[5] = Vector3(m_Box.vMax.x, m_Box.vMax.y, m_Box.vMax.z);
	vVertex[6] = Vector3(m_Box.vMax.x, m_Box.vMin.y, m_Box.vMax.z);
	vVertex[7] = Vector3(m_Box.vMin.x, m_Box.vMin.y, m_Box.vMax.z);

	vMin = Vector3(100000.0f, 100000.0f, 100000.0f);
	vMax = Vector3(-100000.0f, -100000.0f, -100000.0f);
	for (int iVer = 0; iVer < 8; iVer++)
	{
		vMin = Vector3::Transform(vVertex[iVer], matWorld);
		if (vVertex[iVer].x > vMax.x) vMax.x = vVertex[iVer].x;
		if (vVertex[iVer].y > vMax.y) vMax.y = vVertex[iVer].y;
		if (vVertex[iVer].z > vMax.z) vMax.z = vVertex[iVer].z;

		if (vVertex[iVer].x < vMin.x) vMin.x = vVertex[iVer].x;
		if (vVertex[iVer].y < vMin.y) vMin.y = vVertex[iVer].y;
		if (vVertex[iVer].z < vMin.z) vMin.z = vVertex[iVer].z;
	}

	m_Box.vMin = vMin;
	m_Box.vMax = vMax;
	m_matWorld = matWorld;
}

SNode::SNode()
{
	m_pChild[0] = nullptr;
	m_pChild[1] = nullptr;
	m_pChild[2] = nullptr;
	m_pChild[3] = nullptr;

	m_dwDepth = 0;
	m_blLeaf = FALSE;

	m_Box.vCenter = Vector3(0.0f, 0.0f, 0.0f);
	m_Box.vMax = Vector3(1.0f, 1.0f, 1.0f);
	m_Box.vMin = Vector3(-1.0f, -1.0f, -1.0f);
	m_Box.vAxis[0] = Vector3(1.0f, 0.0f, 0.0f);
	m_Box.vAxis[1] = Vector3(0.0f, 1.0f, 0.0f);
	m_Box.vAxis[2] = Vector3(0.0f, 0.0f, 1.0f);
	m_Box.fExtent[0] = 1;
	m_Box.fExtent[1] = 1;
	m_Box.fExtent[2] = 1;

}
SNode::~SNode()
{
	if (m_pChild[0] != nullptr)
	{
		for (int iChild = 0; iChild < 4; iChild++)
		{
			delete m_pChild[iChild];
		}
	}

	if (!m_vCornerList.empty()) m_vCornerList.clear();

	for (int i = 0; i < m_ChildList.size(); i++)
	{
		SAFE_DEL(m_ChildList[i]);
	}

}