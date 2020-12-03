#include "SNetwork.h"
void main()
{
	HANDLE hExecuteMutex = CreateMutex(NULL, FALSE, L"OnceMutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		return;
	}
	SNetwork net;
	if (net.InitNetwork() == false)
	{
		return;
	}
	net.Run();
	net.DeleteNetwork();
	CloseHandle(hExecuteMutex);
}