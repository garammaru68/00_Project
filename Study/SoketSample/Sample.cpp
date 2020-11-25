#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winSock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#pragma comment (lib, "ws2_32.lib")

struct SMsg
{
	int iCnt;
	char buffer[3000];
};
void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN sa;

	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("192.168.0.5");
	sa.sin_port = htons(10000);
	int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	int iCount = 0;
	clock_t s = clock();
	clock_t e = clock();
	SMsg msg;
	while (e - s < 1000)
	{
		memset(&msg, 0, sizeof(msg));
		strcpy_s(msg.buffer, 32, "¾È³ç");
		msg.iCnt = iCount;
		char recvBuf[3001] = { 0, };
		int iLen = sizeof(msg);
		clock_t t1 = clock();
		iRet = send(sock, (char*)&msg, iLen, 0);
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			break;
		}
		iRet = recv(sock, recvBuf, 3001, 0);
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			break;
		}
		memcpy(&msg, recvBuf, sizeof(SMsg));
		e = clock();
		clock_t t = e - t1;
		printf("%d", t);
		iCount++;
	}
	closesocket(sock);
	WSACleanup();
}