#ifndef TCP_SERVER
#define TCP_SERVER
#include "TcpClient.h"


class TcpServer
{
public:
	TcpServer(int portNumber, int threadsCount, RequestCallback requestCallback);
	void Start(void);

private:

	RequestCallback _requestCallback;
	int _listenfd;
	int _portNumber;
	int _threadsCount;
	struct sockaddr_in _servAddr;
	TcpClient** _clients;
};

#endif
