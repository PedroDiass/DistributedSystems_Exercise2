#include "pch.h"
#include "ServerSocket.h"


ServerSocket::ServerSocket()
{
}

void ServerSocket::start()
{
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(12345);

	bind(serverSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));

	listen(serverSocket, SOMAXCONN);

	std::cout << "listening..." << std::endl;
}

void ServerSocket::run()
{
	fd_set master;
	FD_ZERO(&master);

	FD_SET(serverSocket, &master);

	while (true) {

		if ((master.fd_count - 1) == REQUIRED_WORKERS_COUNT) {
			sendJob(master);
		}

		fd_set clone = master;

		int socketCount = select(0, &clone, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET socket = clone.fd_array[i];

			if (FD_ISSET(serverSocket, &clone)) {
				processWorkerConnect(master);
			}
			else {
				char message[300];

				int bytesIn = recv(socket, message, 300 + 1, 0);
				if (bytesIn <= 0) {
					processWorkerDisconnect(socket, master);
				}
				else {
					std::cout << "worker" << socket << " finish his job, message = " 
						<< message << std::endl;
				}
			}

		}
	}
}

void ServerSocket::processWorkerConnect(fd_set &master)
{
	SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
	FD_SET(clientSocket, &master);	// add worker to socket's set

	std::cout << "worker connected -> " << clientSocket
		<< "\nworkers count -> " << master.fd_count - 1 << std::endl;

	string welcomeMessage = "Bem Vindo ao Chat!";
	send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size() + 1, 0);
}

void ServerSocket::processWorkerDisconnect(SOCKET &socket, fd_set &master)
{
	FD_CLR(socket, &master);		// remove worker from socket's set

	std::cout << "worker disconnected -> " << socket
		<< "\nworkers count -> " << master.fd_count - 1 << std::endl;

	closesocket(socket);
}

void ServerSocket::sendJob(fd_set &master)
{
	std::vector<std::string> words = split();

	int wordsForWorker = words.size() / REQUIRED_WORKERS_COUNT;
	int wordLastWorker = words.size() % REQUIRED_WORKERS_COUNT;

	int i = 0;
	int sendedJobsCount = 0;
	while (true) {
		if (master.fd_array[i] != serverSocket) {
			if (sendedJobsCount < (REQUIRED_WORKERS_COUNT - 1)) {
				string message = "";
				for (int k = sendedJobsCount * (int)wordsForWorker; k < (sendedJobsCount + 1) * (int)wordsForWorker; k++) {
					message += words[k] + " ";
				}
				send(master.fd_array[i], message.c_str(), message.size(), 0);

				sendedJobsCount++;
			}
			else {
				string message = "";
				for (int k = sendedJobsCount * (int)wordsForWorker; k < words.size(); k++) {
					message += words[k] + " ";
				}
				send(master.fd_array[i], message.c_str(), message.size(), 0);

				break;
			}
		}
		i++;
	}
}

ServerSocket::~ServerSocket()
{
}

std::vector<string> ServerSocket::split()
{
	std::istringstream iss(jobText);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
		std::istream_iterator<std::string>());

	return results;
}