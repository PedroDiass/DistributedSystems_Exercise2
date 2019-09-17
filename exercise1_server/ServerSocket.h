#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <winsock2.h>


using std::string;
using std::map;

class ServerSocket
{
public:
	ServerSocket();

	/*
	initilizes server socket and andress, then bind and listen
	*/
	void start();

	/*
	uses select method to recieve requests, then process
	*/
	void run();
	
	~ServerSocket();

private:
	map<string, SOCKET> clientsList;
	WSADATA WSAData;
	SOCKET serverSocket;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddr;
	SOCKADDR_IN clientAddr;

	int const REQUIRED_WORKERS_COUNT = 4;

	void processWorkerConnect(fd_set &master);
	void processWorkerDisconnect(SOCKET &socket, fd_set &master);
	void sendJob(fd_set &master);

	string jobText = {"fala galerinha,do,youtube aqui,quem vos fala,e o coxolinha viajante e,hoje estamos aqui para apresentar,mais, um video da serie minecraft pro youtube e nois ja deixa aquele like monstro e ativa o xininho"};

	std::vector<string> split();
};

