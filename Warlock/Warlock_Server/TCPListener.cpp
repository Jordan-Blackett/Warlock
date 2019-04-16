#include "TCPListener.h"



TCPListener::TCPListener(std::string ipAddress, int port, std::function<void(SOCKET*)> handler)
	: ipAddress_(ipAddress), port_(port), connectionRecieved_(handler)
{
}


TCPListener::~TCPListener()
{
	CleanUp();
}

bool TCPListener::Init()
{
	listeningSocket = CreateListenSocket();
	if (listeningSocket == INVALID_SOCKET)
	{
		std::cout << "listeningSocket INVALID_SOCKET" << std::endl;
		system("pause");
		return false;
	}

	FD_ZERO(&readfds_);
	FD_ZERO(&exceptfds_);

	//std::cout << "TCP Listener Successful." << std::endl;
	return true;
}

void TCPListener::listen()
{
	while (true)
	{
		FD_ZERO(&readfds_);
		FD_ZERO(&exceptfds_);

		// Add listen socket to readfds and exceptfds
		FD_SET(listeningSocket, &readfds_);
		FD_SET(listeningSocket, &exceptfds_);

		select(0, &readfds_, NULL, &exceptfds_, NULL);

		if (FD_ISSET(listeningSocket, &exceptfds_))
		{
			// CLose down listener
			PrintExceptionalCondition(listeningSocket);
			std::cout << "Winsock error occurred on listeningSocket. Ending server." << std::endl;
			closesocket(listeningSocket);
			break;
		}

		// if listenSocket was in readfds_
		if (FD_ISSET(listeningSocket, &readfds_))
		{
			SOCKET newClientSocket = WaitForConnection(listeningSocket);
			if (newClientSocket != INVALID_SOCKET)
			{
				connectionRecieved_(&newClientSocket);
				std::cout << "New connection: " << newClientSocket << std::endl;
			}
			else {
				std::cout << "Connection failed: " << newClientSocket << std::endl;
			}
		}
	}
}

void TCPListener::CleanUp()
{
	closesocket(listeningSocket);
}

SOCKET TCPListener::CreateListenSocket()
{
	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningSocket != INVALID_SOCKET)
	{
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port_);
		inet_pton(AF_INET, ipAddress_.c_str(), &hint.sin_addr);

		if (bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) != SOCKET_ERROR)
		{
			if (::listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
			{
				return -1;
			}

			// Set listen socket - nonblocking
			u_long iMode = 1;
			ioctlsocket(listeningSocket, FIONBIO, &iMode);
		}
		else
		{
			// Unable to bind socket
			CleanUp();
			return -1;
		}
	}

	return listeningSocket;
}

SOCKET TCPListener::WaitForConnection(SOCKET listeningSocket)
{
	SOCKET clientSocket = accept(listeningSocket, NULL, NULL);
	return clientSocket;
}

void TCPListener::PrintExceptionalCondition(SOCKET socket)
{
	int errorCode = 0;
	int errorCodeSize = sizeof(errorCode);
	if (getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)&errorCode, &errorCodeSize) == 0)
	{
		std::cout << "Winsock error: " << errorCode << "." << std::endl;
	}
	else
	{
		std::cout << "Winsock error occurred. getsockopt() failed." << std::endl;
	}
}