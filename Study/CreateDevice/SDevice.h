#pragma once
#include "SStd.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
class SDevice
{
public:
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*	m_pd3dContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRenderTargetView;
public:
	bool	Init();
	bool	Frame();
	bool	PreRender();
	bool	Render();
	bool	PostRender();
	bool	Release();
};