#pragma once
#include "SStd.h"
#include "dxtk/WICTextureLoader.h" //../../include/DirectXTK
#include "dxtk/DDSTextureLoader.h"
#include "SimpleMath.h"
#include "SDxState.h"
#include "STextureManager.h"
using namespace DirectX;
using namespace SimpleMath;

struct P_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	P_VERTEX() {}
	P_VERTEX(Vector3 p,
		Vector3 n,
		Vector4 c,
		Vector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};
struct PNCT_VERTEX
{
	Vector3		p;
	Vector3 	n;
	Vector4		c;
	Vector2     t;
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(Vector3		vp,
		Vector3		vn,
		Vector4		vc,
		Vector2     vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};

struct S_SPHERE
{
	Vector3		vCenter;
	float		fRadius;
};
struct S_BOX
{
	// Common
	Vector3		vCenter;
	Vector3		vPos[8];
	// AABB
	Vector3		vMax;
	Vector3		vMin;
	// OBB
	Vector3		vAxis[3];
	float		fExtent[3];
};

struct SDataCB
{
	Matrix  matWorld;
	Matrix  matView;
	Matrix  matProject;
	float vColor[4];
	float vTime[4];
};

namespace SBASIS_CORE_LIB
{
	void		ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);

	HRESULT	CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT D3DX11CompileFromFile(LPCWSTR pSrcFile, CONST D3D_SHADER_MACRO* pDefines, LPD3DINCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2,
		/*ID3DX11ThreadPump* pPump, */ID3DBlob** ppShader, ID3DBlob** ppErrorMsgs, HRESULT* pHResult);
	ID3D11VertexShader* LoadVertexShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11PixelShader*  LoadPixelShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, const char *pFuntionName = 0, bool bBinary = false, ID3DBlob** pRetBlob = nullptr);
	ID3D11GeometryShader*  LoadGeometryShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11HullShader* LoadHullShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11DomainShader*  LoadDomainShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11ComputeShader*  LoadComputeShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);


	ID3D11InputLayout* CreateInputlayout(ID3D11Device*  pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device*  pd3dDevice, void *vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic = false);
	ID3D11Buffer* CreateIndexBuffer(ID3D11Device*  pd3dDevice, void *indices, UINT iNumIndex, UINT iSize, bool bDynamic = false);
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device*  pd3dDevice, void *data, UINT iNumIndex, UINT iSize, bool bDynamic = false);

	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, const TCHAR* strFilePath);
	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, ID3D11DeviceContext*    pContext, const TCHAR* strFilePath);
	ID3D11DepthStencilView* CreateDepthStencilView(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);

	class SDxObject
	{
	public:
		ID3D11Device* g_pd3dDevice;
		ID3DBlob*	  m_pVSObj;
		UINT		  m_iTopology;
		C_STR		  m_szVertexShader;
		C_STR		  m_szPixelShader;
	public:
		Vector3		m_vLook;
		Vector3		m_vUp;
		Vector3		m_vRight;
		Vector3		m_vPos = { 0,0,0 };
		Vector3		m_vTarget = { 0,0,0 };

		Matrix    m_matScale;
		Matrix    m_matRotation;
		Matrix    m_matWorld;
		Matrix    m_matView;
		Matrix    m_matProj;
	public:
		SDataCB					m_cbData;
		std::vector<PNCT_VERTEX>	m_VertexList;
		std::vector<DWORD>		m_IndexList;
		ID3D11Buffer*			m_pVertexBuffer;
		ID3D11Buffer*			m_pIndexBuffer;
		ID3D11Buffer*			m_pConstantBuffer;
		ID3D11InputLayout*		m_pInputLayout;
		ID3D11VertexShader*		m_pVertexShader;
		ID3D11PixelShader*		m_pPixelShader;
		STexture*				m_pTexture;
	public:
		virtual bool	Init();
		virtual bool	Frame();
		virtual bool	SetMatrix(Matrix* pWorld,
			Matrix* pView,
			Matrix* pProj);
		virtual bool    Update(ID3D11DeviceContext*	pd3dContext);
		virtual bool	PreRender(ID3D11DeviceContext* pd3dContext);
		virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
		virtual bool	PostRender(ID3D11DeviceContext* pd3dContext);
		virtual bool	Release();
		virtual bool    CreateVertexData();
		virtual bool    CreateIndexData();
		virtual bool    CreateVertexBuffer();
		virtual bool    CreateIndexBuffer();
		virtual bool    CreateConstantBuffer();
		virtual bool	LoadShader(T_STR szVS, T_STR szPS);
		virtual bool	CreateInputLayout();
		virtual bool	LoadTexture(T_STR szTex);
		virtual bool	Create(ID3D11Device* pDevice,
			T_STR szVS, T_STR szPS,
			T_STR	szTex);
		void	CompilerCheck(ID3DBlob* pErrorMsgs);
	public:
		SDxObject();
		virtual ~SDxObject();
	};
}