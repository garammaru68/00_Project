#include "SNetwork.h"

int SNetwork::SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type)
{
	UPACKET packet;
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);

	int iSendSize = 0;
	while (iSendSize < packet.ph.len)
	{
		iSendSize += send(sock, (char*)&packet, packet.ph.len - iSendSize, 0);
		if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
			return 0;
		}
	}
	return iSendSize;
}
void SNetwork::REQ_UserName(SOCKET sock, int type)
{
	SendMsg(sock, nullptr, 0, type);
}
bool SNetwork::RecvData(SUser& user)
{
	int iLen = recv(user.m_Sock, &user.recvBuf[user.iRecvSize],
		PACKET_HEADER_SIZE - user.iRecvSize, 0);
	if (iLen == 0 || iLen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
		return true;
	}
	user.iRecvSize += iLen;
	if (user.iRecvSize == PACKET_HEADER_SIZE)
	{
		UPACKET* packet = (UPACKET*)&user.recvBuf;
		while (user.iRecvSize < packet->ph.len)
		{
			user.iRecvSize += recv( user.m_Sock, &user.recvBuf[user.iRecvSize],
									packet->ph.len - user.iRecvSize, 0 );
		}
		if (packet->ph.type == PACKET_CHAT_MSG)
		{

		}
		AddPacket(user, packet);
		memset(user.recvBuf, 0, sizeof(char) * 10000);
		user.iRecvSize = 0;
	}
	return true;
}
bool SNetwork::SendData(SUser& user, UPACKET& msg)
{
	while (user.iSendSize < msg.ph.len)
	{
		user.iSendSize += send( user.m_Sock, (char*)&msg, 
								msg.ph.len - user.iSendSize, 0 );
		if (user.iSendSize == 0)
		{
			return false;
		}
		if (user.iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			user.iSendSize = 0;
			break;
		}
	}
	if (user.iSendSize == msg.ph.len)
	{
		user.iSendSize = 0;
	}
	return true;
}
bool SNetwork::RecvUserList()
{
	std::list<SUser>::iterator iter;
	for (iter = m_UserList.begin();
		iter != m_UserList.end();)
	{
		if (RecvData(*iter) == false)
		{
			DelUser(*iter);
			iter = m_UserList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool SNetwork::Broadcastting()
{
	std::list<SUser>::iterator iter;
	for (iter = m_UserList.begin();
		iter != m_UserList.end();)
	{
		bool bDelete = false;
		std::vector<UPACKET>::iterator senditer;
		for (senditer = m_recvPacket.begin(); 
			senditer != m_recvPacket.end(); 
			senditer++)
		{
			if (SendData(*iter, *senditer) == false)
			{
				bDelete == true;
				break;
			}
		}
		if (bDelete == true)
		{
			DelUser(*iter);
			iter = m_UserList.erase(iter);
		}
	}
}
bool SNetwork::InitNetwork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}

	int iRet;
	m_ListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ListenSock == INVALID_SOCKET)
	{
		Error("socket");
		return false;
	}

	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(m_ListenSock, SOL_SOCKET, SO_TYPE, (char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%\r\n", "SOCK_DGRAM");

	int iSendBuffer = 100000;
	setsockopt(m_ListenSock, SOL_SOCKET, SO_SNDBUF, (char*)&iSendBuffer, sockTypeLen);
	getsockopt(m_ListenSock, SOL_SOCKET, SO_SNDBUF, (char*)&sockType, &sockTypeLen);
	printf("SendBuffer = %d\n", sockType);
	getsockopt(m_ListenSock, SOL_SOCKET, SO_RCVBUF, (char*)&sockType, &sockTypeLen);
	printf("RecvBuffer = %d\n", sockType);

	int optval = 1;

	Check(setsockopt(m_ListenSock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
					(char*)&optval, sizeof(optval)), __LINE__);
	// 다수 프로세스에서 ip&port 재사용 가능
	/*iRet = setsockopt(m_ListenSock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}*/
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(m_ListenSock, SOL_SOCKET, SO_LINGER, (char*)&optLinger, iLingerLen) != 0)
	{
		Error("SO_LINGER");
		return false;
	}

	SOCKADDR_IN sa;
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(10000);
	Check(bind(m_ListenSock, (SOCKADDR*)&sa, sizeof(sa)), __LINE__);

	iRet = listen(m_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		Error("listen");
		return false;
	}
	unsigned long iMode = 1;
	ioctlsocket(m_ListenSock, FIONBIO, &iMode);

	return true;
}
bool SNetwork::DeleteNetwork()
{
	closesocket(m_ListenSock);
	WSACleanup();
	return true;
}
bool SNetwork::AddUser(SOCKET sock, SOCKADDR_IN addr)
{
	SUser user;
	user.m_Sock = sock;
	user.addr = addr;
	m_UserList.push_back(user);
	printf("\n접속->%s:%d", inet_ntoa(addr.sin_addr),
							ntohs(addr.sin_port));
	
	return true;
}
bool SNetwork::DelUser(SUser& user)
{
	shutdown(user.m_Sock, SD_SEND);
	closesocket(user.m_Sock);
	printf("\n해제->%s:%d", inet_ntoa(user.addr.sin_addr),
		ntohs(user.addr.sin_port));
	
	return true;
}
void SNetwork::AddPacket(SUser& user, UPACKET* packet)
{
	if (packet->ph.type == PACKET_USERNAME_ACK)
	{
		user.szName = to_mw((char*)packet->msg);
	}
	m_recvPacket.push_back(*packet);
	user.iRecvSize = 0;
}
bool SNetwork::Accept()
{
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	SOCKET client = accept(m_ListenSock, (SOCKADDR*)&clientAddr, &len);
	if (client == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	else
	{
		AddUser(client, clientAddr);
		REQ_UserName(client, PACKET_USERNAME_REQ);
	}
	return true;
}
bool SNetwork::Run()
{
	while (1)
	{
		if (Accept() == false)
		{
			break;
		}
		if (RecvUserList() == false)
		{
			break;
		}
		if (Broadcastting() == false)
		{
			break;
		}
	}
	return true;
}
SNetwork::SNetwork() {}
SNetwork::~SNetwork() {}