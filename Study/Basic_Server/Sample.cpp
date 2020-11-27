#define _WINSOCK_DEPRECATED_NO_WARNINGS
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

	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(sock, SOL_SOCKET, SO_TYPE,
		(char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	int iSendBuffer = 100000;
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&iSendBuffer, sockTypeLen);
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&sockType, &sockTypeLen);
	printf("SendBuffer=%d\n", sockType);
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&sockType, &sockTypeLen);
	printf("RecvBuffer=%d\n", sockType);

	int optval = 1;
	iRet = setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	// 다수 프로세스에서 ip&port 재사용 가능
	/*iRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}*/

	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optLinger, iLingerLen) != 0)
	{
		return;
	}

	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("192.168.0.5");
	sa.sin_port = htons(10000);

	iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	iRet = listen(sock, SOMAXCONN);

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
		printf("\n접속->%s:%d", inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port));
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
			while (iRecvSize < iPacketSize)
			{
				iRecvSize += recv(client, recvBuf, iPacketSize - iRecvSize, 0);
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
			//printf("\n%d:%s", msg.iCnt, msg.buffer);
			printf("\n%s", msg.buffer);
		}
		shutdown(client, SD_SEND);
		closesocket(client);
		printf("\n해제->%s:%d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}
	closesocket(sock);

	WSACleanup();
	recvlist.clear();
}