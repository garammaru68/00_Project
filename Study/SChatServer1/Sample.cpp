#include "SNetwork.h"
void main()
{
	SNetwork net;
	if (net.InitNetwork() == false)
	{
		return;
	}
	net.Run();
	net.DeleteNetwork();
}