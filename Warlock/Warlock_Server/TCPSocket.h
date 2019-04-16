#pragma once

#include <Ws2tcpip.h>

#define MAX_BUFFER_SIZE (49152)

class TCPSocket
{
public:
	TCPSocket() {};
	TCPSocket(SOCKET socket);
	~TCPSocket();

	SOCKET socket_;

	// Partial Send/Recv Variables
	// Sending
	int bytesSent = 0;
	int outgoingPacketLength = 0; // Length (bytes) of outgoing packet

	// Recv'ing
	int bytesReceived = 0;
	int incomingPacketLength = 2; // Length (bytes) of incoming packet
	char buffer[MAX_BUFFER_SIZE];
};

