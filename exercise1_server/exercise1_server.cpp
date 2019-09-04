#include "pch.h"

#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <iterator>

#include <winsock2.h>

using namespace std;

constexpr int MESSAGE_SIZE = 1024;

int main()
{
	std::map<string, SOCKET> clientsList;

	WSADATA WSAData;
	SOCKET serverSocket;
	SOCKET clientSocket ;
	SOCKADDR_IN serverAddr, clientAddr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(12345);

	bind(serverSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));

	listen(serverSocket, SOMAXCONN);
	 
	cout << "Listening..." << endl;

	fd_set master;
	FD_ZERO(&master);

	FD_SET(serverSocket, &master);

	while (true) {
		
		fd_set clone = master;
		
		int socketCount = select(0, &clone, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET socket = clone.fd_array[i];

			if (FD_ISSET(serverSocket, &clone)) {
				SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
				FD_SET(clientSocket, &master);
				cout << "client = " << clientSocket << endl;
				string welcomeMessage = "Bem Vindo ao Chat!";
				int sendMsg = send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size() + 1, 0);
			
				for (int i = 0; i < master.fd_count; i++) {
					char message[MESSAGE_SIZE];

					SOCKET destSocket = master.fd_array[i];
					if (destSocket != clientSocket && destSocket != serverSocket) {
						std::stringstream ss;
						ss << "<Usuario @" << socket << " conectado> " << message;
						string finalMessage = ss.str();
						send(destSocket, finalMessage.c_str(), finalMessage.size() + 1, 0);
					}
				}
			}
			else {
				char message[MESSAGE_SIZE];
				//ZeroMemory(message, MESSAGE_SIZE);

				int bytesIn = recv(socket, message, MESSAGE_SIZE + 1, 0);
				if (bytesIn <= 0) {
					closesocket(socket);
					FD_CLR(socket, &master);
					cout << "Um corno foi desconectado" << endl;
				}
				else {
					for (int i = 0; i < master.fd_count; i++) {
						SOCKET destSocket = master.fd_array[i];
						if (destSocket != socket && destSocket != serverSocket) {
							cout << "broadcast para socket = " << destSocket << endl;
							std::stringstream ss;
							ss << "@" << socket << " diz: " << message;
							string finalMessage = ss.str();
 							cout << "finalMsg = " << finalMessage << endl;
							send(destSocket, finalMessage.c_str(), finalMessage.size(), 0);
						}
					}
				}
			}

		}

	}
}

