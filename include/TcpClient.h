#include <stdlib.h>



class TcpClient
{
public:
	TcpClient(int client);
	static void* _HandleRequest(void* data);
	void HandleRequest();



	volatile bool free;
private:
	int _client;
	pthread_t _thread;
	const int intervalMs;
	const int effortCount;
};

