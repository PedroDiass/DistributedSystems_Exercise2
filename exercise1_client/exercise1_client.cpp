
#include "pch.h"
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>

using namespace std;

int main()
{
	WSADATA WSAData;
	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(12345);

	connect(clientSocket, (SOCKADDR *)&clientAddr, sizeof(clientAddr));

	char buffer[1024] = { "Hello Amazing Socket World." };
	send(clientSocket, buffer, sizeof(buffer), 0);
	cout << "Message sent!" << endl;
	closesocket(clientSocket);

	WSACleanup();

	cout << "Socket closed." << endl << endl;
}