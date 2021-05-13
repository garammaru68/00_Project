#include "SPlaneRect.h"
void SControlUI::SetAmbientColor(float fR, float fG, float fB, float fA)
{
	//m_pShape->m_cbData.vColor = Vector4(fR, fG, fB, fA);
}
void SControlUI::SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		//m_pShape->m_vCenter.x = pWorld->_41;
		//m_pShape->m_vCenter.y = pWorld->_42;
		//m_pShape->m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_pShape->m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_pShape->m_matProj = *pProj;
	}
	m_matWorld.Transpose(m_pShape->m_cbData.matWorld);
	m_pShape->m_matView.Transpose(m_pShape->m_cbData.matView);
	m_pShape->m_matProj.Transpose(m_pShape->m_cbData.matProject);
	//D3DXMatrixTranspose(&m_pShape->m_cbData.matWorld, &m_matWorld);
	//D3DXMatrixTranspose(&m_pShape->m_cbData.matView, &m_pShape->m_matView);
	//D3DXMatrixTranspose(&m_pShape->m_cbData.matProj, &m_pShape->m_matProj);
}
bool		SControlUI::Init()
{
	m_vScale.x = 1; m_vScale.y = 1; m_vScale.z = 1;
	m_vRotate.x = 0; m_vRotate.y = 0; m_vRotate.z = 0;
	m_vTrans.x = 0; m_vTrans.y = 0; m_vTrans.z = 0;

	m_matWorld.CreateScale(m_vScale.x, m_vScale.y, m_vScale.z);
	//D3DXMatrixScaling(&m_matWorld, m_vScale.x, m_vScale.y, m_vScale.z);
	m_matWorld._41 = 0;
	m_matWorld._42 = 0;
	m_matWorld._41 = 0;
	// 충돌 평면 구성
	m_Vertices[0].p = Vector3(-1.0f, 1.0f, -1.0f);	m_Vertices[0].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[0].t = Vector2(0.0f, 0.0f);
	m_Vertices[1].p = Vector3(1.0f, 1.0f, -1.0f);	m_Vertices[1].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].t = Vector2(1.0f, 0.0f);
	m_Vertices[2].p = Vector3(1.0f, -1.0f, -1.0f);	m_Vertices[2].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[2].t = Vector2(1.0f, 1.0f);
	m_Vertices[3].p = Vector3(-1.0f, -1.0f, -1.0f);	m_Vertices[3].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[3].t = Vector2(0.0f, 1.0f);

	Vector3 vEye = Vector3(0.0f, 0.0f, -100.0f);
	Vector3 vAt = Vector3(0.0f, 0.0f, 100.0f);
	Vector3 vUp(0.0f, 1.0f, 0.0f);
	//D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUp);
	m_matView.CreateLookAt(vEye, vAt, vUp);
	return true;
}
bool		SControlUI::Set(UINT iWidth, UINT iHeight)
{
	m_iWidthVP = iWidth;
	m_iHeightVP = iHeight;
	m_matViewPort.Identity;
	//D3DXMatrixIdentity(&m_matViewPort);

	m_matViewPort._11 = 1;
	m_matViewPort._22 = -1;
	m_matViewPort._41 = (m_iWidthVP / 2.0f);
	m_matViewPort._42 = (m_iHeightVP / 2.0f);

	CreateResource(iWidth, iHeight);
	return true;
}
void		SControlUI::Move(float x, float y, float z)
{
	m_vTrans.x += x; //-200, 150, 0
	m_vTrans.y += y;
	m_vTrans.z += z;

}
void		SControlUI::Scale(float x, float y, float z)
{
	// 200, 150, 1
	m_vScale.x += x;
	m_vScale.y += y;
	m_vScale.z += z;
}
void		SControlUI::Rotate(float x, float y, float z)
{

}
bool		SControlUI::Update()
{
	Matrix matScale, matRotate;
	matScale.CreateScale(m_vScale.x, m_vScale.y, m_vScale.z);
	matRotate.CreateFromYawPitchRoll(m_vRotate.x, m_vRotate.y, m_vRotate.z);
	//D3DXMatrixRotationYawPitchRoll matRotate.Rota (&matRotate, m_vRotate.x, m_vRotate.y, m_vRotate.z);
	m_matWorld = matScale * matRotate;
	m_matWorld._41 = m_vTrans.x;
	m_matWorld._42 = m_vTrans.y;
	m_matWorld._43 = m_vTrans.z;

	// 2D 클라이언트 좌표계 변환하여 충돌 영역 지정.
	Vector3 v[4];
	for (int iVer = 0; iVer < 4; iVer++)
	{
		v[iVer].Transform(m_Vertices[iVer].p, m_matWorld);
		v[iVer].Transform(v[iVer], m_matViewPort);
		//D3DXVec3TransformCoord(&v[iVer], &m_Vertices[iVer].p, &m_matWorld);
		//D3DXVec3TransformCoord(&v[iVer], &v[iVer], &m_matViewPort);
	}
	m_rt.left = v[0].x;
	m_rt.right = v[1].x;
	m_rt.top = v[0].y;
	m_rt.bottom = v[2].y;
	return true;
}
bool		SControlUI::Begin(ID3D11DeviceContext* pContext) {
	assert(m_pShape);
	m_pShape->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
	return m_pShape->PreRender(pContext);
}
bool		SControlUI::End(ID3D11DeviceContext* pContext) {
	assert(m_pShape);
	return m_pShape->PostRender(pContext);
}
bool		SControlUI::Render(ID3D11DeviceContext* pContext) {
	Begin(pContext);
	End(pContext);
	return true;
}
bool		SControlUI::Release() {
	assert(m_pShape);
	m_pShape->Release();
	return true;
}
HRESULT SControlUI::CreateResource(int iRectWidth, int iRectHeight)
{
	HRESULT hr = S_OK;
	iRectWidth = iRectWidth / 2;
	iRectHeight = iRectHeight / 2;
	//D3DXMatrixOrthoLH(&m_Projection[1], iRectWidth * 2, iRectHeight * 2, 0.0f, 1000.0f);
	// 화면 중앙이 원점으로 계산되기 때문에 넓이 및 높이가 -1 ~ 1 범위로 직교투영된다. 
	m_matProj.CreateOrthographicOffCenter(-iRectWidth, iRectWidth, -iRectHeight, iRectHeight, 0.0f, 1000.0f);
	//D3DXMatrixOrthoOffCenterLH(&m_matProj, -iRectWidth, iRectWidth, -iRectHeight, iRectHeight, 0.0f, 1000.0f);
	return hr;
}
SControlUI::SControlUI()
{
	m_pShape = nullptr;
	Init();
}


SControlUI::~SControlUI()
{
	m_pShape->Release();
}

HRESULT		SButtonCtl::Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString)
{
	HRESULT hr = S_OK;
	if (pLoadShaderFile == nullptr)
	{
		if (FAILED(hr = m_Box.Create(pDevice, L"box.hlsl", pLoadTextureString)))
		{
			return hr;
		}
	}
	else
	{
		if (FAILED(hr = m_Box.Create(pDevice, pLoadShaderFile, pLoadTextureString)))
		{
			return hr;
		}
	}
	m_pShape = &m_Box;
	return hr;
}

HRESULT		SEditCtl::Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString)
{
	HRESULT hr = S_OK;
	if (pLoadShaderFile == nullptr)
	{
		if (FAILED(hr = m_Plane.Create(pDevice, L"plane.hlsl", pLoadTextureString)))
		{
			return hr;
		}
	}
	else
	{
		if (FAILED(hr = m_Plane.Create(pDevice, pLoadShaderFile, pLoadTextureString)))
		{
			return hr;
		}
	}
	m_pShape = &m_Plane;
	return hr;
}
HRESULT		SImageCtl::Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString)
{
	HRESULT hr = S_OK;
	if (pLoadShaderFile == nullptr)
	{
		if (FAILED(hr = m_Box.Create(pDevice, L"plane.hlsl", pLoadTextureString)))
		{
			return hr;
		}
	}
	else
	{
		if (FAILED(hr = m_Box.Create(pDevice, pLoadShaderFile, pLoadTextureString)))
		{
			return hr;
		}
	}
	m_pShape = &m_Box;
	return hr;
}