#pragma once
#include "SCore.h"
#include "SVertex.h"
#include <d3dcompiler.h>
#include "WICTextureLoader.h"// ../../include/DirectXTK
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "directxtk.lib")
struct P_VERTEX
{
	SVertex3 p;
	SVertex3 n;
	SVertex4 c;
	SVertex2 t;
	P_VERTEX() {}
	P_VERTEX(SVertex3 p,
		SVertex3 n,
		SVertex4 c,
		SVertex2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};
struct SDataCB
{
	float vColor[4];
	float vTime[4];
};
class Sample : public SCore
{
	SDataCB					m_cbData;
	std::vector<P_VERTEX>	m_VertexList;
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	ID3D11Buffer*			m_pConstantBuffer;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11RasterizerState*	m_pRSSolidBack;
	ID3D11RasterizerState*	m_pRSWireBack;
	ID3D11RasterizerState*	m_pRS;
	ID3D11ShaderResourceView*	m_pTextureSRV;
	ID3D11SamplerState*			m_pWrapLinear;
	D3D11_FILL_MODE			m_FillMode;
	D3D11_CULL_MODE			m_CullMode;
public:
	ID3D11DepthStencilView*  m_pDSV;
	ID3D11DepthStencilState* m_pDSS;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool PreRender() override;
	bool Release() override;
public:
	void CompilerCheck(ID3DBlob* pErrorMsgs);
	void SetRasterizerState();
};

SGAME_RUN;