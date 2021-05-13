#pragma once
#include "SCore.h"
#include "SDxObject.h"
#include "SPlaneRect.h"

#pragma comment( lib, "IMM32.lib" )
using namespace DX;
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	Vector3 p;
	Vector4 c;
	Vector2 t;
};

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
class Sample : public SCore
{
public:
	SDataCB			m_cbData;
	std::vector<SControlUI*>	m_pUIList;
	SControlUI*					m_pSelectPlane;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV[3];
	SSceneUI						m_Scene;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	//bool		PlaneRender(Matrix matWorld, Vector4 vColor);
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	SControlUI* AddRect(TUI_TYPE type);
	SControlUI* SelectRect();
	int		 WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	Sample(void);
	virtual ~Sample(void);
};
