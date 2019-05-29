#pragma once

#include <map>
#include <queue>
#include "InputPacket.h"

class Server_InputBuffer
{
public:
	Server_InputBuffer();
	~Server_InputBuffer();

	void Push(int ClientID, InputPacket* packet);
	InputPacket* GetInputPacket(int ClientID);

	bool Empty(int ClientID);

private:
	std::map<int, std::queue<InputPacket*>> inputBuffer;

};

