// WinSock2_Experiments.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TCP_Server.h"


int main()
{
	int ServerPortNumber = 4242;
	TCP_Server TCPServer(ServerPortNumber);
	TCPServer.StartListening();
	
	return 0;
}

