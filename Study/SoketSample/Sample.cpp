#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#pragma comment (lib, "ws2_32.lib")

struct SMsg
{
	int		iCnt;
	char	buffer[3000];
};

void main(int argc, char* argv[])
{
	const char* ipAddress = "192.168.0.151";
	unsigned short iPort = 10000;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	while (1)
	{
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		int optval = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) != 0)
		{
			break;
		}

		SOCKADDR_IN sa;
		USHORT jValue = 10000;
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = inet_addr(ipAddress);
		sa.sin_port = htons(iPort);
		
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
		while (e - s < 10000 && bConnect)
		{
			while (iSendSize > iPacketSize)
			{
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.buffer, 32, "¾È³ç");
				msg.iCnt = iCount;

			}
		}

	}
}