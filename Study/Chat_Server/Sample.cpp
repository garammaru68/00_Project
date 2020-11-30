#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <vector>
#include <list>
#pragma comment (lib, "ws2_32.lib")

struct SMsg
{
	int iCnt;
	char buffer[3000];
};

const int			g_iPacketSize = sizeof(SMsg);
std::vector<SMsg>	g_recvPacket;

struct SUser
{
	SOCKET		g_ListenSock;
	SOCKADDR_IN	addr;
	char		recvBuf[10000];
};
std::list<SUser> userlist;
SOCKET g_ListenSock;

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
		sprintf_s(szBuffer, "%s[%d]", __FILE__, line);
		Error("ERROR", szBuffer);
		exit(1);
	}
}
bool RecvUserData()
{
	int iRecvSize = 0;
	bool bRecvData = false;
	std::list<SUser>::iterator iter;
	for (iter = userlist.begin();
		iter != userlist.end();)
	{
		SMsg msg;
		bool bDelete = false;
		bRecvData = true;
		while (iRecvSize < g_iPacketSize && bRecvData)
		{
			iRecvSize += recv(iter->g_ListenSock, iter->recvBuf,
				g_iPacketSize - iRecvSize, 0);
			if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					bDelete = true;
				}
				bRecvData = false;
				break;
			}
		}
		if (iRecvSize == g_iPacketSize)
		{
			memcpy(&msg, iter->recvBuf, sizeof(SMsg));
			g_recvPacket.push_back(msg);
		}
		iRecvSize = 0;
		if (bDelete == true)
		{
			shutdown(iter->g_ListenSock, SD_SEND);
			closesocket(iter->g_ListenSock);
			printf("\n해제->%s:%d", 
				inet_ntoa(iter->addr.sin_addr),
				ntohs(iter->addr.sin_port));
			iter = userlist.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool Broadcastting()
{
	int iSendSize = 0;
	std::list<SUser>::iterator iter;
	for (iter = userlist.begin();
		iter != userlist.end();)
	{
		iSendSize = 0;
		bool bDelete = false;
		std::vector<SMsg>::iterator senditer;
		for (senditer = g_recvPacket.begin();
			senditer != g_recvPacket.end();
			senditer++)
		{
			while (iSendSize < g_iPacketSize)
			{
				iSendSize += send(iter->g_ListenSock, (char*)&(*senditer),
					g_iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						bDelete = true;
					}
					break;
				}
			}
		}
		if (bDelete == true)
		{
			shutdown(iter->g_ListenSock, SD_SEND);
			closesocket(iter->g_ListenSock);
			iter = userlist.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	g_recvPacket.clear();
	return true;
}
bool InitNetwork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}

	int iRet;
	g_ListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_ListenSock == INVALID_SOCKET)
	{
		Error("socket");
		return false;
	}

	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(g_ListenSock, SOL_SOCKET, SO_TYPE, (char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	int iSendBuffer = 100000;
	setsockopt(g_ListenSock, SOL_SOCKET, SO_SNDBUF, (char*)&iSendBuffer, sockTypeLen);
	getsockopt(g_ListenSock, SOL_SOCKET, SO_SNDBUF, (char*)&sockType, &sockTypeLen);
	printf("SendBuffer = %d\n", sockType);
	getsockopt(g_ListenSock, SOL_SOCKET, SO_RCVBUF, (char*)&sockType, &sockTypeLen);
	printf("RecvBuffer = %d\n", sockType);

	int optval = 1;
	Check(setsockopt(g_ListenSock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
					(char*)&optval, sizeof(optval)), __LINE__);

	linger optLinger;
	int ilingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(g_ListenSock, SOL_SOCKET, SO_LINGER, (char*)&optLinger, ilingerLen) != 0)
	{
		Error("SO_LINGER");
		return false;
	}

	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(10000);

	Check(bind(g_ListenSock, (SOCKADDR*)&sa, sizeof(sa)), __LINE__);

	iRet = listen(g_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		Error("listen");
		return false;
	}

	unsigned long iMode = 1;
	ioctlsocket(g_ListenSock, FIONBIO, &iMode);

	return true;
}
bool DeleteNetwork()
{
	closesocket(g_ListenSock);
	WSACleanup();
	return true;
}
bool AddUser()
{
	int iRet;
	bool brecvData = false;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	SOCKET client = accept(g_ListenSock, (SOCKADDR*)&clientAddr, &len);
	if (client == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	else
	{
		SUser user;
		user.g_ListenSock = client;
		user.addr = clientAddr;
		userlist.push_back(user);
		printf("\n접속->%s:%d",
			inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port));
	}
	return true;
}
void main()
{
	if (InitNetwork())
	{
	}
	while (1)
	{
		if (AddUser() == false)
		{
			break;
		}
		if (RecvUserData() == false)
		{
			break;
		}
		if (Broadcastting() == false)
		{
			break;
		}
	}
	DeleteNetwork();
}