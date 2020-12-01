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
			user.iRecvSize += recv(user.m_Sock,
				&user.recvBuf[user.iRecvSize],
				packet->ph.len - user.iRecvSize, 0);
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
