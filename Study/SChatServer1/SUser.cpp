#include "SUser.h"

SUser::SUser()
{
	iRecvSize = 0;
	iSendSize = 0;
	ZeroMemory(recvBuf, sizeof(char) * 10000);
}
SUser::~SUser()
{

}
