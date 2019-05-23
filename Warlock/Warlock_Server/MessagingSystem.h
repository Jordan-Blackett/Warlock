#pragma once

#include <functional>
#include <queue>
#include <vector>
#include "MutexQueue.h"
#include "Server_SnapshotPacket.h"

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

private:
	std::string message_;
	SnapshotPacket* snapshotPacket_;
};

class MessagingSystem
{
public:
	MessagingSystem();
	~MessagingSystem();

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
		std::unique_lock<std::mutex> l(lock_);
		while (!messages_.isEmpty()) {
			for (auto iter = receivers_.begin(); iter != receivers_.end(); iter++) {
				(*iter)(messages_.front());
			}
			messages_.pop();
		}
	}

private:
	std::vector<std::function<void(Message)>> receivers_;
	MutexQueue<Message> messages_;
	std::mutex lock_;
};

