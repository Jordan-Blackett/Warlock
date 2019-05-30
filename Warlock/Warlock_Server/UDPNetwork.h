#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include <functional>

class UDPNetwork
{
public:
	UDPNetwork(int port, std::function<void(const std::string*)> handler);
	~UDPNetwork();

	SOCKET* Init();

	void Run();

private:
	SOCKET UDPSocket_;

	int port_;

	std::function<void(const std::string*)> SendMessageSystem;
};

