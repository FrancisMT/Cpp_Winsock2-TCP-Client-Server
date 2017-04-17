#pragma once
#include <WinSock2.h>

class TCP_Client
{
public:
	TCP_Client(const int Port, const char* IP);
	~TCP_Client();

	void ManageCommunication(char *DataToSend) const;

protected:
	const int ServerPort;
	const char *ServerIP;
	SOCKET ClientSocket;
};
