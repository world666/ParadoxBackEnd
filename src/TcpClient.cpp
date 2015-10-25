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
#include <vector>

#include "log.h"
#include "TcpClient.h"


using namespace std;




TcpClient::TcpClient(int client) : _intervalMs(50), _effortCount(10)
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
	char recvBuff[1024];
	char sendBuff[] = "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nContent-Type: json\nContent-Length: 19\n\n{\"name\" : \"andrii\"}";
	int contentPos = 0;
	int contetnLength = 0;
	unsigned int contentTagPos;
	int iCount = 0;
	while(iCount < _effortCount)
	{
		ioctl(_client, FIONREAD, &n);
		LOG4CPLUS_DEBUG(Log::getLogger(), ToString("available: %d", n).c_str());
		if(n > 0)
		{
			n = recv(_client, recvBuff, sizeof(recvBuff)-1, 0);
			LOG4CPLUS_DEBUG(Log::getLogger(), ToString("read: %d", n).c_str());
			recvBuff[n] = 0;
			_request.append(recvBuff);
			if(_request.find("\r\n\r\n") != string::npos && contentPos == 0)
			{
				contentPos = _request.find("\r\n\r\n") + 4;
				contentTagPos = _request.find("Content-Length:");
				LOG4CPLUS_DEBUG(Log::getLogger(), "head finished");
				if(contentTagPos != string::npos)
				{

					sscanf (_request.c_str() + contentTagPos,"Content-Length: %d", &contetnLength);
					LOG4CPLUS_DEBUG(Log::getLogger(), ToString("content-length: %d", contetnLength).c_str());
				}
				else
				{
					LOG4CPLUS_DEBUG(Log::getLogger(), "request finished");
					break;
				}
			}
			if(contentPos && (_request.length() - contentPos) == contetnLength)
			{
				LOG4CPLUS_DEBUG(Log::getLogger(), "request finished");
				_content = _request.substr(contentPos, contetnLength);
				break;
			}
			usleep(_intervalMs);
			continue;
		}
		LOG4CPLUS_DEBUG(Log::getLogger(), ToString("sleep: %d", _intervalMs).c_str());
		usleep(_intervalMs);
		iCount++;
	}
	LOG4CPLUS_DEBUG(Log::getLogger(), _request.c_str());
	write(_client, sendBuff, strlen(sendBuff));
	close(_client);
	free = true;
}
