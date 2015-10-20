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

using namespace std;




TcpClient::TcpClient(int client) : intervalMs(50), effortCount(10)
{
	_client = client;
	free = false;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	LOG4CPLUS_DEBUG(Log::getLogger(), "create thread");
	pthread_create(&_thread, &attr, &TcpClient::_HandleRequest, this);
}

void* TcpClient::_HandleRequest(void* data)
{
	TcpClient* tcpClient = static_cast<TcpClient*>(data);
	LOG4CPLUS_DEBUG(Log::getLogger(), "thread started");
	tcpClient->HandleRequest();
	return NULL;
}

void TcpClient::HandleRequest()
{
	int n = 0;
	string request;
	bool headFinished = false;
	bool contentExsists = false;
	bool reqFinished = false;
	char sendBuff[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 14\n\nandrey kyrylov";
	bool ret = false;
	int iCount = 0;
	while(iCount < effortCount)
	{
		ioctl(_client, FIONREAD, &n);
		LOG4CPLUS_DEBUG(Log::getLogger(), ToString("available: %d", n).c_str());
		if(n > 0)
		{
			n = recv(_client, recvBuff, sizeof(recvBuff)-1, 0);
			recvBuff[n] = 0;
			request.append(recvBuff);
			if(request.find("\r\n\r\n") != string::npos)
			{
				LOG4CPLUS_DEBUG(Log::getLogger(), "head finished");
				headFinished = true;
				if(request.find("Content-Length:") != string::npos)
				{
					contentExsists = true;
				}
				else
				{
					LOG4CPLUS_DEBUG(Log::getLogger(), "request finished");
					break;
				}
			}
			usleep(10);
			ret = true;
			continue;
		}
		if(ret)
		{
			break;
		}
		LOG4CPLUS_DEBUG(Log::getLogger(), ToString("sleep: %d", intervalMs).c_str());
		usleep(intervalMs);
		iCount++;
	}
	printf("%s", request.c_str());
	write(_client, sendBuff, strlen(sendBuff));
	close(_client);
}
