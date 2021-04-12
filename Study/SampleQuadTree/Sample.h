#pragma once
#include "SCore.h"
#include "SMap.h"
#include "SQuadtree.h"
//#include "HeightMap"
struct S_RAY
{
	Vector3 vDirection;
	Vector3 vOrigin;
};
class SHeightMap :public SMap
{
public:
	float   GetHeight(UINT index) override;
	bool	CreateHeightMap(ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext, const TCHAR* pszFileName);
};
class Sample : public SCore
{
public:
	SHeightMap m_HeightMap;
	SMap  m_Map;
	SQuadtree m_Quadtree;
	S_RAY   m_Ray;
	Vector3 vRayOrigin;
	Vector3 vRayDir;
	Vector3 m_vIntersection;
	Vector3 vList[3];
	vector<SNode*> m_SelectNode;
	bool   m_bSelect;
public:
	bool	Init();
	bool    Frame();
	bool    Render();
	bool    Release();
	bool    GetIntersection(SNode* pNode);
	bool	IntersectBox(S_BOX* pBox, S_RAY* pRay);
	bool	IntersectRayToSphere(S_SPHERE* pSphere, S_RAY* pRay);
	bool    GetIntersection(
		Vector3 vStart,
		Vector3 vEnd,
		Vector3 vNormal,
		Vector3 v0,
		Vector3 v1,
		Vector3 v2);
	bool    PointInPolygon(
		Vector3 vert,
		Vector3 faceNormal,
		Vector3 v0,
		Vector3 v1,
		Vector3 v2);
};
SGAME_RUN;
