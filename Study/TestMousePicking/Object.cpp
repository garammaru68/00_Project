#include "Object.h"

S_BoxObject::S_BoxObject()
{
	m_Box.vCenter = Vector3(0.0f, 0.0f, 0.0f);
	m_Box.vMax = Vector3(1.0f, 1.0f, 1.0f);
	m_Box.vMin = Vector3(-1.0f, -1.0f, -1.0f);

	// OBB
	m_vPosition = Vector3(31 - rand() % 62, 0, 31 - rand() % 62);
	m_vColor = Vector4((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);

	Matrix matScale, matRotation, matWorld;
	matScale._11 = (rand() % 256) / 255.0f * 2.0f;
	matScale._22 = (rand() % 256) / 255.0f * 2.0f;
	matScale._33 = (rand() % 256) / 255.0f * 2.0f;
	matRotation = Matrix::CreateRotationX((rand() % 256) / 255.0f);
	matWorld = matScale * matRotation;

	m_Box.vCenter = m_vPosition;
	Vector3 vMax, vMin, vHalf;
	m_Box.vAxis[0] = Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f), matWorld);
	m_Box.vAxis[1] = Vector3::Transform(Vector3(0.0f, 1.0f, 0.0f), matWorld);
	m_Box.vAxis[2] = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), matWorld);

	m_Box.vAxis[0].Normalize();
	m_Box.vAxis[1].Normalize();
	m_Box.vAxis[2].Normalize();

	matWorld._41 = m_vPosition.x;
	matWorld._42 = m_vPosition.y;
	matWorld._43 = m_vPosition.z;

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
Object::Object()
{

}
Object::~Object()
{

}