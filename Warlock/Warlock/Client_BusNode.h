#pragma once

#include "Client_MessagingSystem.h"
#include <iostream>
#include <functional>

class Client_BusNode
{
public:
	Client_BusNode() {};
	Client_BusNode(Client_MessagingSystem* messageBus)
	{
		this->messageBus_ = messageBus;
		this->messageBus_->addReceiver(this->getNotifyFunc());
	}
	void Init(Client_MessagingSystem* messageBus)
	{
		this->messageBus_ = messageBus;
		this->messageBus_->addReceiver(this->getNotifyFunc());
	}
	~Client_BusNode();

protected:
	Client_MessagingSystem * messageBus_;

	std::function<void(Message)> getNotifyFunc()
	{
		auto messageListener = [=](Message message) -> void {
			this->onNotify(message);
		};
		return messageListener;
	}

	void SendMessageSystem(Message message)
	{
		messageBus_->sendMessage(message);
	}

	virtual void onNotify(Message message)
	{
		std::cout << "Forgot to implement onNotify()" << std::endl;
	}

	void Notify()
	{
		messageBus_->notify();
	}
};

