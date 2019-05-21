#pragma once

#include <WinSock2.h>
#include <stdint.h>
#include <string>
#include <iostream>

// MessageLength (16-bits), MessageType (8-bits) and MessageSubType (8-bits)
struct PacketHeader {
	//uint16_t msgClientID;
	uint16_t msgType;
	uint16_t msgSubType;
};

struct InputPacket2 {
	uint8_t left = false;
	uint8_t right = false;
	uint8_t up = false;
	uint8_t down = false;
	uint8_t space = false;
	uint8_t leftclick = false;
	uint8_t rightclick = false;
	uint16_t frameID;
};

struct PacketData {
	InputPacket2 input;
	uint64_t pf2;
};

class TestPacket
{
public:
	TestPacket(const PacketData& data, const uint16_t Type, const uint16_t SubType);
	~TestPacket();

	void htonHeaderData(const struct PacketHeader& header, const struct PacketData& data);
	void htonHeader(const struct PacketHeader& header);
	void htonData(const struct PacketData& data);

	//char[] GetBuffer() { return buffer_; }
	//char* buffer_;
	char buffer_[128]; //128
	int bufferSize_ = 0;

private:
	int bufferOffset_ = 2; // 0-1 is packet size
};

