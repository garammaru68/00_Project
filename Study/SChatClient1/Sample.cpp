#define _WINSOC_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <conio.h>
#include <algorithm>
#include "SProtocol.h"
#pragma comment (lib, "ws2_32.lib")
typedef std::basic_string<TCHAR>	T_STR;
typedef std::basic_string<CHAR>		C_STR;

static std::wstring mtw(std::string str)
{
	std::wstring ret = std::wstring(str.begin(), str.end());
	return ret;
}
static std::string wtm(std::wstring str)
{
	return std::string(str.begin(), str.end());
}
static char* GetWtM(WCHAR* data)
{
	char retData[4096] = { 0 };
	int iLength = WideCharToMultiByte(CP_ACP, 0, data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0, data, -1, retData, iLength, NULL, NULL);
	
	return retData;
}
static bool GetWtM(WCHAR* src, char* pDest)
{
	int iLength = WideCharToMultiByte(CP_ACP, 0, src, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0, src, -1, pDest, iLength, NULL, NULL);
	if (iRet == 0) return false;
	
	return true;
}
static WCHAR* GetMtW(const char* data)
{
	WCHAR retData[4096] = { 0 };
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0, data, -1, retData, iLength);

	return retData;
}
static bool GetMtW(char* pSrc, WCHAR* pDest)
{
	int iLength = MultiByteToWideChar(CP_ACP, 0, pSrc, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0, pSrc, -1, pDest, iLength);
	if (iRet == 0) return false;
	return true;
}
static void PRINT(char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char buf[256] = { 0, };
	vsprintf_s(buf, fmt, arg);
	printf("\n=====> %s", buf);
	va_end(arg);
}