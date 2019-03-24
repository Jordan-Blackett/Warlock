#include "TCPListener.h"

TCPListener::TCPListener()
{

}

TCPListener::TCPListener(std::string ipAddress, int port, std::function<void(SOCKET*)> handler)
	: ipAddress_(ipAddress), port_(port), connectionRecieved(handler)
{
}


TCPListener::~TCPListener()
{
	CleanUp();
}

bool TCPListener::Init()
{
	// Initialze winsock
	WSADATA winsockData;
	WORD winsockVersion = MAKEWORD(2, 2); //0x202;

	if (WSAStartup(winsockVersion, &winsockData))
	{
		std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
		system("pause");
		return false;
	}

	FD_ZERO(&readfds_);
	FD_ZERO(&writefds_);
	FD_ZERO(&exceptfds_);

	return true;
}

void TCPListener::listen()
{
	//char buf[MAX_BUFFER_SIZE];

	SOCKET listeningSocket = CreateListenSocket();
	if (listeningSocket == INVALID_SOCKET)
	{
	}

	//while (true)
	//{
		FD_ZERO(&readfds_);
		FD_ZERO(&writefds_);
		FD_ZERO(&exceptfds_);

		// Add listen socket to readfds and exceptfds
		FD_SET(listeningSocket, &readfds_); 
		FD_SET(listeningSocket, &exceptfds_);

	/*	for (int i = 0; i < connections_.size(); i++)
		{
			FD_SET(connections_[i].socket_, &readfds_);
			FD_SET(connections_[i].socket_, &writefds_);
			FD_SET(connections_[i].socket_, &exceptfds_);
		}*/

		select(0, &readfds_, &writefds_, &exceptfds_, NULL);

		//if (FD_ISSET(listeningSocket, &exceptfds_))
		//{
		//	// CLose down server
		//	PrintExceptionalCondition(listeningSocket);
		//	std::cout << "Winsock error occurred on listeningSocket. Ending server." << std::endl;
		//	closesocket(listeningSocket);
		//	for (int i = 0; i < connections_.size(); i++)
		//	{
		//		closesocket(connections_[i].socket_);
		//	}
		//	break;
		//}

		// if listenSocket was in readfds_
		if (FD_ISSET(listeningSocket, &readfds_))
		{
			SOCKET newClientSocket = WaitForConnection(listeningSocket);
			if (newClientSocket != INVALID_SOCKET)
			{
				//SOCKET* newClientSocket2 = new SOCKET();
				connectionRecieved(&newClientSocket);
				//connectionRecieved(newClientSocket2);
				//connections_.push_back(newClientSocket);
				std::cout << "New connection: " << newClientSocket << std::endl;
			}
			else {
				std::cout << "Connection failed: " << newClientSocket << std::endl;
			}
		}

		//for (int i = 0; i < connections_.size(); i++)
		//{
		//	if (FD_ISSET(connections_[i].socket_, &exceptfds_))
		//	{
		//		PrintExceptionalCondition(listeningSocket);
		//		std::cout << "Winsock error occurred on client:" << connections_[i].socket_ << ". Disconnecting client." << std::endl;
		//		closesocket(connections_[i].socket_);
		//		connections_.erase(connections_.begin() + i);
		//		i -= 1;
		//		continue;
		//	}

		//	if (FD_ISSET(connections_[i].socket_, &readfds_))
		//	{
		//		// MessageLength (16-bits), MessageType (8-bits) and MessageSubType (8-bits)
		//		ZeroMemory(buf, MAX_BUFFER_SIZE);


		//		int bytesReceived = recv(connections_[i].socket_, buf, connections_[i].incomingPacketLength - connections_[i].bytesReceived, 0);
		//		if (bytesReceived > 0)
		//		{
		//			std::cout << "Message: '" << std::string(buf, 0, bytesReceived) << "'" << std::endl;
		//			if (connections_[i].incomingPacketLength == 2)
		//			{
		//				uint16_t u16;
		//				memcpy(&u16, buf, 2);
		//				u16 = htons(u16);
		//				connections_[i].incomingPacketLength = u16; // value of first 16 bytes
		//				std::cout << "Length: " << connections_[i].incomingPacketLength << std::endl;
		//			}

		//			memcpy(connections_[i].buffer + connections_[i].bytesReceived, buf, bytesReceived);
		//			//std::cout << "memcpy " << std::string(connections_[i].buffer, 0, connections_[i].bytesReceived) << " + " << std::string(buf, 0, bytesReceived) << std::endl;

		//			connections_[i].bytesReceived += bytesReceived;
		//			//std::cout << "LEFT: " << connections_[i].incomingPacketLength - connections_[i].bytesReceived << std::endl;
		//			if (connections_[i].bytesReceived >= connections_[i].incomingPacketLength 
		//				&& MessageRecieved != NULL)
		//			{


		//				int headerSize = 6;
		//				uint16_t u16;
		//				u16 = connections_[i].socket_;
		//				std::cout << "Debug Length: " << connections_[i].bytesReceived << std::endl;
		//				memcpy(connections_[i].buffer, &u16, 2);

		//				memcpy(&u16, connections_[i].buffer + 2, 2);
		//				u16 = htons(u16);
		//				std::cout << "Type: " << u16 << std::endl;

		//				memcpy(&u16, connections_[i].buffer + 4, 2);
		//				u16 = htons(u16);
		//				std::cout << "Sub Type: " << u16 << std::endl;

		//				std::cout << "Message: '" << std::string(connections_[i].buffer + headerSize, 0, connections_[i].incomingPacketLength - headerSize) << "'" << std::endl;
		//				MessageRecieved(this, connections_[i].socket_, std::string(connections_[i].buffer, 0, connections_[i].incomingPacketLength));


		//				connections_[i].bytesReceived = 0; 
		//				connections_[i].incomingPacketLength = 2; // 2 = Packet length size (bytes)
		//				ZeroMemory(connections_[i].buffer, MAX_BUFFER_SIZE);
		//			}
		//		}
		//		else if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
		//		{
		//			if (WSAGetLastError() != WSAEWOULDBLOCK)
		//			{
		//				std::cout << "Lost connection to " << connections_[i].socket_ << std::endl;
		//				closesocket(connections_[i].socket_);
		//				connections_.erase(connections_.begin() + i);
		//				i -= 1;
		//				continue;
		//			}
		//		}
		//	}

		//	if (FD_ISSET(connections_[i].socket_, &writefds_))
		//	{
		//		// Send data here
		//	}
		//}
	//}
	//CleanUp();
}

void TCPListener::CleanUp()
{
	WSACleanup();
}

void TCPListener::Send(int clientSocket, std::string msg)
{
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
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
			// unable to bind socket
			//cleanup
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

//char buf[MAX_BUFFER_SIZE];
//
//while (true)
//{
//	SOCKET listeningSocket = CreateSocket();
//	if (listeningSocket == INVALID_SOCKET)
//	{
//		break;
//	}
//
//	SOCKET clientSocket = WaitForConnection(listeningSocket);
//	if (clientSocket != INVALID_SOCKET)
//	{
//		closesocket(listeningSocket);
//
//		int bytesReceived = 0;
//		do
//		{
//			ZeroMemory(buf, MAX_BUFFER_SIZE);
//			bytesReceived = recv(clientSocket, buf, MAX_BUFFER_SIZE, 0);
//			if (bytesReceived > 0)
//			{
//				if (MessageRecieved != NULL)
//				{
//					MessageRecieved(this, clientSocket, std::string(buf, 0, bytesReceived));
//				}
//			}
//		} while (bytesReceived > 0);
//
//		closesocket(clientSocket);
//	}
//}