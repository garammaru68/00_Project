#include "TButton.h"
D3DXVECTOR3  TButton::Gen(float x, float y)
{
	D3DXVECTOR3 vRet = TActor::Gen(x,y);
	//vRet.x = vRet.x * (800 * 0.5f);
	//vRet.y = vRet.y * (600 * 0.5f);
	return vRet;
}
void   TButton::SetVertexData()
{
	//	float fScaleX0, float fScaleX1,	0.1f, 0.1f, 
	//	float fScaleY0, float fScaleY1,	0.1f, 0.1f,
	//	float fOffsetU0, float fOffsetU1	0.2f, 0.2f,
	//	float fOffsetV0, float fOffsetV1	0.33f, 0.33f
	// m_rtInit.left = 10; m_rtInit.top = 10; 
	// m_rtInit.right = m_rcWindowClient.right - 20;
	// m_rtInit.bottom = m_rcWindowClient.bottom - 20
	m_cpPoint[0] = { m_rtInit.left*1.0f, m_rtInit.top*1.0f }; // 10,10
	m_cpPoint[1] = { m_rtInit.left + m_rtInit.right * m_tOffset.fScaleX0, m_rtInit.top*1.0f }; // 79,10
	m_cpPoint[2] = { m_rtInit.left + m_rtInit.right * (1.0f-m_tOffset.fScaleX1), m_rtInit.top*1.0f }; // 711,10
	m_cpPoint[3] = { m_rtInit.left + m_rtInit.right*1.0f, m_rtInit.left*1.0f }; // 790, 10

	m_cpPoint[4] = { m_cpPoint[0].x, m_rtInit.top + m_rtInit.bottom * m_tOffset.fScaleY0 };
	m_cpPoint[5] = { m_cpPoint[1].x, m_rtInit.top + m_rtInit.bottom * m_tOffset.fScaleY0 };
	m_cpPoint[6] = { m_cpPoint[2].x, m_rtInit.top + m_rtInit.bottom * m_tOffset.fScaleY0 };
	m_cpPoint[7] = { m_cpPoint[3].x, m_rtInit.top + m_rtInit.bottom * m_tOffset.fScaleY0 };

	m_cpPoint[8] = { m_cpPoint[0].x, m_rtInit.top + m_rtInit.bottom * (1.0f-m_tOffset.fScaleY1) };
	m_cpPoint[9] = { m_cpPoint[1].x, m_rtInit.top + m_rtInit.bottom * (1.0f - m_tOffset.fScaleY1) };
	m_cpPoint[10] = { m_cpPoint[2].x, m_rtInit.top + m_rtInit.bottom * (1.0f - m_tOffset.fScaleY1) };
	m_cpPoint[11] = { m_cpPoint[3].x, m_rtInit.top + m_rtInit.bottom * (1.0f - m_tOffset.fScaleY1) };

	m_cpPoint[12] = { m_cpPoint[0].x, m_rtInit.top + m_rtInit.bottom*1.0f };
	m_cpPoint[13] = { m_cpPoint[1].x, m_rtInit.top + m_rtInit.bottom*1.0f };
	m_cpPoint[14] = { m_cpPoint[2].x, m_rtInit.top + m_rtInit.bottom*1.0f };
	m_cpPoint[15] = { m_cpPoint[3].x, m_rtInit.top + m_rtInit.bottom*1.0f };

	

	m_Actor[0].SetData(m_cpPoint[0].x, m_cpPoint[0].y, m_cpPoint[1].x- m_cpPoint[0].x, m_cpPoint[4].y- m_cpPoint[0].y);
	m_Actor[0].m_VertexList[1].t.x = m_tOffset.fOffsetU0; 	
	m_Actor[0].m_VertexList[1].t.y = 0.0f;
	m_Actor[0].m_VertexList[2].t.x = 0.0f;	
	m_Actor[0].m_VertexList[2].t.y = m_tOffset.fOffsetV0;
	m_Actor[0].m_VertexList[3] = m_Actor[0].m_VertexList[2];
	m_Actor[0].m_VertexList[4] = m_Actor[0].m_VertexList[1];
	m_Actor[0].m_VertexList[5].t.x = m_tOffset.fOffsetU0;
	m_Actor[0].m_VertexList[5].t.y = m_tOffset.fOffsetV0;

	m_Actor[1].SetData( m_cpPoint[1].x, m_cpPoint[1].y,	m_cpPoint[2].x- m_cpPoint[1].x,	m_cpPoint[6].y- m_cpPoint[2].y);
	m_Actor[1].m_VertexList[0].t.x = m_tOffset.fOffsetU0;
	m_Actor[1].m_VertexList[0].t.y = 0.0f;
	m_Actor[1].m_VertexList[1].t.x = 1.0f- m_tOffset.fOffsetU0;
	m_Actor[1].m_VertexList[1].t.y = 0.0f;
	m_Actor[1].m_VertexList[2].t.x = m_Actor[1].m_VertexList[0].t.x;
	m_Actor[1].m_VertexList[2].t.y = m_tOffset.fOffsetV0;
	m_Actor[1].m_VertexList[3] = m_Actor[1].m_VertexList[2];
	m_Actor[1].m_VertexList[4] = m_Actor[1].m_VertexList[1];
	m_Actor[1].m_VertexList[5].t.x = m_Actor[1].m_VertexList[1].t.x;
	m_Actor[1].m_VertexList[5].t.y = m_Actor[1].m_VertexList[2].t.y;

	m_Actor[2].SetData(	m_cpPoint[2].x, m_cpPoint[2].y, m_cpPoint[3].x- m_cpPoint[2].x, m_cpPoint[6].y - m_cpPoint[2].y);
	m_Actor[2].m_VertexList[0].t.x = 1.0f - m_tOffset.fOffsetU0;
	m_Actor[2].m_VertexList[0].t.y = 0.0f;
	m_Actor[2].m_VertexList[1].t.x = 1.0f;
	m_Actor[2].m_VertexList[1].t.y = 0.0f;
	m_Actor[2].m_VertexList[2].t.x = m_Actor[2].m_VertexList[0].t.x;
	m_Actor[2].m_VertexList[2].t.y = m_tOffset.fOffsetV0;
	m_Actor[2].m_VertexList[3] = m_Actor[2].m_VertexList[2];
	m_Actor[2].m_VertexList[4] = m_Actor[2].m_VertexList[1];
	m_Actor[2].m_VertexList[5].t.x = m_Actor[2].m_VertexList[1].t.x;
	m_Actor[2].m_VertexList[5].t.y = m_Actor[2].m_VertexList[2].t.y;
#pragma endregion
	/////////////////////////////////////////////////////
#pragma region 3~5
	m_Actor[3].SetData(m_cpPoint[4].x, m_cpPoint[4].y,	m_cpPoint[1].x - m_cpPoint[0].x, m_cpPoint[8].y- m_cpPoint[4].y);
	m_Actor[3].m_VertexList[0].t = m_Actor[0].m_VertexList[2].t;
	m_Actor[3].m_VertexList[1].t = m_Actor[0].m_VertexList[5].t;
	m_Actor[3].m_VertexList[2].t.x = 0.0f;
	m_Actor[3].m_VertexList[2].t.y = 1.0f-(m_tOffset.fOffsetV0- m_tOffset.fOffsetV1);
	m_Actor[3].m_VertexList[3] = m_Actor[3].m_VertexList[2];
	m_Actor[3].m_VertexList[4] = m_Actor[3].m_VertexList[1];
	m_Actor[3].m_VertexList[5].t.x = m_Actor[3].m_VertexList[1].t.x;
	m_Actor[3].m_VertexList[5].t.y = m_Actor[3].m_VertexList[2].t.y;

	m_Actor[4].SetData(m_cpPoint[5].x, m_cpPoint[5].y, m_cpPoint[2].x - m_cpPoint[1].x, m_cpPoint[8].y - m_cpPoint[4].y);
	m_Actor[4].m_VertexList[0].t = m_Actor[3].m_VertexList[1].t;
	m_Actor[4].m_VertexList[1].t.x = m_Actor[1].m_VertexList[1].t.x;
	m_Actor[4].m_VertexList[1].t.y = m_tOffset.fOffsetV0;
	m_Actor[4].m_VertexList[2].t.x = m_tOffset.fOffsetU0;
	m_Actor[4].m_VertexList[2].t.y = 1.0f - (m_tOffset.fOffsetV0 - m_tOffset.fOffsetV1);
	m_Actor[4].m_VertexList[3] = m_Actor[4].m_VertexList[2];
	m_Actor[4].m_VertexList[4] = m_Actor[4].m_VertexList[1];
	m_Actor[4].m_VertexList[5].t.x = m_Actor[4].m_VertexList[1].t.x;
	m_Actor[4].m_VertexList[5].t.y = m_Actor[4].m_VertexList[2].t.y;

	m_Actor[5].SetData(m_cpPoint[6].x, m_cpPoint[6].y, m_cpPoint[3].x - m_cpPoint[2].x, m_cpPoint[8].y - m_cpPoint[4].y);
	m_Actor[5].m_VertexList[0].t =	m_Actor[4].m_VertexList[1].t;
	m_Actor[5].m_VertexList[1].t.x = 1.0f;
	m_Actor[5].m_VertexList[1].t.y = m_tOffset.fOffsetV0;
	m_Actor[5].m_VertexList[2].t = m_Actor[4].m_VertexList[5].t;
	m_Actor[5].m_VertexList[3] = m_Actor[5].m_VertexList[2];
	m_Actor[5].m_VertexList[4] = m_Actor[5].m_VertexList[1];
	m_Actor[5].m_VertexList[5].t.x = m_Actor[5].m_VertexList[1].t.x;
	m_Actor[5].m_VertexList[5].t.y = m_Actor[5].m_VertexList[2].t.y;
#pragma endregion 3~5
#pragma region 6~9
	m_Actor[6].SetData(m_cpPoint[8].x, m_cpPoint[8].y,	m_cpPoint[1].x- m_cpPoint[0].y, m_cpPoint[12].y - m_cpPoint[8].y);
	m_Actor[6].m_VertexList[0].t = m_Actor[3].m_VertexList[2].t;
	m_Actor[6].m_VertexList[1].t = m_Actor[3].m_VertexList[5].t;
	m_Actor[6].m_VertexList[2].t.x = 0.0f;
	m_Actor[6].m_VertexList[2].t.y = 1.0f;
	m_Actor[6].m_VertexList[3] = m_Actor[6].m_VertexList[2];
	m_Actor[6].m_VertexList[4] = m_Actor[6].m_VertexList[1];
	m_Actor[6].m_VertexList[5].t.x = m_Actor[6].m_VertexList[1].t.x;
	m_Actor[6].m_VertexList[5].t.y = m_Actor[6].m_VertexList[2].t.y;

	m_Actor[7].SetData(m_cpPoint[9].x, m_cpPoint[9].y, m_cpPoint[2].x - m_cpPoint[1].x, m_cpPoint[12].y - m_cpPoint[8].y);
	m_Actor[7].m_VertexList[0].t = m_Actor[4].m_VertexList[3].t;
	m_Actor[7].m_VertexList[1].t = m_Actor[4].m_VertexList[5].t;
	m_Actor[7].m_VertexList[2].t.x = m_tOffset.fOffsetU0;
	m_Actor[7].m_VertexList[2].t.y = 1.0f;
	m_Actor[7].m_VertexList[3] = m_Actor[7].m_VertexList[2];
	m_Actor[7].m_VertexList[4] = m_Actor[7].m_VertexList[1];
	m_Actor[7].m_VertexList[5].t.x = m_Actor[7].m_VertexList[1].t.x;
	m_Actor[7].m_VertexList[5].t.y = m_Actor[7].m_VertexList[2].t.y;

	m_Actor[8].SetData(m_cpPoint[10].x, m_cpPoint[10].y, m_cpPoint[3].x - m_cpPoint[2].x, m_cpPoint[12].y - m_cpPoint[8].y);
	m_Actor[8].m_VertexList[0].t = m_Actor[5].m_VertexList[3].t;
	m_Actor[8].m_VertexList[1].t = m_Actor[5].m_VertexList[5].t;
	m_Actor[8].m_VertexList[2].t = m_Actor[7].m_VertexList[5].t;
	m_Actor[8].m_VertexList[3] = m_Actor[8].m_VertexList[2];
	m_Actor[8].m_VertexList[4] = m_Actor[8].m_VertexList[1];
	m_Actor[8].m_VertexList[5].t.x = m_Actor[8].m_VertexList[1].t.x;
	m_Actor[8].m_VertexList[5].t.y = m_Actor[8].m_VertexList[2].t.y;
	/////////////////////////////////////////////
#pragma endregion 3~5

	for (int iRect = 0; iRect < 9; iRect++)
	{
		for (int iV = 0; iV < 6; iV++)
		{
			m_tVertexList.push_back(m_Actor[iRect].m_VertexList[iV]);
		}
	}
}
void   TButton::MoveX(float fDis)
{
	for (int iV = 0; iV < 6; iV++)
	{
		m_VertexList[iV].p.x += fDis;
	}
	GenCenter();
}
void   TButton::MoveY(float fDis)
{
	for (int iV = 0; iV < 6; iV++)
	{
		m_VertexList[iV].p.y += fDis;
	}
	GenCenter();
}
bool    TButton::SetData(RECT rt,	float fScaleX0, float fScaleX1, 
												float fScaleY0, float fScaleY1, 
												float fOffsetU0, float fOffsetU1,
												float fOffsetV0, float fOffsetV1)
{
	m_rtInit = rt;
	m_tOffset.fScaleX0 = fScaleX0;
	m_tOffset.fScaleX1 = fScaleX1;
	m_tOffset.fScaleY0 = fScaleY0;
	m_tOffset.fScaleY1 = fScaleY1;
	m_tOffset.fOffsetU0 = fOffsetU0;
	m_tOffset.fOffsetU1 = fOffsetU1;
	m_tOffset.fOffsetV0 = fOffsetV1;
	m_tOffset.fOffsetV0 = fOffsetV1;
	SetVertexData();	
	return true;
}

bool    TButton::Create(
	ID3D11Device* pd3dDevice,
	const TCHAR* pVsFile,
	const TCHAR* pPsFile,
	const TCHAR* pTexFile)
{
	m_dxObj.CreateVertexBuffer(	pd3dDevice,	&m_tVertexList.at(0), m_tVertexList.size());
	m_dxObj.CreateConstantBuffer(pd3dDevice, 
		&m_ActorCB, 
		sizeof(TActorCB));
	m_dxObj.CreateVertexShader(pd3dDevice,pVsFile);
	m_dxObj.CreatePixelShader(pd3dDevice, pPsFile);
	m_dxObj.CreateLayout(pd3dDevice, m_dxObj.m_pVSBlob);
	m_dxObj.LoadTexture(pd3dDevice, pTexFile);
	return true;
}
bool TButton::Init()
{
	return true;
}
bool TButton::Frame(float fGameTimer, float fFps)
{	
	m_ActorCB.m_matWorld.RoationZ(fGameTimer);
	D3DXVECTOR3 vEye = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	D3DXMatrixLookAtLH((D3DXMATRIX*)&m_ActorCB.m_matView, &vEye,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixOrthoLH((D3DXMATRIX*)&m_ActorCB.m_matProj,
		2, 2, 0.0f, 100.0f);

	m_ActorCB.m_matWorld.Transpose();
	m_ActorCB.m_matView.Transpose();
	m_ActorCB.m_matProj.Transpose();
	return true;
}
bool	TButton::Render(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_dxObj.m_pConstantBuffer,
		 0, NULL, &m_ActorCB, 0,0);
	m_dxObj.PreRender(pContext);
	PostRender(pContext);
	return true;
}
bool	TButton::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(m_tVertexList.size(), 0);
	return true;
}
bool   TButton::Release()
{
	TActor::Release();
	for (int iRect = 0; iRect < 9; iRect++)
	{
		m_Actor[iRect].Release();
	}
	return true;
}
TButton::TButton()
{
}


TButton::~TButton()
{
}
