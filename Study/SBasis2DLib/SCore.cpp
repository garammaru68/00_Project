#include "SCore.h"
bool SCore::GameInit()
{
	m_bGameRun = true;
	g_Timer.Init();	
	g_Input.Init();
	m_Graphic.Init();
	g_Write.Init();
	g_SoundMgr.Init();
	Init();
	return true;
}
bool SCore::GameRelease()
{
	Release();
	g_Timer.Release();
	g_Input.Release();
	g_Write.Release();
	g_SoundMgr.Release();
	m_Graphic.Release();
	return true;
}
bool SCore::GameFrame()
{
	g_Timer.Frame();
	g_Input.Frame();
	m_Graphic.Frame();
	g_SoundMgr.Frame();
	Frame();
	g_Write.Frame();
	return true;
}
bool SCore::PreRender()
{
	m_Graphic.PreRender();
	return true;
}
bool SCore::PostRender()
{
	g_Timer.Render();
	g_Input.Render();
	g_SoundMgr.Render();
	g_Write.Render();
	g_Write.Draw(0, 0, g_Timer.m_szBuffer);
	m_Graphic.PostRender();
	return true;
}
bool SCore::GameRender()
{
	PreRender();
	Render();
	PostRender();
	return true;
}
bool SCore::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}
bool SCore::Run()
{
	GameInit();
	while (m_bGameRun)
	{
		if (MsgProcess() == false)
		{
			m_bGameRun = false;
		}
		else
		{
			GameRun();
		}
	}
	GameRelease();
	return true;
}