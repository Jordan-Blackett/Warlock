#pragma once

#include <stdint.h>
#include <vector>

struct ObjectState
{
public:
	//bool interacting;
	uint16_t positionX;
	uint16_t positionY;
	float angle;
};

struct SnapshotPacket
{
public:
	uint16_t sequence;
	//int deltapacketindex;
	bool initial = false;
	std::vector<ObjectState*> objectStates;
	//float time;
};

class Client_SnapshotPacket
{
public:
	Client_SnapshotPacket();
	~Client_SnapshotPacket();
};

