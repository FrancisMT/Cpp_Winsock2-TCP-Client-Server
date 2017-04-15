// TCP_Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TCP_Client.h"

int main()
{
	int ServerPort(4242);
	char* ServerIP("127.0.0.1");

	TCP_Client NewTCPClient(ServerPort, ServerIP);

    return 0;
}

