#include <iostream>
#include <string>
#include <thread>
#include <functional>

//#include "TCPListener.h"
#include "Network1.h"
#include "Warlock.h"

//void Listener_MessageReceived(TCPListener* listener, int client, std::string msg);
void InitGame();

using namespace std::placeholders;

void run()
{
	Warlock gameWarlock;
	if (gameWarlock.Init())
	{
		gameWarlock.Run();
	}
}

int main() {

	std::cout << "Hello World Server" << std::endl;

	//std::thread game(run);

	//Network1 server

	Network1 server;
	if (server.Init())
	{
		server.run();
	}

	// Create a listener socket and make it wait for new connections on port 54000
	//TCPListener listener("127.0.0.1", PORT, Listener_MessageReceived);
	//if (listener.Init())
	//{
	//	listener.listen();
	//}

	system("pause");

	return 0;
}

void InitGame()
{

}

void Listener_MessageReceived(TCPListener* listener, int client, std::string msg)
{
	//listener->Send(client, msg);
}

