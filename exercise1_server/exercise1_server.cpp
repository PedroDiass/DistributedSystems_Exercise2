#include "pch.h"

#include <iostream>
#include <winsock2.h>

using namespace std;

int main()
{
	WSADATA WSAData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddr, clientAddr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(12345);

	bind(serverSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));

	listen(serverSocket, 0);

	cout << "Listening for incoming connections..." << endl;

	char buffer[1024];
	int clientAddrSize = sizeof(clientAddr);
	if ((clientSocket = accept(serverSocket, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
		cout << "Client connected!" << endl;
		recv(clientSocket, buffer, sizeof(buffer), 0);
		cout << "Client says: " << buffer << endl;
		memset(buffer, 0, sizeof(buffer));

		closesocket(clientSocket);
		cout << "Client disconnected." << endl;
	}
}

