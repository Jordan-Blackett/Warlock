#include "Server_InputBuffer.h"



Server_InputBuffer::Server_InputBuffer()
{
}


Server_InputBuffer::~Server_InputBuffer()
{
}

void Server_InputBuffer::Push(int ClientID, InputPacket* packet)
{
	inputBuffer[ClientID].push(packet);
}

InputPacket* Server_InputBuffer::GetInputPacket(int ClientID)
{
	//if (!inputBuffer.empty())
	//{
		InputPacket* input = inputBuffer[ClientID].front();
		inputBuffer[ClientID].pop();
		return input;
	//}
}

bool Server_InputBuffer::Empty(int ClientID)
{
	return inputBuffer[ClientID].empty();
}
