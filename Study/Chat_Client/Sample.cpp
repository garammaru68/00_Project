#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <conio.h>
#pragma comment (lib, "ws2_32.lib")

struct SMsg
{
	int iCnt;
	char buffer[3000];
};
void main(int argc, char* argv[])
{
	const char* ipAddress = "192.168.0.5";
	unsigned short iPort = 10000;

	WSADATA	wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int optval = 1;
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
	sa.sin_addr.s_addr = inet_addr(ipAddress);
	sa.sin_port = htons(iPort);

	int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	unsigned long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	int iCount = 0;
	SMsg msg;
	int iRecvSize = 0;
	int iSendSize = 0;
	int iPacketSize = sizeof(SMsg);
	char recvBuf[10000] = { 0, };
	char sendBuf[10000] = { 0, };

	bool bConnect = true;
	bool bRecvData = false;
	int iMsgLength = 0;
	while (bConnect)
	{
		if (_kbhit() != 0)
		{
			int iValue = _getche();
			int ilen = strlen(sendBuf);
			if (ilen == 0 && iValue == '\r')
			{
				break;
			}
			if (iValue == '\r')
			{
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.buffer, sendBuf);
				msg.iCnt = iCount++;

				iMsgLength = 0;
				while (iSendSize < iPacketSize)
				{
					char recvBuf[3001] = { 0, };
					iSendSize += send(sock, (char*)&msg, iPacketSize - iSendSize, 0);
					if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							bConnect = false;
						}
						break;
					}
				}
				memset(sendBuf, 0, sizeof(char) * 10000);
			}
			else
			{
				sendBuf[iMsgLength++] = iValue;
			}
		}
		memset(recvBuf, 0, sizeof(char) * 10000);
		bRecvData = true;
		while (iRecvSize < iPacketSize && bRecvData)
		{
			iRecvSize += recv(sock, recvBuf, iPacketSize - iRecvSize, 0);
			if (iRecvSize == 0)
			{
				bConnect = false;
				break;
			}
			if (iRecvSize == SOCKET_ERROR)
			{
				bRecvData = false;
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					bConnect = false;
					break;
				}
			}
			if (sizeof(SMsg) == iRecvSize)
			{
				memcpy(&msg, recvBuf, sizeof(SMsg));
				printf("\nRecv = %d:%s\n", msg.iCnt, msg.buffer);
				bRecvData = false;
			}
		}
		iSendSize = 0;
		iRecvSize = 0;
	}
	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
}