#include "Network1.h"



Network1::Network1()
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

	// Create a listener socket and make it wait for new connections on port 54000
	//TCPListener listener("127.0.0.1", PORT, &Network1::Listener_ConnectionReceived);
	//TCPListener listener2("127.0.0.1", PORT, std::bind(&Network1::Listener_ConnectionReceived, this, _1));
	listener = new TCPListener("127.0.0.1", PORT, std::bind(&Network1::Listener_ConnectionReceived, this, _1));
	//listener = listener2;
	if (listener->Init())
	{
		listener->listen();
	}

	return true;
}

void Network1::run()
{
	while (true)
	{
		listener->listen();

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
				//PrintExceptionalCondition(listeningSocket);
				std::cout << "Winsock error occurred on client:" << connections_[i].socket_ << ". Disconnecting client." << std::endl;
				closesocket(connections_[i].socket_);
				connections_.erase(connections_.begin() + i);
				i -= 1;
				continue;
			}

			if (FD_ISSET(connections_[i].socket_, &readfds_))
			{
				// MessageLength (16-bits), MessageType (8-bits) and MessageSubType (8-bits)
				ZeroMemory(buf, MAX_BUFFER_SIZE);


				int bytesReceived = recv(connections_[i].socket_, buf, connections_[i].incomingPacketLength - connections_[i].bytesReceived, 0);
				if (bytesReceived > 0)
				{
					std::cout << "Message: '" << std::string(buf, 0, bytesReceived) << "'" << std::endl;
					if (connections_[i].incomingPacketLength == 2)
					{
						uint16_t u16;
						memcpy(&u16, buf, 2);
						u16 = htons(u16);
						connections_[i].incomingPacketLength = u16; // value of first 16 bytes
						std::cout << "Length: " << connections_[i].incomingPacketLength << std::endl;
					}

					memcpy(connections_[i].buffer + connections_[i].bytesReceived, buf, bytesReceived);
					//std::cout << "memcpy " << std::string(connections_[i].buffer, 0, connections_[i].bytesReceived) << " + " << std::string(buf, 0, bytesReceived) << std::endl;

					connections_[i].bytesReceived += bytesReceived;
					//std::cout << "LEFT: " << connections_[i].incomingPacketLength - connections_[i].bytesReceived << std::endl;
					if (connections_[i].bytesReceived >= connections_[i].incomingPacketLength)
					{
						int headerSize = 6;
						uint16_t u16;
						u16 = connections_[i].socket_;
						std::cout << "Debug Length: " << connections_[i].bytesReceived << std::endl;
						memcpy(connections_[i].buffer, &u16, 2);

						memcpy(&u16, connections_[i].buffer + 2, 2);
						u16 = htons(u16);
						std::cout << "Type: " << u16 << std::endl;

						memcpy(&u16, connections_[i].buffer + 4, 2);
						u16 = htons(u16);
						std::cout << "Sub Type: " << u16 << std::endl;

						std::cout << "Message: '" << std::string(connections_[i].buffer + headerSize, 0, connections_[i].incomingPacketLength - headerSize) << "'" << std::endl;
						//MessageRecieved(this, connections_[i].socket_, std::string(connections_[i].buffer, 0, connections_[i].incomingPacketLength));

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
	//CleanUp();
}

void Network1::Listener_ConnectionReceived(SOCKET* socket)
{
	connections_.push_back(*socket);
}
