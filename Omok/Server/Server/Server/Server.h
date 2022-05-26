#pragma once

#include <winsock2.h>
#include <iostream>
#include <thread>
#include <set>
#include <map>

#include "../../../Message/Message.h"

#pragma comment(lib, "ws2_32.lib")

#define PACKET_SIZE 1024 
#define SERVER Server::Instance()
#define SOCKLIST std::set<SOCKET>
#define CLIENT_THREAD std::map<SOCKET, std::thread>

void proc_recvs(SOCKET socket);
void recv_client();

class Server
{
public:
	Server();
private:
	static void Destory();
	static Server* instance;

public:
	static Server&	Instance();

public:
	void			Run();

public:
	SOCKLIST*		GetSockList();
	CLIENT_THREAD*	GetClientThread();
	SOCKET			GetServerSocket();
private:
	SOCKLIST		clientSock;
	CLIENT_THREAD	clientThread;

private:
	SOCKET			serverSock;
};