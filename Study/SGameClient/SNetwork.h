#pragma once
#include "SUser.h"

struct SPacket
{
	SUser*	pUser;
	UPACKET packet;
};
class SNetwork
{
public:
	std::list<SUser>		m_UserList;
	SOCKET					m_Sock;
	std::vector<SPacket>	m_recvPacket;
	CRITICAL_SECTION		m_cs;
public:
	bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	bool AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool DelUser(SUser& user);
	void AddPacket(SUser& user, UPACKET* packet);
	virtual bool RecvData(SUser& user);
	bool SendData(SUser& user, UPACKET& msg);
	int SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
public:
	virtual bool Run();
	virtual bool RecvUserList();
	virtual bool Broadcastting();
	virtual bool InitNetwork(std::string ip, int iPort);
	virtual bool InitSocket(std::string ip, int iPort);
	virtual bool DeleteNetwork();
	virtual bool Accept();
	virtual bool Process();
	virtual void PacketProcess();
public:
	SNetwork();
	virtual ~SNetwork();
};

