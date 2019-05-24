#pragma once

#include <functional>
#include <queue>
#include <vector>
#include "Client_MutexQueue.h"
#include "Client_SnapshotPacket.h"

#include <iostream>

struct Message
{
public:
	Message(const std::string message, SnapshotPacket* packet = nullptr)
	{
		message_ = message;

		if (packet != nullptr)
			snapshotPacket_ = packet;
	}

	std::string getMessage()
	{
		return message_;
	}

	SnapshotPacket* GetSnapshotPacket()
	{
		return snapshotPacket_;
	}

	void assignStr(std::string str, size_t start, size_t end)
	{
		message_.assign(str, start, end);
	}

	void memcpyStr(void* src , size_t size)
	{
		memcpy(&message_[0], src, size);
	}

private:
	std::string message_;
	SnapshotPacket* snapshotPacket_;
};

class Client_MessagingSystem
{
public:
	Client_MessagingSystem();
	~Client_MessagingSystem();

	void addReceiver(std::function<void(Message)> messageReceiver)
	{
		receivers_.push_back(messageReceiver);
	}

	void sendMessage(Message message)
	{
		messages_.push(message);
	}

	void notify()
	{
		while (!messages_.isEmpty()) {
			for (auto iter = receivers_.begin(); iter != receivers_.end(); iter++) {
				(*iter)(messages_.front());
			}
			messages_.pop();
		}
	}

private:
	std::vector<std::function<void(Message)>> receivers_;
	Client_MutexQueue<Message> messages_;
};

