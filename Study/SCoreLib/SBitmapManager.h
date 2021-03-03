//#pragma once
//#include "SBitMap.h"
//
//class SBitMapManager : public SSingleton<SBitMapManager>
//{
//public:
//	friend class SSingleton<SBitMapManager>;
//private:
//	std::map<wstring, SBitMap*>				m_List;
//	std::map<wstring, SBitMap*>::iterator	m_iter;
//	std::wstring							m_szDefaultPath;
//public:
//	SBitMap*	CreateLaod(LONG iWidth, LONG iHeight, const TCHAR* filename, int iBitCount = 24);
//	SBitMap*	Load(const TCHAR* filename);
//	SBitMap*	GetPtr(wstring filename);
//	static HBitMap	CreateDIB(int w, int h, LPVOID* pPixelData = nullptr);
//	static bool		SaveFile(HBitMap hBitMap, const TCHAR* strSaveFile);
//	bool	Init();
//	bool	Frame();
//	bool	Render();
//	bool	Release();
//private:
//	SBitMapManager();
//public:
//	~SBitMapManager();
//};
//#define g_BitMapMgr SBitMapManager::GetInstance()
