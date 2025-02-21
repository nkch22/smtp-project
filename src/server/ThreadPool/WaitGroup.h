#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>

namespace tp
{
class WaitGroup
{
public:
	// Non-copyable
	WaitGroup(const WaitGroup&) = delete;

	WaitGroup& operator=(const WaitGroup&) = delete;

	// Non-movable
	WaitGroup(WaitGroup&&) = delete;

	WaitGroup& operator=(WaitGroup&&) = delete;

	void Add(size_t count);

	void Done();

	void Wait();

private:
	std::atomic<uint32_t> m_jobs{0};
	std::mutex m_mutex;
	std::condition_variable m_is_finished;
	uint32_t m_is_waiting{0};
};
} // namespace tp
