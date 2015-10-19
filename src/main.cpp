#include <iostream>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cstdarg>
#include <sys/types.h>
#include <time.h> 

#include "log.h"

using namespace std;


std::string ToString(const char* fmt, ...){
    int size = 512;
    char* buffer = 0;
    buffer = new char[size];
    va_list vl;
    va_start(vl, fmt);
    int nsize = vsnprintf(buffer, size, fmt, vl);
    if(size<=nsize){ //fail delete buffer and try again
        delete[] buffer;
        buffer = 0;
        buffer = new char[nsize+1]; //+1 for /0
        nsize = vsnprintf(buffer, size, fmt, vl);
    }
    std::string ret(buffer);
    va_end(vl);
    delete[] buffer;
    return ret;
}

class TcpClient
{
public:
	TcpClient(int client) : intervalMs(50), effortCount(10)
	{
		_client = client;
		free = false;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		LOG4CPLUS_DEBUG(Log::getLogger(), "create thread");
		pthread_create(&_thread, &attr, &TcpClient::_HandleRequest, this);
	}
	static void* _HandleRequest(void* data)
	{
		TcpClient* tcpClient = static_cast<TcpClient*>(data);
		LOG4CPLUS_DEBUG(Log::getLogger(), "thread started");
		tcpClient->HandleRequest();
		return NULL;
	}
	void HandleRequest()
	{
		int n = 0;
		string request;
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
				if(request.find("\n\r\n\r") != string::npos)
				{
					LOG4CPLUS_DEBUG(Log::getLogger(), "request finished");
					break;
				}
				usleep(10);
				ret = true;
				continue;
			}
			if(ret)
			{
				printf("%s", request.c_str());
				break;
			}
			LOG4CPLUS_DEBUG(Log::getLogger(), ToString("sleep: %d", intervalMs).c_str());
			usleep(intervalMs);
			iCount++;
		}
		write(_client, sendBuff, strlen(sendBuff));
		close(_client);
	}
	bool free;
private:
	int _client;
	pthread_t _thread;
	const int intervalMs;
	const int effortCount;
	char recvBuff[1025];
};



int main() 
{ 
	
	Log& log = Log::getInstance();
	log.FileName = "Paradox.log";
	log.MaxFileSize = 300000;
	log.MaxFilesCount = 2;
	log.Initialize();
	LOG4CPLUS_DEBUG(Log::getLogger(), "Paradox back-end start up");

	int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr client_addr;
    struct sockaddr_in *client_addr_in;
    char ipstr[INET6_ADDRSTRLEN + 1];
    int port;
    TcpClient* clients[10];

	socklen_t c_len = sizeof(client_addr);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    memset(clients, NULL, sizeof(clients));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 5);

	printf("Server start up\n");
    while(true)
    {
        connfd = accept(listenfd, &client_addr, &c_len);
        client_addr_in = (struct sockaddr_in *)&client_addr;
        inet_ntop(AF_INET, &client_addr_in->sin_addr, ipstr, sizeof ipstr);
        port = ntohs(client_addr_in->sin_port);
        LOG4CPLUS_DEBUG(Log::getLogger(), ToString("client connected addr=%s; port=%d", ipstr, port).c_str());
		for(int iClient = 0; iClient<10; iClient++)
		{
			if(clients[iClient] == NULL)
			{
				clients[iClient] = new TcpClient(connfd);
				break;
			}
			else if(clients[iClient]->free)
			{
				delete clients[iClient];
				clients[iClient] = new TcpClient(connfd);
				break;
			}
		}
     }

    return 0;
}

