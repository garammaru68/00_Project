#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#pragma comment( lib, "Winmm.lib" )
#pragma comment( lib, "SCoreLib.lib" )
#pragma comment( lib, "fmod_vc.lib" )
#pragma comment( lib, "Msimg32.lib" )

using namespace std;

struct SKeyMap
{
	bool	m_bFront;
	bool	m_bLeft;
	bool	m_bRight;
	bool	m_bBack;
	bool	m_bAttack;
	bool	m_bJump;
	bool	m_bHangul;
	bool	m_bCapsLock;
	bool	m_bNumLock;
};
struct SPoint
{
	float x;
	float y;
};
extern SKeyMap	g_KeyMap;
extern float	g_fGameTimer;
extern float	g_fSecondPerFrame;
extern HWND		g_hWnd;
extern HINSTANCE	g_hInstance;
extern HDC		g_hScreenDC;
extern HDC		g_hOffScreenDC;
extern RECT		g_rtClient;
template<class S>
class SSingleton
{
public:
	static S& GetInstance()
	{
		static S theSingle;
		return theSingle;
	}
};