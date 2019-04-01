#pragma once

#include "MessagingSystem.h"
#include <iostream>
#include <functional>

class BusNode
{
public:
	BusNode(MessagingSystem* messageBus)
	{
		this->messageBus_ = messageBus;
		this->messageBus_->addReceiver(this->getNotifyFunc());
	}
	~BusNode();

protected:
	MessagingSystem* messageBus_;

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

