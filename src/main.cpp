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
#include "TcpServer.h"



using namespace std;



TcpServer tcpServer(5000, 8);

int main() 
{ 
	
	Log& log = Log::getInstance();
	log.FileName = "Paradox.log";
	log.MaxFileSize = 300000;
	log.MaxFilesCount = 2;
	log.Initialize();
	printf("Server start up");
	LOG4CPLUS_DEBUG(Log::getLogger(), "Paradox back-end start up");
	tcpServer.Start();


    return 0;
}

