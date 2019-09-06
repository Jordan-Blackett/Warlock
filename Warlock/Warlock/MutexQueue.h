#pragma once

#include<queue>
#include<mutex>

template <typename T>
class MutexQueue
{
public:
	void pop() {
		std::unique_lock<std::mutex> l(m_Lock);
		auto val = m_Queue.front();
		m_Queue.pop();
		//return val;
	}

	void push(T item) {
		std::unique_lock<std::mutex> l(m_Lock);
		m_Queue.push(item);
	}

	T front() {
		std::unique_lock<std::mutex> l(m_Lock);
		return m_Queue.front();
	}

	bool isEmpty() { return m_Queue.empty(); }

	void clear() { m_Queue.empty(); }

	//queue_ = default;
	//queue_ (const queue&);
private:
	std::queue<T> m_Queue;
	std::mutex m_Lock;
	//std::condition_variable cond_;
};

