#define _WNSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <vector>
#pragma comment (lib, "ws2_32.lib")

struct SMsg
{
	int iCnt;
	char buffer[3000];
};

void Error(const CHAR* msg = 0, const char* lpData = 0)
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

void Check(int iRet, int line)
{
	if (iRet == SOCKET_ERROR)
	{
		CHAR szBuffer[256] = { 0, };
		sprintf_s(szBuffer, "%s[%d", __FILE__, line);
		Error("ERROR", szBuffer);
		exit(1);
	}
}

void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	std::vector<SMsg> recvlist;

	int iRet;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		Error("socket");
		return;
	}

	int sockType;
	int sockTypeLen = sizeof(int);

	getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&sockType, &sockTypeLen);

	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	int iSendBuffer = 100000;
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&iSendBuffer, sockTypeLen);
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&sockType, &sockTypeLen);
	printf("SendBuffer = %d\n", sockType);
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&sockType, &sockTypeLen);
	printf("RecvBuffer = %d\n", sockType);

	int optval = 1;
	Check(setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
					(char*)&optval, sizeof(optval)), __LINE__);

	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optLinger, iLingerLen != 0) != 0)
	{
		Error("SO_LINGER");
		return;
	}
	
	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(10000);

	Check(bind(sock, (SOCKADDR*)&sa, sizeof(sa)), __LINE__);
	Check(listen(sock, SOMAXCONN), __LINE__);

	bool bConnect = false;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	while (1)
	{
		SOCKET client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		if (client == INVALID_SOCKET)
		{
			break;
		}
		printf("\n접속->%s:%d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		int iCount = 0;
		bConnect = true;

		SMsg msg;
		char recvBuf[10000] = { 0, };
		char* pRecv = recvBuf;
		int iRecvSize = 0;
		int iSendSize = 0;
		int iPacketSize = sizeof(SMsg);

		while (bConnect)
		{
			while(iRecvSize < iPacketSize)
			{
				iRecvSize += recv(client, recvBuf,
					iPacketSize - iRecvSize, 0);
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			while (iSendSize < iPacketSize && bConnect)
			{
				memcpy(&msg, recvBuf, sizeof(SMsg));
				iSendSize += send(client, (char*)&msg, iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			iSendSize = 0;
			iRecvSize = 0;
			recvlist.push_back(msg);
			printf("\n%d:%s", msg.iCnt, msg.buffer);
		}
		shutdown(client, SD_SEND);
		closesocket(client);
		printf("\n해제->%s:%d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}
	closesocket(sock);

	WSACleanup();
	recvlist.clear();
}