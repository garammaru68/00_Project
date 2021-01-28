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

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "SNetLib.lib")

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
static char* GetWtm(WCHAR* data)
{
	char retData[4096] = { 0 };
	int iLength = WideCharToMultiByte(  CP_ACP, 0, data, -1, 0, 0,
										NULL, NULL);
	int iRet = WideCharToMultiByte( CP_ACP, 0, data, -1, retData, iLength,
									NULL, NULL);
	return retData;
}
static bool GetWtM(WCHAR* src, char* pDest)
{
	int iLength = WideCharToMultiByte(  CP_ACP, 0, src, -1, 0, 0, 
										NULL, NULL);
	int iRet = WideCharToMultiByte( CP_ACP, 0, src, -1, pDest, iLength, 
									NULL, NULL);
	if (iRet == 0) return false;

	return true;
}
static WCHAR* GetMtW(char* data)
{
	WCHAR retData[4096] = { 0 };

	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0, data, -1, retData, iLength);

	return retData;
}