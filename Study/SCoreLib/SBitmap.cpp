#include "SBitmap.h"

bool SBitmap::Init()
{
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;

	return true;
}
HBITMAP SBitmap::LoadDIB(wstring filename)
{
	HANDLE hFile = CreateFile(filename.c_str(),
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	BITMAPFILEHEADER header;
	DWORD dwRead;
	BOOL bRet = ReadFile(hFile, &header, sizeof(BITMAPFILEHEADER),
		&dwRead, NULL);

	DWORD dwDataSize = header.bfOffBits - sizeof(BITMAPFILEHEADER);

	BITMAPINFO* pBitInfo = (BITMAPINFO*)malloc(dwDataSize);

	bRet = ReadFile(hFile, pBitInfo, dwDataSize, &dwRead, NULL);

	PVOID pRaster = 0;
	m_hBitmap = CreateDIBSection(g_hScreenDC, pBitInfo, DIB_RGB_COLORS, 
								(LPVOID*)(&m_pRaster), NULL, 0);

	ReadFile(hFile, m_pRaster, 
			header.bfSize - header.bfOffBits,
			&dwRead, NULL);

	free(pBitInfo);
	CloseHandle(hFile);
	return m_hBitmap;
}
bool SBitmap::Load(wstring filename)
{
	//m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
	//	filename.c_str(),
	//	IMAGE_BITMAP,
	//	0,
	//	0,
	//	LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hBitmap = LoadDIB(filename.c_str());
	if (m_hBitmap == NULL)
	{
		return false;
	}
	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 0, 0);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;
}
bool SBitmap::CreateDIB(LONG iWidth, LONG iHeight, int iBitCount)
{
	BITMAPINFO bmi;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = iBitCount;
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;

	m_hBitmap = CreateDIBSection(g_hScreenDC, (BITMAPINFO*)&bmi,
		DIB_RGB_COLORS, (LPVOID*)&m_pRaster, NULL, 0);

	//for (int j = 0; j < iHeight; ++j)
	//{
	//	LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)m_pRaster)[j * iWidth];
	//	for (int i = 0; i < iWidth; ++i)
	//	{
	//		pbSrcRGB[0] = 255;
	//		pbSrcRGB[1] = 0;
	//		pbSrcRGB[2] = 0;
	//		pbSrcRGB[3] = 128;
	//		pbSrcRGB += 4;
	//	}
	//}
	
	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 0, 255);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;
}
