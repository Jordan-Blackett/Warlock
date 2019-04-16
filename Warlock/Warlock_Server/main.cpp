#include <iostream>
#include <thread>
#pragma comment (lib, "ws2_32.lib")

#include "Network1.h"
#include "Warlock.h"

#include "MessagingSystem.h"

void run(MessagingSystem* messageBus)
{
	Warlock gameWarlock(messageBus);
	if (gameWarlock.Init())
	{
		gameWarlock.Run();
	}
}

int main() {
	std::cout << "Hello World Server" << std::endl;

	MessagingSystem messageBus;
	
	Network1 server(&messageBus);
	if (server.Init())
	{
		std::thread game(run, &messageBus);
		server.run();
	}

	system("pause");

	return 0;
}