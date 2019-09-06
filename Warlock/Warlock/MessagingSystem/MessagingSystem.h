#pragma once

#include <string>
#include <sstream>
#include <functional>
#include <vector>

#include "../MutexQueue.h"

#include "../Network/StatePacket.h"
#include "../Network/InputPacket.h"

#define BIT(x) (1 << x)

class NewConnectionMessage;

enum class MessageType
{
	None = 0,
	NewConnect, State, Input
};

enum MessageCategory
{
	None = 0,
	EventCategoryNetwork = BIT(0),
	EventStatePacket = BIT(1),
	EventInputPacket = BIT(2),
	EventActionInput = BIT(3)
};

#define EVENT_CLASS_TYPE(type) static MessageType GetStaticType() { return MessageType::##type; }\
								virtual MessageType GetMessageType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Message
{
public:
	bool Handled = false;

	virtual MessageType GetMessageType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	inline bool IsInCategory(MessageCategory category)
	{
		return GetCategoryFlags() & category;
	}
};

class MessagingSystem
{
public:
	void AddReceiver(std::function<void(Message&)> messageReceiver)
	{
		m_Receivers.push_back(messageReceiver);
	}

	void sendMessage(std::shared_ptr<Message> message)
	{
		m_Messages.push(std::move(message));
	}

	void Notify()
	{
		while (!m_Messages.isEmpty()) {
			for (auto iter = m_Receivers.begin(); iter != m_Receivers.end(); iter++)
			{
				(*iter)(*m_Messages.front());
			}
			m_Messages.pop();
		}
	}

private:
	std::vector<std::function<void(Message&)>> m_Receivers;
	MutexQueue<std::shared_ptr<Message>> m_Messages;
};

class NewConnectionMessage : public Message
{
public:
	NewConnectionMessage(int socketID)
		: m_ConnectionID(socketID) {}

	inline int GetSocketIDPacket() { return m_ConnectionID; }

	EVENT_CLASS_CATEGORY(EventCategoryNetwork | EventStatePacket)
	EVENT_CLASS_TYPE(State)

protected:
	int m_ConnectionID;
};

class StateMessage : public Message
{
public:
	inline StatePacket* GetStatePacket() { return &m_StatePacket; }

	EVENT_CLASS_CATEGORY(EventCategoryNetwork | EventStatePacket)
	EVENT_CLASS_TYPE(State)

protected:
	StateMessage(StatePacket packet)
		: m_StatePacket(packet) {}

	StatePacket m_StatePacket;
};

class InputMessage : public Message
{
public:
	EVENT_CLASS_CATEGORY(EventCategoryNetwork | EventInputPacket)
	EVENT_CLASS_TYPE(Input)

protected:
	InputMessage(InputPacket& packet)
		: m_InputPacket(packet) {}

	InputPacket m_InputPacket;
};


class ActionMessage : public Message
{
public:
	inline int GetInputCode() const { return m_Input; }

	EVENT_CLASS_CATEGORY(EventCategoryNetwork | EventActionInput)
protected:
	ActionMessage(int input)
		: m_Input(input) {}

	int m_Input;
};

class ActionMessageAttack: public ActionMessage
{
public:
	ActionMessageAttack(int keycode, int repeatCount)
		: ActionMessage(keycode), m_RepeatCount(repeatCount) {}

	inline int GetRepeatCount() const { return m_RepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_Input << " (" << m_RepeatCount << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(Input)
private:
	int m_RepeatCount;
};