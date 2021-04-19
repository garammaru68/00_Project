#pragma once
#include "SCore.h"
#include "SShape.h"
#include "SObject.h"
#include "SModelViewCamera.h"
#include "SHeightMap.h"
#include "SMiniMap.h"
#include "SQuadTree.h"
#include "SSkyBox.h"
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
	Vector4			g_vLightDir[g_iNumLight]; // w = light damping(attenuation) : 감쇠
	Vector4			g_vLightPos[g_iNumLight]; // w = light radius
	Vector4			g_vEyeDir[g_iNumLight];// w = light intensity : 강도
	Vector4			g_vEyePos[g_iNumLight];// w = light radius		
};
struct LIGHT_CONSTANT_BUFFER3
{
	Vector4			g_vSpotInfo[g_iNumLight];
	//float			g_fFalloff;		// 내부 콘과 외부 콘의 휘도( Luminance )
	//float			g_fTheta;		// 내부 콘의 각도 범위		
	//float			g_fPhi;			// 외부 콘의 각도 범위
};
class SBoxUser : public SShapeBox
{
public:
	void FrontMovement(float fDir)
	{
		Vector3 vOffset = m_vLook * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vOffset;
	}
	void RightMovement(float fDir)
	{
		Vector3 vMove = m_vRight * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
	}
	void UpMovement(float fDir)
	{
		Vector3 vMove = m_vUp * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
	}
	void FrontBase(float fDir)
	{
		Vector3 vSide = { 0,0,1 };
		Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
		m_vTarget += m_vLook * m_fSpeed;
	}
	void RightBase(float fDir)
	{
		Vector3 vSide = { 1,0,0 };
		Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
		m_vTarget += m_vLook * m_fSpeed;
	}
	void UpBase(float fDir)
	{
		Vector3 vUp = { 0,1,0 };
		Vector3 vMove = vUp * g_fSecondPerFrame * m_fSpeed * fDir;
		m_vPos += vMove;
		m_vTarget += m_vLook * m_fSpeed;
	}
};
class Sample : public SCore
{
public:
	Vector3		m_vLightVector;
	LIGHT_CONSTANT_BUFFER1   m_cbLight1;
	LIGHT_CONSTANT_BUFFER2   m_cbLight2;
	LIGHT_CONSTANT_BUFFER3   m_cbLight3;
	ComPtr<ID3D11Buffer>	m_pConstantBufferLight[g_iNumLight];
public:
	SQuadTree		m_QuadTree;
	S_BoxObject*	m_pObject;
	S_BOX			m_TBoxBase;
	SampleSkyBox			m_Skybox;
	SMiniMap		m_MiniMap;
	SHeightMap		m_Map;
	Vector4			m_vDirValue;
	std::vector<SObject*> m_ObjList;
	SBoxUser		m_UserShape;
	SBoxUser		m_BoxShape;
	SShapePlane		m_PlaneShape;
	SModelViewCamera  m_ModelCamera;
	SCamera			m_TopCamera;
public:
	Matrix			m_matBoxWorld;
	Matrix			m_matPlaneWorld;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool PostRender() override;
	bool Release() override;
	bool DrawQuadLine(SNode* pNode);
	Matrix CreateMatrixShadow(
		Vector4* plane,
		Vector4* Light);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void DrawObject(Matrix* pView, Matrix* pProj);
};

SGAME_RUN;