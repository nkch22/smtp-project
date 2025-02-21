#pragma once

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

namespace tp
{

// Unbounded blocking multi-producers/multi-consumers (MPMC) queue

template<typename T>
class UnboundedBlockingMPMCQueue
{
public:
	void Push(T value)
	{
		assert(!m_is_closed);
		std::lock_guard guard(m_mutex);
		m_buffer.push_back(std::move(value));
		m_not_empty.notify_one();
	}

	std::optional<T> Pop()
	{
		std::unique_lock lock(m_mutex);
		while (m_buffer.empty())
		{
			if (m_is_closed) return std::nullopt;
			m_not_empty.wait(lock);
		}
		return PopLocked();
	}

	void Close()
	{
		m_is_closed.store(true);
		m_not_empty.notify_all();
	}

private:
	T PopLocked()
	{
		assert(!m_buffer.empty());
		T value{std::move(m_buffer.front)};
		m_buffer.pop_front();
		return value;
	}

private:
	std::deque<T> m_buffer;
	std::mutex m_mutex;
	std::condition_variable m_not_empty;
	std::atomic<bool> m_is_closed{false};
};

} // namespace tp