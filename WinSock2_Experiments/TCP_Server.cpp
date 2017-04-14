#include "stdafx.h"
#include "TCP_Server.h"
#include <iostream>


TCP_Server::TCP_Server(int Port)
	: PortNumber(Port)
	, ServerSocket(0)
	, ClientSocket(0) {}


int TCP_Server::StartListening() {

	WSADATA WSAData;
	WORD WordVersionRequested = MAKEWORD(2, 2);
	//WSAStartup() initializes the program to call winsock2.dll
	int WSAError = WSAStartup(WordVersionRequested, &WSAData);

	if (WSAError != 0) {
		std::cout << "[SERVER] Winsock.dll not found" << std::endl;

		//WSACleanup() terminates use of the Winsock 2 DLL
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "[SERVER] Winsock.dll found" << std::endl;
	}

	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		std::cout << "[SERVER] Error initializing ServerSocket." << std::endl;
		WSACleanup();
		return 0;
	}

	//SOCKADDR_IN struct is used by winsock to specify an endpoint address to which the socket connects
	char* IPAddress("127.0.0.1");

	sockaddr_in OutgoingConnection;
	int OutgoingConnectionSize = sizeof(OutgoingConnection);
	OutgoingConnection.sin_family = AF_INET;
	OutgoingConnection.sin_port = htons(PortNumber);
	OutgoingConnection.sin_addr.s_addr = inet_addr(IPAddress);

	//bind() links the ServerSocket with OutgoingConnection sockaddr_in struct.
	auto BindResult = bind(ServerSocket, reinterpret_cast<sockaddr*>(&OutgoingConnection), OutgoingConnectionSize);
	if (BindResult == SOCKET_ERROR) {
		std::cout << "[SERVER] Error binding ServerSocket to port number: " << PortNumber << std::endl;
		WSACleanup();
	}

	//Wait for incoming connections
	auto ListeningResult = listen(ServerSocket, 10);
	if (ListeningResult == SOCKET_ERROR) {
		std::cout << "[SERVER] Error listening on socket -> " << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "[SERVER] Waiting for connections @ " << IPAddress << ":" << PortNumber << std::endl;
	}
	std::cout << "[SERVER] Waiting for a client to connect ..." << std::endl;

	//Accept connections
	AcceptConnections();
	return 0;
}


int TCP_Server::AcceptConnections() {

	sockaddr_in IncomingConnection;
	int IncomingConnectionSize = sizeof(IncomingConnection);

	//Infinitly echo the client's IP
	while (true) {

		//Accept connection to client
		ClientSocket = accept(ServerSocket, /*out*/reinterpret_cast<sockaddr*>(&IncomingConnection), /*out*/&IncomingConnectionSize);

		auto ClientIP = inet_ntoa(IncomingConnection.sin_addr);
		auto ClientPort = IncomingConnection.sin_port;
		std::cout << "[SERVER] Received connection from: " << ClientIP << ":" << ClientPort << std::endl;

		//Read message from client
		const auto BufferSize(100);
		const auto MsgTerminationChar("_");
		char ClientMessage[BufferSize];

		auto ReceptionStatus = recv(ClientSocket, ClientMessage, BufferSize, 0);
		auto ProcessedClientMessage = strtok(ClientMessage, MsgTerminationChar);

		if (ReceptionStatus == SOCKET_ERROR) {
			std::cout << "[SEVER] Error receiving message from " << ClientIP << ":" << ClientPort << " -> " << WSAGetLastError() << std::endl;
		}
		else if (!strcmp(ProcessedClientMessage, "Ping")) {
			std::cout << "[SEVER] Message received from " << ClientIP << ":" << ClientPort << " -> " << ProcessedClientMessage << std::endl;

			auto MessageToClient("Pong");
			send(ClientSocket, MessageToClient, sizeof(MessageToClient), 0);
		}

		closesocket(ClientSocket);
	}

	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}
