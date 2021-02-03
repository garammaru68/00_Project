#pragma once
#include "SCore.h"
#include "SVertex.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
struct P_VERTEX
{
	SVertex3 p;
	SVertex3 n;
	P_VERTEX() {}
	P_VERTEX(SVertex3 p, SVertex3 n = { 0,0,0 })
	{
		this->p = p;
		this->n = n;
	}
};

class Sample : public SCore
{
	std::vector<P_VERTEX>	m_VertexList;
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
public:
	bool Init() override;
	bool Render() override;
	bool Release() override;
public:
	void CompilerCheck(ID3DBlob* pErrorMsgs);
};
SGAME_RUN;