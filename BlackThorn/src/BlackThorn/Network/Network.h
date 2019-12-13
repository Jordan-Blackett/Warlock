#pragma once

#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "Socket.h"

#include <thread>
#include <map>

#define IP "127.0.0.1"
#define PORT 5555
#define BUFFER_SIZE 1024

namespace BlackThorn {

	enum class NetworkCallBack
	{
		ConnectionReceived = 0
	};

	class BLACKTHORN_API NetworkServer
	{
	public:
		bool CreateServer(const int port, const int maxplayers);
		void InitializeTCP(const char* address, int port);
		void InitializeUDP(const char* address, int port);

		void CleanUp();

		void Close();

		// TCP

		SOCKET CreateListenSocket(const char* address, int port);
		void AcceptConnections(SOCKET ListeningSocket);
		static DWORD TCPConsumer(void* sd_);

		static bool TCPReceive(SOCKET sd);
		static bool EchoIncomingPackets(SOCKET sd);
		static bool ShutdownConnection(SOCKET sd);

		// UDP

		SOCKET CreateUDPSocket(const char* address, int port, sockaddr_in outhint);
		DWORD UDPConsumer(SOCKET udpsocket, sockaddr_in hint);
		bool EchoIncomingUDPPackets(SOCKET udpsocket, sockaddr_in hint);

		// Callbacks
		using NetCallbackFn = std::function<void(int)>;
		void Bind(const NetworkCallBack callbackName, const NetCallbackFn& callback);
		void ConnectionReceived(SOCKET newSocket);

		// Debug
		std::map<u_int64, SOCKET> GetClientConnects() { return m_ClientConnections; }

	private:
		std::thread m_TCPListenerThread;
		std::thread m_UDPThread;

		bool m_TCPClose;
		bool m_UDPClose;

		NetCallbackFn TestCallback;

		std::map<u_int64, SOCKET> m_ClientConnections;

	};

	class BLACKTHORN_API NetworkClient
	{
	public:
		bool CreateClient(const char* address, const int port);
		void InitializeTCP(const char* address, int port);
		void InitializeUDP(const char* address, int port);

		void CleanUp();

		void Close();

		// TCP

		SOCKET CreateTCPSocket(const char* address, int port);
		bool TCPConsumer(SOCKET sd);

		// UDP

		// Callbacks
		using NetCallbackFn = std::function<void(int)>;
		void Connect(const NetworkCallBack callbackName, const NetCallbackFn& callback);
		void ConnectionReceived(SOCKET newSocket);

	private:
		std::thread m_TCPListenerThread;
		std::thread m_UDPThread;

		bool m_TCPClose;
		bool m_UDPClose;
	};

	class BLACKTHORN_API Network
	{
	public:
		bool Initialize();
		void InitializeTCP(const char* address, int port);
		void InitializeUDP(const char* address, int port);

		void CleanUp();

		void Close();

		// TCP

		SOCKET CreateListenSocket(const char* address, int port);
		void AcceptConnections(SOCKET ListeningSocket);
		DWORD TCPConsumer(void* sd_);

		bool EchoIncomingPackets(SOCKET sd);
		bool ShutdownConnection(SOCKET sd);

		// UDP

		SOCKET CreateUDPSocket(const char* address, int port, sockaddr_in outhint);
		DWORD UDPConsumer(SOCKET udpsocket, sockaddr_in hint);
		bool EchoIncomingUDPPackets(SOCKET udpsocket, sockaddr_in hint);

		// Server

		bool CreateServer(const int port, const int maxplayers);

		// Client
		bool CreateClient(const char* address, const int port);

		// Callbacks
		using NetCallbackFn = std::function<void(int)>;
		void Bind(const NetworkCallBack callbackName, const NetCallbackFn& callback);

		//
		bool IsActive() { return m_Active; }
		bool IsServer() { return server; }

		// Debug
		std::map<u_int64, SOCKET>& GetClientConnects() { return host->GetClientConnects(); }
		//GetIPAddress();

	private:
		//static std::thread m_ListenerThread;
		bool m_TCPClose;
		bool m_UDPClose;

		NetworkServer* host;
		NetworkClient* client;

		bool m_Active;
		bool server;
		//connection_status = CONNECTION_CONNECTED;
		std::map<u_int64, SOCKET> m_ClientConnections;
		

		// Debug
	};

}

