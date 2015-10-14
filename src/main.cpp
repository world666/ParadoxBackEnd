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
#include <sys/types.h>
#include <time.h> 

#include "log.h"

using namespace std;

class TcpClient
{
public:
	TcpClient(int client)
	{
		_client = client;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		printf("create thread!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		pthread_create(&_thread, &attr, &TcpClient::HandleRequest, NULL);
	}
	static void* HandleRequest(void* data)
	{
		printf("thread started");
		return NULL;
	}
private:


	int _client;
	pthread_t _thread;
};


int main() 
{ 
	
	Log& log = Log::getInstance();
	log.FileName = "Paradox.log";
	log.MaxFileSize = 300000;
	log.MaxFilesCount = 2;
	log.Initialize();
	LOG4CPLUS_DEBUG(Log::getLogger(), "Paradox back-end start up");

	int listenfd = 0, connfd = 0, n = 0;
    struct sockaddr_in serv_addr;

	bool ret = false;

    char sendBuff[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 14\n\nandrey kyrylov";
	char recvBuff[1025];
    //time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 5);

	printf("Server start up\n");
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

		printf("client connected\n");
		ret = false;
		while(1)
		{
			ioctl(connfd, FIONREAD, &n);
			printf("available: %d", n);
			if(n > 0)
			{
				n = recv(connfd, recvBuff, sizeof(recvBuff)-1, 0);
				recvBuff[n] = 0;
				printf("%s", recvBuff);
				ret = true;
			}
			if(ret)
			{
				printf("create client\n");
				TcpClient* clien = new TcpClient(connfd);
				break;
			}
			printf("sleep\n");
			sleep(1);
		}

		
		

        //ticks = time(NULL);
        //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
		
        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
     }

    return 0;
}

