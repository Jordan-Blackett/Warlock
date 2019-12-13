#include "btpch.h"
#include "Network.h"

namespace BlackThorn {

	//bool Network::m_TCPClose = false;
	//bool Network::m_UDPClose = false;

	bool Network::Initialize()
	{
		// Initialze winsock
		WSADATA winsockData;
		WORD winsockVersion = MAKEWORD(2, 2); //0x202;
		if (WSAStartup(winsockVersion, &winsockData))
		{
			BT_CORE_ERROR("NETWORK: WSAStartup failed: {0}", WSAGetLastError());
			return false;
		}

		BT_CORE_INFO("NETWORK: WSAStartup Successfull.");

		//char localIP[10] = "127.0.0.1";
		//m_ListenerThread = std::thread(Network::InitializeTCP, localIP, PORT);

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// TCP /////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Network::InitializeTCP(const char* address, int port)
	{
		BT_CORE_WARN("NETWORK: Establishing TCP Listener...");
		SOCKET listeningSocket = CreateListenSocket(address, port);
		if (listeningSocket == INVALID_SOCKET)
		{
			BT_CORE_ERROR("NETWORK: ListeningSocket - INVALID_SOCKET.");
			system("pause");
			//return false;
		}
		BT_CORE_INFO("NETWORK: TCP Listener Established.");

		BT_CORE_WARN("NETWORK: Waiting for connections...");
		while (!m_TCPClose) {
			AcceptConnections(listeningSocket);
			BT_CORE_INFO("Acceptor restarting...");
		}

		BT_CORE_INFO("NETWORK: TCP Ending.");
		//return true;
	}

	void Network::InitializeUDP(const char* address, int port)
	{
		BT_CORE_WARN("NETWORK: Establishing UDP Socket...");
		sockaddr_in hint = sockaddr_in();
		SOCKET UDPSocket = CreateUDPSocket(address, port, hint);
		if (UDPSocket == INVALID_SOCKET)
		{
			BT_CORE_ERROR("NETWORK: UDPSocket - INVALID_SOCKET.");
			system("pause");
		}
		BT_CORE_INFO("NETWORK: UDP Established.");
		UDPConsumer(UDPSocket, hint);

		BT_CORE_INFO("NETWORK: UDP Ending.");
	}

	void Network::CleanUp()
	{
		m_TCPClose = true;
		m_UDPClose = true;

		WSACleanup();
	}

	void Network::Close()
	{
	}

	SOCKET Network::CreateListenSocket(const char* address, int port)
	{
		u_long interfaceAddr = inet_addr(address);
		//u_long interfaceAddr = InetPton(AF_INET, address, &interfaceAddr);
		if (interfaceAddr != INADDR_NONE)
		{
			SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (listeningSocket != INVALID_SOCKET)
			{
				sockaddr_in hint;
				hint.sin_family = AF_INET;
				//hint.sin_addr.s_addr = interfaceAddr;
				hint.sin_addr.s_addr = INADDR_ANY;
				hint.sin_port = htons(port);

				if (bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) != SOCKET_ERROR)
				{
					if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
					{
						return INVALID_SOCKET;
					}
				}
				else
				{
					// Failed to bind socket
					BT_CORE_ERROR("NETWORK: Unable To Bind Listen Socket.");
					CleanUp();
					return INVALID_SOCKET;
				}
			}
			return listeningSocket;
		}
		return INVALID_SOCKET;
	}

	void Network::AcceptConnections(SOCKET ListeningSocket)
	{
		sockaddr_in sinRemote;
		int addrSize = sizeof(sinRemote);

		while (true) {
			SOCKET sd = accept(ListeningSocket, (sockaddr*)&sinRemote, &addrSize);
			if (sd != INVALID_SOCKET) {
				BT_CORE_INFO("NETWORK: Accepted Connection From {0} : {1}", inet_ntoa(sinRemote.sin_addr), ntohs(sinRemote.sin_port));

				DWORD threadID;
				//CreateThread(0, 0, TCPConsumer, (void*)sd, 0, &threadID);
			}
			else {
				BT_CORE_WARN("NETWORK: Accept() Failed.");
				return;
			}
		}
	}

	DWORD WINAPI Network::TCPConsumer(void * sd_)
	{
		int retval = 0;
		SOCKET sd = (SOCKET)sd_;

		if (!EchoIncomingPackets(sd)) {
			BT_CORE_ERROR("NETWORK: Echo incoming packets failed");
			retval = 3;
		}

		BT_CORE_WARN("NETWORK: Shutting connection down...");
		if (ShutdownConnection(sd) != SOCKET_ERROR) {
			BT_CORE_INFO("NETWORK: Connection is down");
		}
		else {
			BT_CORE_ERROR("NETWORK: Connection shutdown failed");
			retval = 3;
		}

		return retval;
	}

	bool Network::EchoIncomingPackets(SOCKET sd)
	{
		// Read data from client
		char acReadBuffer[BUFFER_SIZE];
		int readBytes;
		do {
			readBytes = recv(sd, acReadBuffer, BUFFER_SIZE, 0);
			if (readBytes > 0) {
				BT_CORE_TRACE("NETWORK: TCP - Received {0} bytes from client.", readBytes);
				int sentBytes = 0;
				while (sentBytes < readBytes) {
					int temp = send(sd, acReadBuffer + sentBytes,
						readBytes - sentBytes, 0);
					if (temp > 0) {
						BT_CORE_TRACE("NETWORK: TCP - Sent {0} bytes back to client.", temp);
						sentBytes += temp;
					}
					else if (temp == SOCKET_ERROR) {
						return false;
					}
					else {
						// Client closed connection before we could reply to
						// all the data it sent, so bomb out early.
						BT_CORE_TRACE("NETWORK: Peer unexpectedly dropped connection!", temp);
						return true;
					}
				}
			}
			else if (readBytes == SOCKET_ERROR) {
				return false;
			}
		} while (readBytes != 0);

		BT_CORE_WARN("Connection closed by peer.");
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// UDP /////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SOCKET Network::CreateUDPSocket(const char * address, int port, sockaddr_in outhint)
	{
		u_long interfaceAddr = inet_addr(address);
		if (interfaceAddr != INADDR_NONE)
		{
			SOCKET UDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (UDPSocket != INVALID_SOCKET)
			{
				sockaddr_in hint;
				hint.sin_family = AF_INET;
				hint.sin_addr.s_addr = interfaceAddr;
				hint.sin_port = htons(port);
				outhint = hint;

				if (bind(UDPSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
				{
					// Failed to bind socket
					BT_CORE_ERROR("NETWORK: Unable To Bind UDP Socket.");
					// CleanUp();
					return INVALID_SOCKET;
				}
			}
			return UDPSocket;
		}
		return INVALID_SOCKET;
	}

	DWORD Network::UDPConsumer(SOCKET udpsocket, sockaddr_in hint)
	{
		int retval = 0;

		if (!EchoIncomingUDPPackets(udpsocket, hint)) {
			BT_CORE_ERROR("NETWORK: Echo incoming packets failed");
			retval = 3;
		}

		BT_CORE_WARN("NETWORK: Shutting connection down...");
		if (ShutdownConnection(udpsocket) != SOCKET_ERROR) {
			BT_CORE_INFO("NETWORK: Connection is down");
		}
		else {
			BT_CORE_ERROR("NETWORK: Connection shutdown failed");
			retval = 3;
		}

		return retval;
	}

	bool Network::EchoIncomingUDPPackets(SOCKET udpsocket, sockaddr_in hint)
	{
		SOCKADDR_IN client;
		ZeroMemory(&client, sizeof(client));
		int clientLength = sizeof(client);

		// Read data from client
		char acReadBuffer[BUFFER_SIZE];
		int flags = 0;

		int readBytes;
		do {
			readBytes = recvfrom(udpsocket, acReadBuffer, BUFFER_SIZE, flags, (SOCKADDR*)&client, &clientLength);
			if (readBytes > 0) {
				BT_CORE_TRACE("NETWORK: UDP - Received {0} bytes from client.", readBytes);
				int sentBytes = 0;
				while (sentBytes < readBytes) {
					int temp = sendto(udpsocket, acReadBuffer + sentBytes, readBytes - sentBytes, 0, (sockaddr*)&hint, sizeof(hint));
					if (temp > 0) {
						BT_CORE_TRACE("NETWORK: UDP - Sent {0} bytes back to client.", temp);
						sentBytes += temp;
					}
					else if (temp == SOCKET_ERROR) {
						return false;
					}
					else {
						// Client closed connection before we could reply to
						// all the data it sent, so bomb out early.
						BT_CORE_TRACE("NETWORK: Peer unexpectedly dropped connection!", temp);
						return true;
					}
				}
			}
			else if (readBytes == SOCKET_ERROR) {
				return false;
			}
		} while (readBytes != 0);

		BT_CORE_WARN("Connection closed by peer.");
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ... /////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Network::ShutdownConnection(SOCKET sd)
	{
		return closesocket(sd);
	}

	void Network::Bind(const NetworkCallBack callbackName, const NetCallbackFn& callback)
	{
		host->Bind(callbackName, callback);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SERVER //////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Network::CreateServer(const int port, const int maxplayers)
	{
		//ENetAddress address;

		//enet_address_set_ip(&address, bind_ip.get_ipv6(), 16);

		//address.port = p_port;

		//host = enet_host_create(&address /* the address to bind the server host to */,
		//	p_max_clients /* allow up to 32 clients and/or outgoing connections */,
		//	channel_count /* allow up to channel_count to be used */,
		//	p_in_bandwidth /* limit incoming bandwidth if > 0 */,
		//	p_out_bandwidth /* limit outgoing bandwidth if > 0 */);

		host = new NetworkServer(); //NetworkServer(address);
		if (host->CreateServer(port, maxplayers))
		{
			m_Active = true;
			server = true;
			//connection_status = CONNECTION_CONNECTED;

			return true;
		}
		return false;
	}

	bool NetworkServer::CreateServer(const int port, const int maxplayers)
	{
		m_TCPListenerThread = std::thread(&NetworkServer::InitializeTCP, this, IP, PORT);
		m_UDPThread = std::thread(&NetworkServer::InitializeUDP, this, IP, PORT);
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// TCP /////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void NetworkServer::InitializeTCP(const char* address, int port)
	{
		BT_CORE_WARN("NETWORK: Establishing TCP Listener...");
		SOCKET listeningSocket = CreateListenSocket(address, port);
		if (listeningSocket == INVALID_SOCKET)
		{
			BT_CORE_ERROR("NETWORK: ListeningSocket - INVALID_SOCKET.");
			system("pause");
			//return false;
		}
		BT_CORE_INFO("NETWORK: TCP Listener Established.");

		BT_CORE_WARN("NETWORK: Waiting for connections...");
		while (!m_TCPClose) {
			AcceptConnections(listeningSocket);
			BT_CORE_INFO("Acceptor restarting...");
		}

		BT_CORE_INFO("NETWORK: TCP Ending.");
		//return true;
	}

	void NetworkServer::InitializeUDP(const char* address, int port)
	{
		BT_CORE_WARN("NETWORK: Establishing UDP Socket...");
		sockaddr_in hint = sockaddr_in();
		SOCKET UDPSocket = CreateUDPSocket(address, port, hint);
		if (UDPSocket == INVALID_SOCKET)
		{
			BT_CORE_ERROR("NETWORK: UDPSocket - INVALID_SOCKET.");
			system("pause");
		}
		BT_CORE_INFO("NETWORK: UDP Established.");
		UDPConsumer(UDPSocket, hint);

		BT_CORE_INFO("NETWORK: UDP Ending.");
	}

	void NetworkServer::CleanUp()
	{
		m_TCPClose = true;
		m_UDPClose = true;

		WSACleanup();
	}

	void NetworkServer::Close()
	{
	}

	SOCKET NetworkServer::CreateListenSocket(const char* address, int port)
	{
		u_long interfaceAddr = inet_addr(address);
		//u_long interfaceAddr = InetPton(AF_INET, address, &interfaceAddr);
		if (interfaceAddr != INADDR_NONE)
		{
			SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (listeningSocket != INVALID_SOCKET)
			{
				sockaddr_in hint;
				hint.sin_family = AF_INET;
				//hint.sin_addr.s_addr = interfaceAddr;
				hint.sin_addr.s_addr = INADDR_ANY;
				hint.sin_port = htons(port);

				if (bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) != SOCKET_ERROR)
				{
					if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
					{
						return INVALID_SOCKET;
					}
				}
				else
				{
					// Failed to bind socket
					BT_CORE_ERROR("NETWORK: Unable To Bind Listen Socket.");
					CleanUp();
					return INVALID_SOCKET;
				}
			}
			return listeningSocket;
		}
		return INVALID_SOCKET;
	}

	void NetworkServer::AcceptConnections(SOCKET ListeningSocket)
	{
		sockaddr_in sinRemote;
		int addrSize = sizeof(sinRemote);

		while (true) {
			SOCKET sd = accept(ListeningSocket, (sockaddr*)&sinRemote, &addrSize);
			if (sd != INVALID_SOCKET) {
				BT_CORE_INFO("NETWORK: Accepted Connection From {0} : {1}", inet_ntoa(sinRemote.sin_addr), ntohs(sinRemote.sin_port));

				ConnectionReceived(sd);

				DWORD threadID;
				CreateThread(0, 0, NetworkServer::TCPConsumer, (void*)sd, 0, &threadID);
			}
			else {
				BT_CORE_WARN("NETWORK: Accept() Failed.");
				return;
			}
		}
	}

	DWORD WINAPI NetworkServer::TCPConsumer(void* sd_)
	{
		int retval = 0;
		SOCKET sd = (SOCKET)sd_;

		if (!TCPReceive(sd)) {
			BT_CORE_ERROR("NETWORK: Echo incoming packets failed");
			retval = 3;
		}

		BT_CORE_WARN("NETWORK: Shutting connection down...");
		if (ShutdownConnection(sd) != SOCKET_ERROR) {
			BT_CORE_INFO("NETWORK: Connection is down");
		}
		else {
			BT_CORE_ERROR("NETWORK: Connection shutdown failed");
			retval = 3;
		}

		return retval;
	}

	bool NetworkServer::TCPReceive(SOCKET sd)
	{
		// Read data from client
		char acReadBuffer[BUFFER_SIZE];
		int readBytes;
		do {
			readBytes = recv(sd, acReadBuffer, BUFFER_SIZE, 0);
			if (readBytes > 0) {
				BT_CORE_TRACE("NETWORK: TCP - Received {0} bytes from client.", readBytes);
			}
			else if (readBytes == SOCKET_ERROR) {
				//return false;
			}
		} while (readBytes != 0);

		BT_CORE_WARN("Connection closed by peer.");
		return true;
	}

	bool NetworkServer::EchoIncomingPackets(SOCKET sd)
	{
		// Read data from client
		char acReadBuffer[BUFFER_SIZE];
		int readBytes;
		do {
			readBytes = recv(sd, acReadBuffer, BUFFER_SIZE, 0);
			if (readBytes > 0) {
				BT_CORE_TRACE("NETWORK: TCP - Received {0} bytes from client.", readBytes);
				int sentBytes = 0;
				while (sentBytes < readBytes) {
					int temp = send(sd, acReadBuffer + sentBytes,
						readBytes - sentBytes, 0);
					if (temp > 0) {
						BT_CORE_TRACE("NETWORK: TCP - Sent {0} bytes back to client.", temp);
						sentBytes += temp;
					}
					else if (temp == SOCKET_ERROR) {
						return false;
					}
					else {
						// Client closed connection before we could reply to
						// all the data it sent, so bomb out early.
						BT_CORE_TRACE("NETWORK: Peer unexpectedly dropped connection!", temp);
						return true;
					}
				}
			}
			else if (readBytes == SOCKET_ERROR) {
				return false;
			}
		} while (readBytes != 0);

		BT_CORE_WARN("Connection closed by peer.");
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// UDP /////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SOCKET NetworkServer::CreateUDPSocket(const char* address, int port, sockaddr_in outhint)
	{
		u_long interfaceAddr = inet_addr(address);
		if (interfaceAddr != INADDR_NONE)
		{
			SOCKET UDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (UDPSocket != INVALID_SOCKET)
			{
				sockaddr_in hint;
				hint.sin_family = AF_INET;
				hint.sin_addr.s_addr = interfaceAddr;
				hint.sin_port = htons(port);
				outhint = hint;

				if (bind(UDPSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
				{
					// Failed to bind socket
					BT_CORE_ERROR("NETWORK: Unable To Bind UDP Socket.");
					// CleanUp();
					return INVALID_SOCKET;
				}
			}
			return UDPSocket;
		}
		return INVALID_SOCKET;
	}

	DWORD NetworkServer::UDPConsumer(SOCKET udpsocket, sockaddr_in hint)
	{
		int retval = 0;

		if (!EchoIncomingUDPPackets(udpsocket, hint)) {
			BT_CORE_ERROR("NETWORK: Echo incoming packets failed");
			retval = 3;
		}

		BT_CORE_WARN("NETWORK: Shutting connection down...");
		if (ShutdownConnection(udpsocket) != SOCKET_ERROR) {
			BT_CORE_INFO("NETWORK: Connection is down");
		}
		else {
			BT_CORE_ERROR("NETWORK: Connection shutdown failed");
			retval = 3;
		}

		return retval;
	}

	bool NetworkServer::EchoIncomingUDPPackets(SOCKET udpsocket, sockaddr_in hint)
	{
		SOCKADDR_IN client;
		ZeroMemory(&client, sizeof(client));
		int clientLength = sizeof(client);

		// Read data from client
		char acReadBuffer[BUFFER_SIZE];
		int flags = 0;

		int readBytes;
		do {
			readBytes = recvfrom(udpsocket, acReadBuffer, BUFFER_SIZE, flags, (SOCKADDR*)&client, &clientLength);
			if (readBytes > 0) {
				BT_CORE_TRACE("NETWORK: UDP - Received {0} bytes from client.", readBytes);
				int sentBytes = 0;
				while (sentBytes < readBytes) {
					int temp = sendto(udpsocket, acReadBuffer + sentBytes, readBytes - sentBytes, 0, (sockaddr*)&hint, sizeof(hint));
					if (temp > 0) {
						BT_CORE_TRACE("NETWORK: UDP - Sent {0} bytes back to client.", temp);
						sentBytes += temp;
					}
					else if (temp == SOCKET_ERROR) {
						return false;
					}
					else {
						// Client closed connection before we could reply to
						// all the data it sent, so bomb out early.
						BT_CORE_TRACE("NETWORK: Peer unexpectedly dropped connection!", temp);
						return true;
					}
				}
			}
			else if (readBytes == SOCKET_ERROR) {
				return false;
			}
		} while (readBytes != 0);

		BT_CORE_WARN("Connection closed by peer.");
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ... /////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool NetworkServer::ShutdownConnection(SOCKET sd)
	{
		return closesocket(sd);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// CALLBACKS ///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void NetworkServer::Bind(const NetworkCallBack callbackName, const NetCallbackFn& callback)
	{
		switch (callbackName) {
		case NetworkCallBack::ConnectionReceived:
			TestCallback = callback;
			break;
		}
	}

	void NetworkServer::ConnectionReceived(SOCKET newSocket)
	{
		m_ClientConnections.insert(std::pair<unsigned int, SOCKET>(newSocket, newSocket));

		//send client playerlist

		if(TestCallback != nullptr)
			TestCallback(1);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// CLIENT //////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Network::CreateClient(const char* address, const int port)
	{
		client = new NetworkClient(); //NetworkServer(address);
		if (client->CreateClient(address, port))
		{
			m_Active = true;
			server = false;
			//connection_status = CONNECTION_CONNECTED;

			return true;
		}
		return false;
	}

	bool NetworkClient::CreateClient(const char* address, const int port)
	{
		m_TCPListenerThread = std::thread(&NetworkClient::InitializeTCP, this, IP, PORT);
		//m_UDPThread = std::thread(&NetworkClient::InitializeUDP, this, IP, PORT);
		return true;
	}

	void NetworkClient::InitializeTCP(const char* address, int port)
	{
		printf("NETWORK: Establishing TCP Connection\n");
		SOCKET connectSocket = CreateTCPSocket(address, port);
		if (connectSocket == INVALID_SOCKET)
		{
			printf("NETWORK: ConnectSocket - INVALID_SOCKET.\n");
			system("pause");
			//return false;
		}

		while (1) {
			printf("NETWORK: TCPReceive.\n");
			TCPConsumer(connectSocket);
		}
	}

	SOCKET NetworkClient::CreateTCPSocket(const char* address, int port)
	{
		u_long interfaceAddr = InetPton(AF_INET, address, &interfaceAddr);
		if (interfaceAddr != INADDR_NONE)
		{
			SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (clientSocket != INVALID_SOCKET)
			{
				sockaddr_in hint;
				hint.sin_family = AF_INET;
				//hint.sin_addr.s_addr = interfaceAddr;
				InetPton(AF_INET, "127.0.0.1", &hint.sin_addr.s_addr);
				hint.sin_port = htons(port);

				if (connect(clientSocket, (sockaddr*)&hint, sizeof(hint)) != SOCKET_ERROR)
				{
					printf("connect to the server: %d", WSAGetLastError());
				}
				else
				{
					// Failed to connect to server
					printf("Failed to connect to the server: %d", WSAGetLastError());
					closesocket(clientSocket);
					//Cleanup();
					return INVALID_SOCKET;
				}
			}
			return clientSocket;
		}
		return INVALID_SOCKET;
	}

	bool NetworkClient::TCPConsumer(SOCKET sd)
	{
		// Read data from client
		char acReadBuffer[BUFFER_SIZE];
		int readBytes;
		do {
			readBytes = recv(sd, acReadBuffer, BUFFER_SIZE, 0);
			if (readBytes > 0) {
				printf("NETWORK: TCP - Received {0} bytes from client.", readBytes);
			}
			else if (readBytes == SOCKET_ERROR) {
				//return false;
			}
		} while (readBytes != 0);

		printf("Connection closed by peer.");
		return true;
	}

}

