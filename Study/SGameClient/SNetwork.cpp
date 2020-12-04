#include "SNetwork.h"

bool SNetwork::MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}
int SNetwork::SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type)
{
	UPACKET packet;
	MakePacket(packet, msg, iLen, type);

	int iSendSize = 0;
	int iRet = 0;
	while (iSendSize < packet.ph.len)
	{
		iRet = send(sock, (char*)&packet, packet.ph.len - iSendSize, 0);
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			if(WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
			return 0;
		}
		iSendSize += iRet;
	}
	return iSendSize;
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
									packet->ph.len - user.iRecvSize, 0);
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
								msg.ph.len - user.iSendSize, 0);
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
void SNetwork::PacketProcess()
{
	std::vector<SPacket>::iterator senditer;
	for (senditer = m_recvPacket.begin();
		senditer != m_recvPacket.end(); senditer++)
	{
		UPACKET* packet = (UPACKET*)&senditer->packet;
		if (packet->ph.type == PACKET_CHAT_MSG)
		{
			SChatMsg* pMsg = (SChatMsg*)&packet->msg;
			printf("\n[%s]%s:%d", pMsg->szName, pMsg->buffer, pMsg->iCnt);
		}
		if (packet->ph.type == PACKET_LOGIN_REQ)
		{
			UPACKET sendPacket;
			T_STR szID = L"kgca";
			T_STR szPS = L"game";
			SLogin* login = (SLogin*)packet->msg;
			T_STR szIDUser = to_mw(login->szID);
			T_STR szPSUser = to_mw(login->szPS);
			SLoginResult ret;
			if (szID == szIDUser && szPS == szPSUser)
			{
				ret.iRet = 1;
			}
			else
			{
				ret.iRet = 0;
			}
			MakePacket(sendPacket, (char*)&ret, sizeof(SLoginResult), PACKET_LOGIN_ACK);
			senditer->pUser->m_SendPacket.push_back(sendPacket);
		}
	}
}
bool SNetwork::Process()
{
	PacketProcess();
	
	std::list<SUser>::iterator iter;
	for (iter = m_UserList.begin();
		iter != m_UserList.end();)
	{
		bool bDelete = false;
		std::vector<UPACKET>::iterator senditer;
		for (senditer = iter->m_SendPacket.begin();
			senditer != iter->m_SendPacket.end(); senditer++)
		{
			if (SendData(*iter, *senditer) == false)
			{
				bDelete == true;
				break;
			}
		}
		iter->m_SendPacket.clear();

		if (bDelete == true)
		{
			DelUser(*iter);
			iter = m_UserList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return Broadcastting();
}
bool SNetwork::Broadcastting()
{
	std::list<SUser>::iterator iter;
	for (iter = m_UserList.begin();
		iter != m_UserList.end();)
	{
		bool bDelete = false;
		std::vector<SPacket>::iterator senditer;
		for (senditer = m_recvPacket.begin();
			senditer != m_recvPacket.end(); senditer++)
		{
			if (SendData(*iter, senditer->packet) == false)
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
		else
		{
			iter++;
		}
	}
	m_recvPacket.clear();
	return true;
}
bool SNetwork::InitNetwork(std::string ip, int port)
{
	InitializeCriticalSection(&m_cs);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	int iRet;
	m_Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Sock == INVALID_SOCKET)
	{
		Error("socket");
		return false;
	}
	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(m_Sock, SOL_SOCKET, SO_TYPE, (char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");
}