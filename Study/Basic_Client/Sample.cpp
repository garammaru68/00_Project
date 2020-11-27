#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
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
	while (1)
	{
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		int optval = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
			(char*)&optval, sizeof(optval)) != 0)
		{
			break;
		}

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

		int iRecvSize = 0;
		int iSendSize = 0;
		int iPacketSize = sizeof(SMsg);
		char recvBuf[10000] = { 0, };

		bool bConnect = true;
		while (e - s < 1 && bConnect)
		{
			while (iSendSize < iPacketSize)
			{
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.buffer, 32, "¼­»ó¹Î");
				msg.iCnt = iCount;
				char recvBuf[3001] = { 0, };
				clock_t t1 = clock();
				iSendSize += send(sock, (char*)&msg, iPacketSize - iSendSize, 0);

				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			memset(recvBuf, 0, sizeof(char) * 10000);
			while (iRecvSize < iPacketSize && bConnect)
			{
				iRecvSize += recv(sock, recvBuf, iPacketSize - iRecvSize, 0);

				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
				if (sizeof(SMsg) == iRecvSize)
				{
					memcpy(&msg, recvBuf, sizeof(SMsg));
					//printf("\n%d:%s", msg.iCnt, msg.buffer);
					//p\\rintf("\n%s", msg.buffer);
				}
			}
			//iSendSize = 0;
			//iRecvSize = 0;
			//e = clock();
			//clock_t t = e - s;
			//iCount++;
		}
		closesocket(sock);
		Sleep(1000);
	}
	WSACleanup();
}