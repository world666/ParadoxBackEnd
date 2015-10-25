#ifndef TCP_CLIENT
#define TCP_CLIENT
#include <iostream>
#include <string.h>


typedef std::string (*RequestCallback)(std::string request);

class TcpClient
{
public:
	TcpClient(int client);
	static void* _HandleRequest(void* data);
	void HandleRequest();
	volatile bool free;
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
