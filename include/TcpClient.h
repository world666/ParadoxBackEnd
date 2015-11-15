#ifndef TCP_CLIENT
#define TCP_CLIENT
#include <iostream>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef std::string (*RequestCallback)(std::string request);

class TcpClient
{
public:
	TcpClient(int client, RequestCallback requestCallback);
	void ReInit(int client);
	static void* _HandleRequest(void* data);
	void HandleRequest();
	volatile bool free;
	pthread_cond_t  conditionVar;
	pthread_mutex_t conditionMutex;
private:

	RequestCallback _requestCallback;
	int _client;
	pthread_t _thread;
	const int _intervalMs;
	const int _effortCount;
	std::string _request;
	std::string _content;
};
#endif
