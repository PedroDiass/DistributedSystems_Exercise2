#include "pch.h"

#include <iostream>
#include <map>
#include "ServerSocket.h"

using namespace std;

int main()
{
	ServerSocket* server = new ServerSocket();

	server->start();
	server->run();
}

