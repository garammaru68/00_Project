//#include "SBitMap.h"
//
//bool SBitMap::Init()
//{
//	m_BlendFunction.BlendOp = AC_SRC_OVER;
//	m_BlendFunction.BlendFlags = 0;
//	m_BlendFunction.SourceConstantAlpha = 255;
//	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
//
//	return true;
//}
//HBitMap SBitMap::LoadDIB(wstring filename)
//{
//	HANDLE hFile = CreateFile(filename.c_str(),
//		GENERIC_READ, 0, NULL,
//		OPEN_EXISTING,
//		FILE_ATTRIBUTE_NORMAL, NULL);
//	if (hFile == INVALID_HANDLE_VALUE)
//	{
//		return false;
//	}
//	BitMapFILEHEADER header;
//	DWORD dwRead;
//	BOOL bRet = ReadFile(hFile, &header, sizeof(BitMapFILEHEADER),
//		&dwRead, NULL);
//
//	DWORD dwDataSize = header.bfOffBits - sizeof(BitMapFILEHEADER);
//
//	BitMapINFO* pBitInfo = (BitMapINFO*)malloc(dwDataSize);
//
//	bRet = ReadFile(hFile, pBitInfo, dwDataSize, &dwRead, NULL);
//
//	PVOID pRaster = 0;
//	m_hBitMap = CreateDIBSection(g_hScreenDC, pBitInfo, DIB_RGB_COLORS, 
//								(LPVOID*)(&m_pRaster), NULL, 0);
//
//	ReadFile(hFile, m_pRaster, 
//			header.bfSize - header.bfOffBits,
//			&dwRead, NULL);
//
//	free(pBitInfo);
//	CloseHandle(hFile);
//	return m_hBitMap;
//}
//bool SBitMap::Load(wstring filename)
//{
//	//m_hBitMap = (HBitMap)LoadImage(g_hInstance,
//	//	filename.c_str(),
//	//	IMAGE_BitMap,
//	//	0,
//	//	0,
//	//	LR_DEFAULTSIZE | LR_LOADFROMFILE);
//	m_hBitMap = LoadDIB(filename.c_str());
//	if (m_hBitMap == NULL)
//	{
//		return false;
//	}
//	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
//	SelecSObject(m_hMemDC, m_hBitMap);
//
//	COLORREF bkColor = RGB(255, 0, 0);
//	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
//	SelecSObject(m_hMemDC, m_hbrRed);
//
//	GeSObject(m_hBitMap, sizeof(BitMap), &m_BitMapInfo);
//	if (m_BitMapInfo.bmBitsPixel == 32)
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
//	}
//	else
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
//	}
//	return true;
//}
//bool SBitMap::CreateDIB(LONG iWidth, LONG iHeight, int iBitCount)
//{
//	BitMapINFO bmi;
//	memset(&bmi.bmiHeader, 0, sizeof(BitMapINFOHEADER));
//	bmi.bmiHeader.biSize = sizeof(BitMapINFOHEADER);
//	bmi.bmiHeader.biBitCount = iBitCount;
//	bmi.bmiHeader.biWidth = iWidth;
//	bmi.bmiHeader.biHeight = iHeight;
//	bmi.bmiHeader.biPlanes = 1;
//
//	m_hBitMap = CreateDIBSection(g_hScreenDC, (BitMapINFO*)&bmi,
//		DIB_RGB_COLORS, (LPVOID*)&m_pRaster, NULL, 0);
//
//	//for (int j = 0; j < iHeight; ++j)
//	//{
//	//	LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)m_pRaster)[j * iWidth];
//	//	for (int i = 0; i < iWidth; ++i)
//	//	{
//	//		pbSrcRGB[0] = 255;
//	//		pbSrcRGB[1] = 0;
//	//		pbSrcRGB[2] = 0;
//	//		pbSrcRGB[3] = 128;
//	//		pbSrcRGB += 4;
//	//	}
//	//}
//	
//	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
//	SelecSObject(m_hMemDC, m_hBitMap);
//
//	COLORREF bkColor = RGB(255, 0, 255);
//	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
//	SelecSObject(m_hMemDC, m_hbrRed);
//
//	GeSObject(m_hBitMap, sizeof(BitMap), &m_BitMapInfo);
//	if (m_BitMapInfo.bmBitsPixel == 32)
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
//	}
//	else
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
//	}
//	return true;
//}
//bool SBitMap::Frame()
//{
//	return true;
//}
//bool SBitMap::Release()
//{
//	DeleteObject(m_hBitMap);
//	ReleaseDC(g_hWnd, m_hMemDC);
//	return true;
//}
//bool SBitMap::Render()
//{
//	BitBlt( g_hOffScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom, 
//			m_hMemDC, 0, 0, SRCCOPY );
//	return true;
//}
//bool SBitMap::Draw(RECT desk, RECT src, DWORD op, int iType)
//{
//	/*BitBlt( g_hOffScreenDC, desk.left, desk.top, desk.right, desk.bottom,
//			m_hMemDC, src.left, src.top, op );*/
//
//	switch (iType)
//	{
//	//ÁÂ¿ì
//	case 1: {
//		StretchBlt( g_hOffScreenDC, desk.left + desk.right, desk.top, -desk.right, desk.bottom,
//					m_hMemDC, src.left, src.top, src.right, src.bottom, op );
//	}break;
//	//»óÇÏ
//	case 2: {
//		StretchBlt( g_hOffScreenDC, desk.left, desk.top + desk.bottom, desk.right, -desk.bottom,
//					m_hMemDC, src.left, src.top, src.right, src.bottom, op );
//	}break;
//	//ÁÂ¿ì»óÇÏ
//	case 3: {
//		StretchBlt( g_hOffScreenDC, desk.left + desk.right, desk.top + desk.bottom, -desk.right, -desk.bottom,
//					m_hMemDC, src.left, src.top, src.right, src.bottom, op );
//	}break;
//	default: {
//		StretchBlt( g_hOffScreenDC, desk.left, desk.top, desk.right, desk.bottom,
//					m_hMemDC, src.left, src.top, src.right, src.bottom, op );
//	}break;
//	}
//	return true;
//}
//bool SBitMap::DrawColorKey(RECT desk, RECT src, DWORD color)
//{
//	BOOL bRet = TransparentBlt( g_hOffScreenDC, desk.left, desk.top, desk.right, desk.bottom,
//								m_hMemDC, src.left, src.top, src.right, src.bottom, color );
//	return bRet;
//}
//bool SBitMap::DrawColorKey(LONG x, LONG y, DWORD color)
//{
//	RECT desk = { x, y, m_BitMapInfo.bmWidth, m_BitMapInfo.bmHeight };
//	RECT src = { 0, 0, m_BitMapInfo.bmWidth, m_BitMapInfo.bmHeight };
//
//	BOOL bRet = TransparentBlt( g_hOffScreenDC, desk.left, desk.top, desk.right, desk.bottom,
//								m_hMemDC, src.left, src.top, src.right, src.bottom, color );
//	return bRet;
//}
//bool SBitMap::DrawAlphaBlend(LONG x, LONG y, BYTE alpha)
//{
//	RECT desk = { x, y, m_BitMapInfo.bmWidth, m_BitMapInfo.bmHeight };
//	RECT src = { 0, 0, m_BitMapInfo.bmWidth, m_BitMapInfo.bmHeight };
//	m_BlendFunction.BlendOp = AC_SRC_OVER;
//	m_BlendFunction.BlendFlags = 0;
//	m_BlendFunction.SourceConstantAlpha = alpha;
//	//(cosf(g_fGameTimer) * 0.5f + 0.5f) * 255;
//	if (m_BitMapInfo.bmBitsPixel == 32)
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
//	}
//	else
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
//	}
//
//	AlphaBlend( g_hOffScreenDC, desk.left, desk.top, desk.right, desk.bottom,
//				m_hMemDC, src.left, src.top, src.right, src.bottom, m_BlendFunction );
//	return true;
//}
//bool SBitMap::DrawAlphaBlend(RECT desk, RECT src, BYTE alpha)
//{
//	m_BlendFunction.BlendOp = AC_SRC_OVER;
//	m_BlendFunction.BlendFlags = 0;
//	m_BlendFunction.SourceConstantAlpha = alpha;
//	//(cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
//	if (m_BitMapInfo.bmBitsPixel == 32)
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
//	}
//	else
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
//	}
//	
//	AlphaBlend( g_hOffScreenDC, desk.left, desk.top, desk.right, desk.bottom,
//				m_hMemDC, src.left, src.top, src.right, src.bottom, m_BlendFunction );
//	return true;
//}
//bool SBitMap::DrawAlphaBlend(RECT desk, RECT src, BLENDFUNCTION bf)
//{
//	AlphaBlend( g_hOffScreenDC, desk.left, desk.top, desk.right, desk.bottom,
//				m_hMemDC, src.left, src.top, src.right, src.bottom, bf );
//	return true;
//}
//bool SBitMap::DrawAlphaBlend(LONG x, LONG y, BYTE alpha, BLENDFUNCTION bf)
//{
//	RECT desk = { x, y, m_BitMapInfo.bmWidth, m_BitMapInfo.bmHeight };
//	RECT src = { 0, 0, m_BitMapInfo.bmWidth, m_BitMapInfo.bmHeight };
//	m_BlendFunction.BlendOp = AC_SRC_OVER;
//	m_BlendFunction.BlendFlags = 0;
//	m_BlendFunction.SourceConstantAlpha = alpha;
//	//(cosf(g_fGameTimer) * 0.5f + 0.5f) * 255;
//	if (m_BitMapInfo.bmBitsPixel == 32)
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
//	}
//	else
//	{
//		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
//	}
//
//	AlphaBlend( g_hOffScreenDC, desk.left, desk.top, desk.right, desk.bottom,
//				m_hMemDC, src.left, src.top, src.right, src.bottom, bf );
//	return true;
//}