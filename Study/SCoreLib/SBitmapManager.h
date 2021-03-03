//#pragma once
//#include "SBitmap.h"
//
//class SBitmapManager : public SSingleton<SBitmapManager>
//{
//public:
//	friend class SSingleton<SBitmapManager>;
//private:
//	std::map<wstring, SBitmap*>				m_List;
//	std::map<wstring, SBitmap*>::iterator	m_iter;
//	std::wstring							m_szDefaultPath;
//public:
//	SBitmap*	CreateLaod(LONG iWidth, LONG iHeight, const TCHAR* filename, int iBitCount = 24);
//	SBitmap*	Load(const TCHAR* filename);
//	SBitmap*	GetPtr(wstring filename);
//	static HBITMAP	CreateDIB(int w, int h, LPVOID* pPixelData = nullptr);
//	static bool		SaveFile(HBITMAP hBitmap, const TCHAR* strSaveFile);
//	bool	Init();
//	bool	Frame();
//	bool	Render();
//	bool	Release();
//private:
//	SBitmapManager();
//public:
//	~SBitmapManager();
//};
//#define g_BitmapMgr SBitmapManager::GetInstance()
