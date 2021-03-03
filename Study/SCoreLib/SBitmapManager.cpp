//#include "SBitMapManager.h"
//
//bool SBitMapManager::SaveFile(HBitMap hBitMap, const TCHAR* strSaveFile)
//{
//	BitMap BitMap;
//	GeSObject(hBitMap, sizeof(BitMap), &BitMap);
//
//	BitMapINFOHEADER bitHeader;
//	memset(&bitHeader, 0, sizeof(BitMapINFOHEADER));
//	bitHeader.biSize = sizeof(BitMapINFOHEADER);
//	bitHeader.biWidth = BitMap.bmWidth;
//	bitHeader.biHeight = BitMap.bmHeight;
//	bitHeader.biPlanes = 1;
//	bitHeader.biBitCount = BitMap.bmBitsPixel;
//	bitHeader.biCompression = BI_RGB;
//	//bitHeader.biSizeImage = BitMap.bmWidthBytes * BitMap.bmHeight;
//
//	HDC hdc = GetDC(GetDesktopWindow());
//	// biSizeImage 정보 얻기
//	GetDIBits(hdc, hBitMap, 0, BitMap.bmHeight, NULL, (LPBitMapINFO)&bitHeader, DIB_RGB_COLORS);
//	// 비트맵 데이터 얻기
//	LPBYTE lpBits = new BYTE[bitHeader.biSizeImage];
//	GetDIBits(hdc, hBitMap, 0, BitMap.bmHeight, lpBits, (LPBitMapINFO)&bitHeader, DIB_RGB_COLORS);
//	ReleaseDC(GetDesktopWindow(), hdc);
//
//	// 파일 생성 및 저장
//	HANDLE hFile = CreateFile(strSaveFile, GENERIC_WRITE, 0, NULL, 
//		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (hFile == INVALID_HANDLE_VALUE)
//	{
//		delete[] lpBits;
//		return false;
//	}
//	// 파일 헤더 저장
//	BitMapFILEHEADER bmFileHeader;
//	memset(&bmFileHeader, 0, sizeof(BitMapFILEHEADER));
//	bmFileHeader.bfType = ((WORD)('M' << 8) | 'B');
//	bmFileHeader.bfOffBits = sizeof(BitMapFILEHEADER) + sizeof(BitMapINFOHEADER);
//	bmFileHeader.bfSize = bmFileHeader.bfOffBits + bitHeader.biSizeImage;
//
//	DWORD dwWritten;
//	//파일헤더 저장
//	WriteFile(hFile, &bmFileHeader, sizeof(BitMapFILEHEADER), &dwWritten, NULL);
//	//정보헤더 저장
//	WriteFile(hFile, &bitHeader, sizeof(BitMapINFOHEADER), &dwWritten, NULL);
//	//데이터 저장
//	WriteFile(hFile, lpBits, bitHeader.biSizeImage, &dwWritten, NULL);
//	CloseHandle(hFile);
//
//	delete[] lpBits;
//	return true;
//}
//HBitMap SBitMapManager::CreateDIB(
//	int iWidth, int iHeight,
//	LPVOID* pPixelData)
//{
//	BitMapINFO bmi;
//	LPVOID pBits;
//
//	HBitMap hBitMap;
//	memset(&bmi.bmiHeader, 0, sizeof(BitMapINFOHEADER));
//	bmi.bmiHeader.biSize = sizeof(BitMapINFOHEADER);
//	bmi.bmiHeader.biBitCount = 32;
//	bmi.bmiHeader.biWidth = iWidth;
//	bmi.bmiHeader.biHeight = iHeight;
//	bmi.bmiHeader.biPlanes = 1;
//
//	hBitMap = CreateDIBSection(g_hScreenDC, (BitMapINFO*)&bmi,
//		DIB_RGB_COLORS, &pBits, NULL, 0);
//
//	for (int j = 0; j < iHeight; ++j)
//	{
//		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pBits)[j * iWidth];
//		for (int i = 0; i < iWidth; ++i)
//		{
//			pbSrcRGB[0] = 0;
//			pbSrcRGB[1] = 0;
//			pbSrcRGB[2] = 0;
//			pbSrcRGB[3] = 255;
//			pbSrcRGB += 4;
//		}
//	}
//
//	if (pPixelData != nullptr)
//	{
//		*pPixelData = pBits;
//	}
//	return hBitMap;
//}
//SBitMap* SBitMapManager::Load(const TCHAR* filename)
//{
//	TCHAR drive[MAX_PATH] = { 0, };
//	TCHAR dir[MAX_PATH] = { 0, };
//	TCHAR name[MAX_PATH] = { 0, };
//	TCHAR ext[MAX_PATH] = { 0, };
//	_wsplitpath_s(filename, drive, dir, name, ext);
//
//	std::wstring Dir = dir;
//	std::wstring key;
//	std::wstring loadfile;
//	if (Dir.empty())
//	{
//		loadfile = m_szDefaultPath;
//		loadfile += filename;
//		key = filename;
//	}
//	else
//	{
//		loadfile = filename;
//		key = name;
//		key += ext;
//	}
//	SBitMap* pData = GetPtr(key);
//	if (pData != nullptr)
//	{
//		return pData;
//	}
//
//	pData = new SBitMap;
//	pData->Init();
//	if (pData->Load(loadfile))
//	{
//		m_List.insert(make_pair(key, pData));
//		return pData;
//	}
//	delete pData;
//	return nullptr;
//}
//SBitMap* SBitMapManager::CreateLaod(
//	LONG iWidth, LONG iHeight, const TCHAR* filename, int iBitCount)
//{
//	TCHAR drive[MAX_PATH] = { 0, };
//	TCHAR dir[MAX_PATH] = { 0, };
//	TCHAR name[MAX_PATH] = { 0, };
//	TCHAR ext[MAX_PATH] = { 0, };
//	_wsplitpath_s(filename, drive, dir, name, ext);
//
//	std::wstring Dir = dir;
//	std::wstring key;
//	std::wstring loadfile;
//	if (Dir.empty())
//	{
//		loadfile = m_szDefaultPath;
//		loadfile += filename;
//		key = filename;
//	}
//	else
//	{
//		loadfile = filename;
//		key = name;
//		key += ext;
//	}
//	SBitMap* pData = GetPtr(key);
//	if (pData != nullptr)
//	{
//		return pData;
//	}
//
//	pData = new SBitMap;
//	pData->Init();
//	if (pData->CreateDIB(iWidth, iHeight, iBitCount))
//	{
//		m_List.insert(make_pair(key, pData));
//		return pData;
//	}
//	delete pData;
//	return nullptr;
//}
//SBitMap* SBitMapManager::GetPtr(wstring filename)
//{
//	m_iter = m_List.find(filename);
//	if (m_iter == m_List.end())
//	{
//		return nullptr;
//	}
//	return (*m_iter).second;
//}
//bool SBitMapManager::Init()
//{
//	return true;
//}
//bool SBitMapManager::Frame()
//{
//	return true;
//}
//bool SBitMapManager::Render()
//{
//	return true;
//}
//bool SBitMapManager::Release()
//{
//	for (m_iter = m_List.begin();
//		m_iter != m_List.end();
//		m_iter++)
//	{
//		delete (*m_iter).second;
//	}
//	m_List.clear();
//	return true;
//}
//
//SBitMapManager::SBitMapManager()
//{
//	m_szDefaultPath = L"../../data/API/";
//}
//SBitMapManager::~SBitMapManager()
//{
//	Release();
//}