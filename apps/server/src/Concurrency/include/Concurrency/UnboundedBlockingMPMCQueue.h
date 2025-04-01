#pragma once

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <optional>

namespace concurrency
{

/**
 * @brief Unbounded blocking multi-producer/multi-consumer (MPMC) queue.
 *
 * This queue supports multiple producers and multiple consumers, ensuring thread-safe
 * operations for pushing and popping elements. It blocks consumers when empty and wakes them
 * when new elements are available.
 *
 * @tparam T Type of elements stored in the queue.
 */
template<typename T>
class UnboundedBlockingMPMCQueue
{
public:
	/**
	 * @brief Pushes an element into the queue.
	 *
	 * This function moves the provided value into the queue and notifies one waiting consumer.
	 * It is not allowed to push elements after the queue is closed.
	 *
	 * @param value The value to be pushed into the queue.
	 */
	void Push(T value)
	{
		assert(!m_is_closed);
		std::lock_guard guard(m_mutex);
		m_buffer.push(std::move(value));
		m_not_empty.notify_one();
	}

	/**
	 * @brief Pops an element from the queue.
	 *
	 * If the queue is empty, this function blocks until an element is available.
	 * If the queue is closed and empty, it returns `std::nullopt`.
	 *
	 * @return std::optional<T> The popped element, or `std::nullopt` if the queue is closed.
	 */
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

	/**
	 * @brief Closes the queue.
	 *
	 * Once closed, no new elements can be pushed, and waiting consumers will stop blocking.
	 */
	void Close()
	{
		m_is_closed.store(true);
		m_not_empty.notify_all();
	}

	std::queue<T> Extract()
	{
		std::lock_guard guard(m_mutex);
		std::queue<T> extracted = std::move(m_buffer);
		m_buffer = {};
		return std::move(extracted);
	}

private:
	/**
	 * @brief Pops an element when the mutex is already locked.
	 *
	 * @return T The popped element.
	 */
	T PopLocked()
	{
		assert(!m_buffer.empty());
		T value{std::move(m_buffer.front())};
		m_buffer.pop();
		return value;
	}

private:
	std::queue<T> m_buffer;
	std::mutex m_mutex;
	std::condition_variable m_not_empty;
	std::atomic<bool> m_is_closed{false};
};

} // namespace concurrency