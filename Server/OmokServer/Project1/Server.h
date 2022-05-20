#pragma once

#include <winsock2.h>
#include <iostream>
#include <thread>
#include <set>
#include <map>

#pragma comment(lib, "ws2_32.lib")

#define PACKET_SIZE 1024 
#define SERVER Server::Instance()
#define SOCKLIST std::set<SOCKET>
#define CLIENT_THREAD std::map<SOCKET, std::thread>

SOCKET skt, client_sock;
SOCKADDR_IN client = {};

class Server
{
private:
	static Server* instance;

public:
	static Server& Instance();

public:
	void Run();

public:
	SOCKLIST* GetSockList();
	CLIENT_THREAD* GetClientThread();
private:
	SOCKLIST clientSock;
	CLIENT_THREAD clientThread;
};