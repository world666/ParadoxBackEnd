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
#include "TcpClient.h"


using namespace std;





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
    TcpClient* clients[2];

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
		for(int iClient = 0; iClient<2; iClient++)
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

