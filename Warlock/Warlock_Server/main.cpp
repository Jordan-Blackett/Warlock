#include <iostream>
#include <string>

#include "TCPListener.h"

#define PORT 54000
#define SOCKET_BUFFER_SIZE 4096

void Listener_MessageReceived(TCPListener* listener, int client, std::string msg);

int main() {
	std::cout << "Hello World Server" << std::endl;

	// Create a listener socket and make it wait for new connections on port 54000
	TCPListener listener("127.0.0.1", PORT, Listener_MessageReceived);
	if (listener.Init())
	{
		listener.listen();
	}

	system("pause");

	return 0;
}

void Listener_MessageReceived(TCPListener* listener, int client, std::string msg)
{
	listener->Send(client, msg);
}