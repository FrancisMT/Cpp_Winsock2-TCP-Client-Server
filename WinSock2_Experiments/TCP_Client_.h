#pragma once
#include <WinSock2.h>

class TCP_Client
{
public:
	TCP_Client(int Port, char* IP);
	~TCP_Client();
	
	void ManageCommunication(char *DataToSend) const;

protected:
	int PortNumber;
	char *ServerIP;
	SOCKET ClientSocket;
};
