#include "TestPacket.h"



TestPacket::TestPacket(std::string message)
{
	PacketHeader* packetHeader = new PacketHeader();
	packetHeader->msgLength = 6 + message.size();
	packetHeader->msgType = 40;
	packetHeader->msgSubType = 75;

	std::cout << "Debug Length: " << packetHeader->msgLength << std::endl;
	std::cout << "Debug Header Length: " << 6 << std::endl;
	std::cout << "Debug Message Length: " << message.size() << std::endl;
	std::cout << "Debug Message: " << message << std::endl;

	//PacketData* packetData = new PacketData();
	//htonHeaderData(*packetHeader, *packetData, buffer_);
	htonHeader(*packetHeader);
	strcpy_s(buffer_ + 6, packetHeader->msgLength, message.c_str());

	bufferSize_ = packetHeader->msgLength;
}


TestPacket::~TestPacket()
{
}

void TestPacket::htonHeaderData(const PacketHeader& header, const PacketData& data)
{
	htonHeader(header);
	//htonData(data);
}

void TestPacket::htonHeader(const PacketHeader& header)
{
	uint16_t u16;
	uint16_t bufferOffset = 0;
	u16 = htons(header.msgLength);
	memcpy(buffer_ + bufferOffset, &u16, sizeof(uint16_t));
	bufferOffset += sizeof(uint16_t);
	u16 = htons(header.msgType);
	memcpy(buffer_ + bufferOffset, &u16, sizeof(uint16_t));
	bufferOffset += sizeof(uint16_t);
	u16 = htons(header.msgSubType);
	memcpy(buffer_ + bufferOffset, &u16, sizeof(uint16_t));
}

void TestPacket::htonData(const PacketData& data)
{
	//uint16_t u16;
	//uint32_t u32;
	//u16 = htons(data.pf1);
	//memcpy(buffer + 0, &u16, 2);
	//u32 = htonl(data.pf2 >> 32);
	//memcpy(buffer + 2, &u32, 4);
	//u32 = htonl(data.pf2);
	//memcpy(buffer + 6, &u32, 4);
}
