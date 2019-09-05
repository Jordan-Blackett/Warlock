#include "TestPacket.h"



TestPacket::TestPacket(const PacketData& data, const uint16_t Type, const uint16_t SubType)
{
	PacketHeader* packetHeader = new PacketHeader();
	packetHeader->msgType = Type;
	packetHeader->msgSubType = SubType;

	htonHeaderData(*packetHeader, data);

	//packetHeader->msgLength = bufferOffset_;

	//std::cout << "Debug Length: " << packetHeader->msgLength << std::endl;
	//std::cout << "Debug Header Length: " << 6 << std::endl;
	//std::cout << "Debug Message Length: " << message.size() << std::endl;
	//std::cout << "Debug Message: " << message << std::endl;
	//strcpy_s(buffer_ + 6, packetHeader->msgLength, message.c_str());

	bufferSize_ = bufferOffset_;
}


TestPacket::~TestPacket()
{
}

void TestPacket::htonHeaderData(const PacketHeader& header, const PacketData& data)
{
	htonHeader(header);
	htonData(data);

	// Packet size
	uint16_t u16;
	u16 = htons(bufferOffset_);
	memcpy(buffer_, &u16, sizeof(uint16_t));
}

void TestPacket::htonHeader(const PacketHeader& header)
{
	uint16_t u16;
	u16 = htons(header.msgType);
	memcpy(buffer_ + bufferOffset_, &u16, sizeof(uint16_t));
	bufferOffset_ += sizeof(uint16_t);
	u16 = htons(header.msgSubType);
	memcpy(buffer_ + bufferOffset_, &u16, sizeof(uint16_t));
	bufferOffset_ += sizeof(uint16_t);
}

void TestPacket::htonData(const PacketData& data)
{
	//switch
	//{}
	memcpy(buffer_ + bufferOffset_, &data.input.left, sizeof(uint8_t));
	bufferOffset_ += sizeof(uint8_t);
	memcpy(buffer_ + bufferOffset_, &data.input.right, sizeof(uint8_t));
	bufferOffset_ += sizeof(uint8_t);
	memcpy(buffer_ + bufferOffset_, &data.input.up, sizeof(uint8_t));
	bufferOffset_ += sizeof(uint8_t);
	memcpy(buffer_ + bufferOffset_, &data.input.down, sizeof(uint8_t));
	bufferOffset_ += sizeof(uint8_t);
	memcpy(buffer_ + bufferOffset_, &data.input.space, sizeof(uint8_t));
	bufferOffset_ += sizeof(uint8_t);
	memcpy(buffer_ + bufferOffset_, &data.input.leftclick, sizeof(uint8_t));
	bufferOffset_ += sizeof(uint8_t);
	memcpy(buffer_ + bufferOffset_, &data.input.rightclick, sizeof(uint8_t));
	bufferOffset_ += sizeof(uint8_t);
}

//{
//	std::string devider = "::";
//	strcpy_s(buffer_ + bufferOffset_, devider.size(), devider.c_str());
//	bufferOffset_ += devider.size();
//}