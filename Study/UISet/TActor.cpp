#include "TActor.h"
// ½ºÅ©¸° ÁÂÇ¥ -> ¿ùµå ÁÂÇ¥ -> Åõ¿µ ÁÂÇ¥
D3DXVECTOR3  TActor::Gen(float x, float y)
{
	D3DXVECTOR3 vRet;
	// 0 ~ 800 - > 0 ~1 -> -1 ~ +1
	vRet.x = x / 800;
	vRet.y = y / 600;
	vRet.x = vRet.x * 2.0f - 1.0f;
	vRet.y = (vRet.y * 2 - 1.0f)*-1.0f;
	return vRet;
}
void    TActor::GenCenter()
{
	m_vCenter.x = 0.0f;
	m_vCenter.y = 0.0f;
	for (int iV = 0; iV < 6; iV++)
	{
		D3DXVECTOR3 vertex = m_VertexList[iV].p;
		m_vCenter.x += vertex.x;
		m_vCenter.y += vertex.y;
	}
	m_vCenter.x /= 6.0f;
	m_vCenter.y /= 6.0f;
}
bool    TActor::SetData(UINT left, UINT top, UINT width, UINT height)
{
	m_rtInit.left = left;
	m_rtInit.right = width;
	m_rtInit.top = top;
	m_rtInit.bottom = height;
	SetVertexData();
	return true;
}
void   TActor::SetVertexData()
{
	D3DXVECTOR3 pos = Gen(m_rtInit.left, m_rtInit.top);
	m_VertexList[0].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	m_VertexList[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	pos = Gen(m_rtInit.left + m_rtInit.right,
		m_rtInit.top);
	m_VertexList[1].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	m_VertexList[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	pos = Gen(m_rtInit.left,
		m_rtInit.top + m_rtInit.bottom);
	m_VertexList[2].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	m_VertexList[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[3] = m_VertexList[2];
	m_VertexList[4] = m_VertexList[1];
	pos = Gen(m_rtInit.left + m_rtInit.right,
		m_rtInit.top + m_rtInit.bottom);
	m_VertexList[5].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	m_VertexList[5].t = D3DXVECTOR2(1.0f, 1.0f);
	m_VertexList[5].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	GenCenter();

}
bool	TActor::Frame(float fGameTimer,float fFps)
{
	return true;
}
bool	TActor::PreRender(ID3D11DeviceContext* pContext)
{
	m_dxObj.PreRender(pContext);
	return true;
}
bool	TActor::Render(ID3D11DeviceContext* pContext)
{
	m_dxObj.Render(pContext);
	return true;
}
bool	TActor::PostRender(ID3D11DeviceContext* pContext)
{
	m_dxObj.PostRender(pContext);
	return true;
}
bool	TActor::Init()
{
	return true;
}
bool	TActor::Release()
{
	m_dxObj.Release();
	return true;
}
TActor::TActor()
{
}


TActor::~TActor()
{
}
