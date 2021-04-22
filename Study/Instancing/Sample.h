#pragma once
#include "SCore.h"
#include "SMap.h"
#include "SQuadtree.h"
#include "SModel.h"
const int g_iNumLight = 3;
struct LIGHT_CONSTANT_BUFFER1
{
	// Material
	Vector4			g_cAmbientMaterial[g_iNumLight];
	Vector4			g_cDiffuseMaterial[g_iNumLight];
	Vector4			g_cSpecularMaterial[g_iNumLight];
	Vector4			g_cEmissionMaterial[g_iNumLight];
	// Light
	Vector4			g_cAmbientLightColor[g_iNumLight];
	Vector4			g_cDiffuseLightColor[g_iNumLight];
	Vector4			g_cSpecularLightColor[g_iNumLight];
};
struct LIGHT_CONSTANT_BUFFER2
{
	Matrix			g_matInvWorld[g_iNumLight];
	Vector4			g_vLightDir[g_iNumLight]; // w = light damping(attenuation) : °¨¼è
	Vector4			g_vLightPos[g_iNumLight]; // w = light radius
	Vector4			g_vEyeDir[g_iNumLight];// w = light intensity : °­µµ
	Vector4			g_vEyePos[g_iNumLight];// w = light radius		
};
class Sample : public SCore
{
	LIGHT_CONSTANT_BUFFER1   m_cbLight1;
	LIGHT_CONSTANT_BUFFER2   m_cbLight2;
	ComPtr<ID3D11Buffer>	m_pConstantBufferLight[g_iNumLight];
	std::vector<Matrix>  m_ModelList;
public:
	SModel  m_pObj;
	SMap  m_Map;
	SQuadtree m_Quadtree;
	S_RAY   m_Ray;
	Vector3 vRayOrigin;
	Vector3 vRayDir;
	Vector3 m_vIntersection;
	Vector3 vList[3];
	vector<SNode*> m_SelectNode;
	vector<SNode*> m_ControlNode;
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
