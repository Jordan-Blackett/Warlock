#pragma once

#include <cstdint>

struct InputPacket {
	uint8_t left = false;
	uint8_t right = false;
	uint8_t up = false;
	uint8_t down = false;
	uint8_t space = false;
	uint8_t leftclick = false;
	uint8_t rightclick = false;
	uint16_t frameID;
};