#pragma once

#include <WinSock2.h>
#include <stdint.h>
#include <string>
#include <iostream>

// MessageLength (16-bits), MessageType (8-bits) and MessageSubType (8-bits)
struct PacketHeader {
	uint16_t msgLength;
	uint16_t msgType;
	uint16_t msgSubType;
};

struct PacketData {
	uint16_t pf1;
	uint64_t pf2;
};

class TestPacket
{
public:
	TestPacket(std::string message);
	~TestPacket();

	void htonHeaderData(const struct PacketHeader& header, const struct PacketData& data);
	void htonHeader(const struct PacketHeader& header);
	void htonData(const struct PacketData& data);

	//char[] GetBuffer() { return buffer_; }
	//char* buffer_;
	char buffer_[128];
	int bufferSize_ = 0;
private:
};

