#pragma once
#include "SBasisStd.h"

#if NDEBUG
#pragma comment (lib, "SBasis2DLib_R.lib")
#else
#pragma comment (lib, "SBasis2DLib_D.lib")
#endif

using namespace std;

namespace SBASIS_CORE_LIB
{
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

	extern SKeyMap	g_KeyMap;
	extern float	g_fGameTimer;
	extern float	g_fSecondPerFrame;
	extern HWND		g_hWnd;
	extern HINSTANCE	g_hInstance;
	extern HDC		g_hScreenDC;
	extern HDC		g_hOffScreenDC;
	extern RECT		g_rtClient;
	extern bool		g_bActive;
}
using namespace SBASIS_CORE_LIB;