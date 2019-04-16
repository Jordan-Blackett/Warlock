#pragma once

#include <functional>
#include <queue>
#include <vector>
#include "Client_MutexQueue.h"

#include <iostream>

struct Message
{
public:
	Message(const std::string message)
	{
		message_ = message;
	}

	std::string getMessage()
	{
		return message_;
	}

private:
	std::string message_;
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

