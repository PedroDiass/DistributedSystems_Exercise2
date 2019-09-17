
#include "pch.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <sstream>
#include <iterator>
#include <algorithm>
#include <winsock2.h>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::ws;

std::vector<string> split(string message)
{
	std::istringstream iss(message);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
		std::istream_iterator<std::string>());

	return results;
}

int main()
{
	WSADATA WSAData;
	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;

	fd_set clientSet;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(12345);

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	connect(clientSocket, (SOCKADDR *)&clientAddr, sizeof(clientAddr));
	char *response = new char[120];
	recv(clientSocket, response, string(response).length() + 1, 0);
	cout << string(response) << endl;
	delete[] response;

	std::thread input_thread([](SOCKET clientSocket) {
		while (true) {
			char message[120];
			cin.getline(message, 120);

			send(clientSocket, message, string(message).size() + 1, 0);

			cin.clear();
			//delete[] message;
		}
	}, clientSocket);

	while (true) {
		response = new char[120];

		recv(clientSocket, response, 120 + 1, 0);

		cout << string(response) << endl;
		cout.clear();
		break;
	}

	string message = string(response);

	for (int i = 0; i < message.size(); i++) {
		if (message.at(i) == ',') {
			message[i] = ' ';
		}
	}

	send(clientSocket, message.c_str(), message.size() + 1, 0);

	string a;
	cin >> a;

	closesocket(clientSocket);
	cout << "connection closed" << endl;

	WSACleanup();

	cout << "socket closed." << endl << endl;
}

