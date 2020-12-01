#pragma once
#include "SNetStd.h"

struct SUser
{
	SOCKET		m_Sock;
	SOCKADDR_IN	addr;
	char		recvBuf[10000];
	int			iRecvSize;
	int			iSendSize;
	T_STR		szName;
	SUser()
	{
		iRecvSize = 0;
		iSendSize = 0;
		ZeroMemory(recvBuf, sizeof(char) * 10000);
	}
};
const int	g_ipacketSize = sizeof(SChatMsg);

class SNetwork
{
public:
	std::list<SUser>	 m_UserList;
	SOCKET				 m_ListenSock;
	std::vector<UPACKET> m_recvPacket;
public:
	bool AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool DelUser(SUser& user);
	void AddPacket(SUser& user, UPACKET* packet);
	bool RecvData(SUser& user);
	bool SendData(SUser& user, UPACKET& msg);
public:
	void REQ_UserName(SOCKET sock, int type);
	int  SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
public:
	virtual bool Run();
public:
	bool RecvUserList();
	bool Broadcastting();
	bool InitNetwork();
	bool DeleteNetwork();
	bool Accept();
public:
	SNetwork();
	virtual ~SNetwork();
};