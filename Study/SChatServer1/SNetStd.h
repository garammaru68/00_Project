#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <atlconv.h>
#include "SProtocol.h"

#pragma comment(lib, "ws2_32.lib")

typedef std::basic_string<TCHAR>	T_STR;
typedef std::basic_string<CHAR>		C_STR;
typedef std::vector<T_STR>			T_STR_VECTOR;
static std::wstring to_mw(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};
static char* GetWtM(WCHAR* data)
{
	char retData[4096] = { 0 };
	int iLength = WideCharToMultiByte(CP_ACP, 0, data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0, data, -1, retData, iLength, NULL, NULL);

	return retData;
}
static WCHAR* GetMtW(char* data)
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
static void Error(const CHAR* msg = 0, const char* lpData = 0)
{
	LPVOID* lpMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(CHAR*)&lpMsg, 0, NULL);

	std::string szBuffer = (lpData != nullptr) ? lpData : "";
	szBuffer += "\n";
	szBuffer += (CHAR*)lpMsg;

	MessageBoxA(NULL, szBuffer.c_str(), msg, MB_ICONERROR);
	LocalFree(lpMsg);
}
static void Check(int iRet, int line)
{
	if (iRet == SOCKET_ERROR)
	{
		CHAR	szBuffer[256] = { 0, };
		sprintf_s(szBuffer, "%s[%d]", __FILE__, line);
		Error("ERROR", szBuffer);
		exit(1);
	}
}