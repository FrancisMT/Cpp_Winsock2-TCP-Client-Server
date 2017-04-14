#pragma once

#include <WinSock2.h>

class TCP_Server
{
public:
	explicit TCP_Server(int Port);
	~TCP_Server() = default;
	int StartListening();

protected:
	int PortNumber;
	SOCKET ServerSocket;
	SOCKET ClientSocket;

private:
	int AcceptConnections();
};

