#pragma once
#include "SDevice.h"
#include "STimer.h"
#include "SInput.h"
#include "SWrite.h"
//#include "SSoundManager.h"
#include "SDebugCamera.h"

class SCore : public SDevice
{
public:
	bool			m_bGameRun;
	SDebugCamera	m_Camera;
	SCamera*		m_pMainCamera;
	SShapeLine		m_LineShape;
public:
	virtual bool	PreInit();
	virtual bool	Init();
	virtual bool	PostInit();

	virtual bool	PreFrame() {return true;};
	virtual void    CameraFrame();
	virtual bool	Frame() {return true;};
	virtual bool	PostFrame() {return true;};
	virtual bool	Render() {return true;};
	virtual bool	PreRender();
	virtual bool	PostRender();
	virtual bool	Release() {return true;};
	virtual HRESULT DeleteDXResource();
	virtual HRESULT CreateDXResource(UINT w, UINT h);
private:
	bool GameInit();
	bool GameFrame();
	bool GameRender();
	bool GameRelease();
	bool GameRun();
public:
	bool Run();
public:
	SCore();
	virtual ~SCore();
};

