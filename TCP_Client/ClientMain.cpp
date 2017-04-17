// TCP_Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TCP_Client.h"
#include <thread>

int main()
{
	const int ServerPort(4242);
	const char* ServerIP("127.0.0.1");

	TCP_Client TCPClientOne(ServerPort, ServerIP);

	return 0;
}

