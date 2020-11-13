#pragma once
#include "SWindow.h"
#include "STimer.h"
#include "SInput.h"
#include "SGraphicAPI.h"
#include "SWrite.h"
#include "SSoundManager.h"

class SCore : public SWindow
{
public:
	bool		m_bGameRun;
	SGraphicAPI	m_Graphic;
public:
	virtual bool Init() { return true; }
	virtual bool Frame() { return true; }
	virtual bool Render() { return true; }
	virtual bool PreRender();
	virtual bool PostRender();
	virtual bool Release() { return true; }
private:
	bool GameInit();
	bool GameFrame();
	bool GameRender();
	bool GameRelease();
	bool GameRun();
public:
	bool Run();
};

