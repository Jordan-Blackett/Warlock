#include "Network1.h"



Network1::Network1(MessagingSystem* messageBus) : BusNode(messageBus)
{
}


Network1::~Network1()
{
}

bool Network1::Init()
{
	using namespace std::placeholders;

	// Initialze winsock
	WSADATA winsockData;
	WORD winsockVersion = MAKEWORD(2, 2); //0x202;

	if (WSAStartup(winsockVersion, &winsockData))
	{
		std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
		system("pause");
		return false;
	}

	listenerThread_ = std::thread(&Network1::InitListener, this);

	return true;
}

void Network1::InitListener()
{
	// Create a listener socket and make it wait for new connections
	using namespace std::placeholders;
	TCPListener listener("127.0.0.1", PORT, std::bind(&Network1::Listener_ConnectionReceived, this, _1));
	listener.Init();
	listener.listen();
}

void Network1::run()
{
	while (true)
	{
		// Message System
		Notify();

		if (connections_.size() == 0)
		{
			continue;
		}

		FD_ZERO(&readfds_);
		//FD_ZERO(&writefds_);
		FD_ZERO(&exceptfds_);

		// Recieve
		for (int i = 0; i < connections_.size(); i++)
		{
			FD_SET(connections_[i].socket_, &readfds_);
			//FD_SET(connections_[i].socket_, &writefds_);
			FD_SET(connections_[i].socket_, &exceptfds_);
		}

		select(0, &readfds_, &writefds_, &exceptfds_, NULL);

		char buf[MAX_BUFFER_SIZE];

		for (int i = 0; i < connections_.size(); i++)
		{
			if (FD_ISSET(connections_[i].socket_, &exceptfds_))
			{
				PrintExceptionalCondition(connections_[i].socket_);
				std::cout << "Winsock error occurred on client:" << connections_[i].socket_ << ". Disconnecting client." << std::endl;
				closesocket(connections_[i].socket_);
				connections_.erase(connections_.begin() + i);
				i -= 1;
				continue;
			}

			if (FD_ISSET(connections_[i].socket_, &readfds_))
			{
				ZeroMemory(buf, MAX_BUFFER_SIZE);

				int bytesReceived = recv(connections_[i].socket_, buf, connections_[i].incomingPacketLength - connections_[i].bytesReceived, 0);
				if (bytesReceived > 0)
				{
					if (connections_[i].incomingPacketLength == 2)
					{
						// Get packet length value of first 2 bytes
						uint16_t packetLength;
						memcpy(&packetLength, buf, 2);
						packetLength = htons(packetLength);
						connections_[i].incomingPacketLength = packetLength;
					}

					memcpy(connections_[i].buffer + connections_[i].bytesReceived, buf, bytesReceived);

					connections_[i].bytesReceived += bytesReceived;
					if (connections_[i].bytesReceived >= connections_[i].incomingPacketLength)
					{
						int headerSize = 6;

						// Client ID - Swap packet length to client ID
						uint16_t u16;
						u16 = connections_[i].socket_;
						memcpy(connections_[i].buffer, &u16, sizeof(uint16_t));

						// Send packet to messaging system
						Message packet(std::string(connections_[i].buffer, 0, connections_[i].incomingPacketLength));
						SendMessageSystem(packet);

						connections_[i].bytesReceived = 0;
						connections_[i].incomingPacketLength = 2; // 2 = Packet length size (bytes)
						ZeroMemory(connections_[i].buffer, MAX_BUFFER_SIZE);
					}
				}
				else if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						std::cout << "Lost connection to " << connections_[i].socket_ << std::endl;
						closesocket(connections_[i].socket_);
						connections_.erase(connections_.begin() + i);
						i -= 1;
						continue;
					}
				}
			}
		}
	}
	CleanUp();
}

void Network1::CleanUp()
{
	for (int i = 0; i < connections_.size(); i++)
	{
		closesocket(connections_[i].socket_);
		connections_.erase(connections_.begin() + i);
	}
}

void Network1::Listener_ConnectionReceived(SOCKET* socket)
{
	connections_.push_back(*socket);
}

void Network1::Send(int clientSocket, std::string msg)
{
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

void Network1::PrintExceptionalCondition(SOCKET socket)
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
