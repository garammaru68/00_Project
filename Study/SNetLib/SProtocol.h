#pragma once
#include <SBasisStd.h>
#include <windows.h>
#include <iostream>

#pragma pack(push,1)
typedef struct
{
	uint16_t iotype;
	uint16_t len;
	uint16_t type;
	uint32_t time;
}PACKET_HEADER;
#define PACKET_HEADER_SIZE 10
typedef struct
{
	PACKET_HEADER	ph;
	int8_t			msg[1024];
}UPACKET, *P_UPACKET;

struct SChatMsg
{
	int  iCnt;
	char szName[32];
	char buffer[128];
};
struct SLogin
{
	char szID[16];
	char szPS[16];
};
#define SLoginSize sizeof(SLogin)
struct SLoginResult
{
	int   iRet;
};
typedef struct
{
	uint16_t	Point;		
	uint16_t	STR;		
	uint16_t	INT;		
	uint16_t	DEX;		
	uint16_t	CHA;		
	uint16_t	Life;		
	uint16_t	Stamina;	
	int8_t		Status;		
	uint16_t	Skins[6];	
	int8_t		Parts[4];	
	int8_t		Level;		
	uint32_t	Exp;		
	uint32_t	Skills[16];	

}SSTATUS_VARS;

//------------------------------------------------------------------------------
typedef struct
{
	int8_t		Rank;
	int8_t		Medals;
	uint16_t	Fame;
	uint32_t	Win;
	uint32_t	Lose;

}SSCORE_VARS;
#pragma pack(pop)

enum SPACKET_TYPE
{
	PACKET_LOGIN_REQ = 10,
	PACKET_LOGIN_ACK,
	PACKET_CHAT_MSG = 100,
	PACKET_TSTATUS_VARS,
};