#include "stdafx.h"
#include "TCP_Client_.h"
#include <iostream>


TCP_Client_::TCP_Client_(int Port, char* IP)
	: PortNumber(Port)
	, ServerIP(IP) {

	WSAData WSAData;
	WORD DllVersionRequested = MAKEWORD(2, 1);
	int WSAInitResult = WSAStartup(DllVersionRequested, &WSAData);
	if (WSAInitResult != 0) {
		std::cout << "[CLIENT] Winsock.dll not found" << std::endl;

		//WSACleanup() terminates use of the Winsock 2 DLL
		WSACleanup();
		exit(1);
	}
	else {
		std::cout << "[CLIENT] Winsock.dll found" << std::endl;
	}

	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		std::cout << "[CLIENT] Error initializing ClientSocket." << std::endl;
		WSACleanup();
	}

	sockaddr_in OutgoingConnection;
	int OutgoingConnectionSize = sizeof(OutgoingConnection);
	OutgoingConnection.sin_family = AF_INET;					//AF is Address Family. The AF we want to use is Internet (IPv4 addresses)
	OutgoingConnection.sin_port = htons(PortNumber);
	OutgoingConnection.sin_addr.s_addr = inet_addr(ServerIP);

	auto ConnectionResult = connect(ClientSocket, reinterpret_cast<sockaddr*>(&OutgoingConnection), OutgoingConnectionSize);
	if (ConnectionResult == SOCKET_ERROR) {
		std::cout << "[CLIENT] Error Connecting to Server @" << ServerIP << ":" << PortNumber << std::endl;
		WSACleanup();
		this->~TCP_Client_();
		exit(1);
	}
	else {
		std::cout << "[CLIENT] Connected to Server @" << ServerIP << ":" << PortNumber << std::endl;

		ManageCommunication("Ping_");
	}

}

TCP_Client_::~TCP_Client_() {
	
	std::cout << "Terminating Client" << std::endl;
	closesocket(ClientSocket);
	WSACleanup();
}

void TCP_Client_::ManageCommunication(char* DataToSend) const {

	//Send test message to Server
	send(ClientSocket, DataToSend, sizeof(DataToSend), 0);

	//Read message from server
	const auto BufferSize(100);
	const auto MsgTerminationChar("_");
	char ServerMessage[BufferSize];

	auto ReceptionStatus = recv(ClientSocket, ServerMessage, BufferSize, 0);
	auto ProcessedServerMessage = strtok(ServerMessage, MsgTerminationChar);

	if (ReceptionStatus == SOCKET_ERROR) {
		std::cout << "[CLIENT] Error receiving message from " << ServerIP << ":" << PortNumber << " -> " << WSAGetLastError() << std::endl;
	}
	else if (!strcmp(ProcessedServerMessage, "Pong")) {
		std::cout << "[CLIENT] Message received from " << ServerIP << ":" << PortNumber << " -> " << ProcessedServerMessage << std::endl;

		this->~TCP_Client_();
		exit(1);
	}
}
