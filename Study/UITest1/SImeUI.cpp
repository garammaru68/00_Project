#include "SImeUI.h"
static HKL g_hklCurrent = 0;

static LPTSTR g_aszIndicator[] =
{
	TEXT("A"),
#ifdef UNICODE
	L"\x7B80",
	L"\x7E41",
	L"\xac00",
	L"\x3042",
#else
	"\xd6\xd0",
	"\xa4\xa4",
	"\xb0\xa1",
	"\x82\xa0",
#endif
};
void SImeUI::CheckInputLocale()
{
	static HKL hklPrev = 0;
	g_hklCurrent = GetKeyboardLayout(0);
	if (hklPrev == g_hklCurrent)
	{
		return;
	}
	hklPrev = g_hklCurrent;
	switch (GETPRIMLANG())
	{
		// Simplified Chinese
	case LANG_CHINESE:
		switch (GETSUBLANG())
		{
		case SUBLANG_CHINESE_SIMPLIFIED:
			m_pszIndicatior = g_aszIndicator[INDICATOR_CHS];
			break;
		case SUBLANG_CHINESE_TRADITIONAL:
			m_pszIndicatior = g_aszIndicator[INDICATOR_CHT];
			break;
		default:	// unsupported sub-language
			m_pszIndicatior = g_aszIndicator[INDICATOR_NON_IME];
			break;
		}
		break;
		// Korean
	case LANG_KOREAN:
		m_pszIndicatior = g_aszIndicator[INDICATOR_KOREAN];
		break;
		// Japanese
	case LANG_JAPANESE:
		m_pszIndicatior = g_aszIndicator[INDICATOR_JAPANESE];
		break;
	default:
		m_pszIndicatior = g_aszIndicator[INDICATOR_NON_IME];
	}

}

bool SImeUI::ImeUi_Initialize(HWND hwnd, bool bDisable)
{
	if (m_bInitialized)
	{
		return true;
	}
	m_hWndMain = hwnd;

	m_hImmDll = LoadLibraryA("imm32.dll");
	if (!m_hImmDll)
	{
		return false;
	}

	m_bInitialized = true;

	m_himcOrg = ImmGetContext(m_hWndMain);
	if (!m_himcOrg)
	{
		m_bInitialized = bDisable;
		return false;
	}

	CheckInputLocale();

	ImmNotifyIME(m_himcOrg, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	ImmReleaseContext(m_hWndMain, m_himcOrg);
	return true;
}

void SImeUI::ImeUi_Uninitialize()
{
	if (!m_bInitialized)
	{
		return;
	}

	if (m_hWndMain)
	{
		ImmAssociateContext(m_hWndMain, m_himcOrg);
	}
	m_hWndMain = NULL;
	m_himcOrg = NULL;
	if (m_hImmDll)
	{
		FreeLibrary(m_hImmDll);
		m_hImmDll = NULL;
	}

	m_bInitialized = false;
}
void SImeUI::Initialize(HWND hWnd)
{
	ImeUi_Initialize(hWnd);
}
//--------------------------------------------------------------------------------------
void SImeUI::Uninitialize()
{
	ImeUi_Uninitialize();
}
BOOL SImeUI::OnImeComposition(WPARAM wParam, LPARAM lParam)
{
	HIMC	hIMC;

	hIMC = ImmGetContext(m_hWndMain);
	if (hIMC == NULL) 	return TRUE;

	// 조합완성
	if (lParam & GCS_RESULTSTR)
	{
		TCHAR	szBackup[256] = { 0, };
		// Get result string
		m_nComSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, (LPVOID)szBackup, sizeof(TCHAR) * 256);
		if (m_nComSize > 0)
		{
			m_CompString += szBackup;
		}
	}
	// 조합중
	else if (lParam & GCS_COMPSTR)
	{
		TCHAR	szBackup[256] = { 0, };
		// Get composition string
		m_nComSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, (LPVOID)szBackup, sizeof(TCHAR) * 256);
		m_CompWord = szBackup;
	}
	else
	{
		m_nComSize = 0;
	}
	ImmReleaseContext(m_hWndMain, hIMC);
	return TRUE;
}
void SImeUI::ResetCompositionString()
{
	m_CompString.clear();
}
int SImeUI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bInitialized)		return -1;

	switch (uMsg)
	{
	case WM_IME_STARTCOMPOSITION: // ㅇ
		m_CompWord.clear();
		return 1;
	case WM_IME_ENDCOMPOSITION: // 우 리
		m_CompWord = wParam;
		return 0;
	// 편집중에 있는 스트링을 얻는다.
	case WM_IME_COMPOSITION: // 리 
	{
		OnImeComposition(wParam, lParam);
	}
	return 1;
	//case WM_IME_CHAR:
	//	m_CompString += wParam;
	//	return 1;

	case WM_CHAR:
	{
		switch ((WCHAR)wParam)
		{
			// Backspace
		case VK_BACK:
		{
			if (!m_CompString.empty())
			{
				T_ITOR it = m_CompString.begin() + (m_CompString.size() - 1);
				m_CompString.erase(it);
			}
			break;
		}
		case VK_RETURN:
			ResetCompositionString();
			break;

		default:
		{
			m_CompString += wParam;
		}
		}
		return 1;
	}
	case WM_IME_NOTIFY:
	{
		if (wParam == IMN_SETCONVERSIONMODE)
		{
			HIMC hIMC;
			DWORD dwConversion;
			DWORD dwSentence;

			hIMC = ::ImmGetContext(hWnd);
			ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence);

			if (dwConversion != m_dwConversion)
			{
				if (dwConversion & IME_CMODE_NATIVE)     // 한글 mode 
				{
					if (dwConversion & IME_CMODE_FULLSHAPE)
						MessageBox(hWnd, L"한글 완성형 모드", L"IME Status", MB_OK);
					else
						MessageBox(hWnd, L"한글 조합형 모드", L"IME Status", MB_OK);
				}
				else									// 영문 mode
				{

					MessageBox(hWnd, L"영문 모드", L"IME Status", MB_OK);
				}
				ImmReleaseContext(hWnd, hIMC);
				m_dwConversion = dwConversion;
			}
		}
	}break;
	}
	return -1;
}
SImeUI::SImeUI()
{
}


SImeUI::~SImeUI()
{
}
