#include <iostream>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "log.h"
#include "TcpClient.h"
#include "TcpServer.h"


TcpServer::TcpServer(int portNumber, int threadsCount)
{
	_portNumber = portNumber;
	_threadsCount = threadsCount;
	memset(&_servAddr, '0', sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr.sin_port = htons(_portNumber);
	_listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(_listenfd, (struct sockaddr*)&_servAddr, sizeof(_servAddr));
	listen(_listenfd, 5);
	memset(_clients, 0, sizeof(_clients));
}

void TcpServer::Start()
{
	int connfd = 0;
	struct sockaddr_in client_addr_in;
	char ipstr[INET6_ADDRSTRLEN + 1];
	int port;
	socklen_t c_len = sizeof(client_addr_in);

	LOG4CPLUS_DEBUG(Log::getLogger(),"Server start up");
	while(true)
	{
		connfd = accept(_listenfd, (struct sockaddr *)&client_addr_in, &c_len);
		inet_ntop(AF_INET, &client_addr_in.sin_addr, ipstr, sizeof ipstr);
		port = ntohs(client_addr_in.sin_port);
		LOG4CPLUS_DEBUG(Log::getLogger(), ToString("client connected addr=%s; port=%d", ipstr, port).c_str());
		for(int iClient = 0; iClient<2; iClient++)
		{
			if(_clients[iClient] == NULL)
			{
				_clients[iClient] = new TcpClient(connfd);
				break;
			}
			else if(_clients[iClient]->free)
			{
				delete _clients[iClient];
				_clients[iClient] = new TcpClient(connfd);
				break;
			}
		}
	 }
}
