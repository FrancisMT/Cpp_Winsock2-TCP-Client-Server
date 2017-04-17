#pragma once

#include <WinSock2.h>

class TCP_Server
{
public:
	explicit TCP_Server(int Port);
	~TCP_Server();
	int StartListening();

protected:
	const char* ServerIP;
	int ServerPort;
	SOCKET ServerSocket;
	SOCKET ClientSocket;

private:
	void AcceptConnections();
};

