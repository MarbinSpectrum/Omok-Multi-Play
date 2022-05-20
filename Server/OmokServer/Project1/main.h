#pragma once

#include <winsock2.h>
#include <iostream>
#include <thread>
#include <set>
#include <map>

#pragma comment(lib, "ws2_32.lib")

#define PACKET_SIZE 1024 

SOCKET skt, client_sock;
SOCKADDR_IN client = {};

std::set<SOCKET> clientSock;
std::map<SOCKET, std::thread> clientThread;

void proc_recvs(SOCKET socket);