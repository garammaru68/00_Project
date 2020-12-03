#pragma once
#include "SUser.h"

class SNetwork
{
public:
	std::list<SUser>	 m_UserList;
	SOCKET				 m_Sock;
	std::vector<UPACKET> m_recvPacket;
public:
	bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	bool AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool DelUser(SUser& user);
	void AddPacket(SUser& user, UPACKET* packet);
	bool RecvData(SUser& user);
	bool SendData(SUser& user, UPACKET& msg);
	int	 SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
public:
	virtual bool Run();
	virtual bool RecvUserList();
	virtual	bool Broadcastting();
	virtual	bool InitNetwork();
	virtual	bool DeleteNetwork();
	virtual	bool Accept();
	virtual bool Process();
public:
	SNetwork();
	virtual ~SNetwork();
};