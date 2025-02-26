#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>

namespace tp
{

/**
 * @brief A synchronization primitive that waits for a group of tasks to complete.
 *
 * The `WaitGroup` allows multiple tasks to be tracked, ensuring that `Wait()` blocks
 * until all tasks have finished. Tasks are added using `Add()`, and marked as done
 * using `Done()`. Once all tasks are marked as done, waiting threads are unblocked.
 */
class WaitGroup
{
public:
	/// @brief Deleted copy constructor to prevent copying.
	WaitGroup(const WaitGroup&) = delete;

	/// @brief Deleted copy assignment operator to prevent copying.
	WaitGroup& operator=(const WaitGroup&) = delete;

	/// @brief Deleted move constructor to prevent moving.
	WaitGroup(WaitGroup&&) = delete;

	/// @brief Deleted move assignment operator to prevent moving.
	WaitGroup& operator=(WaitGroup&&) = delete;

	/**
	 * @brief Adds a specified number of tasks to the wait group.
	 *
	 * This increases the count of unfinished jobs. The `Wait()` function
	 * will block until all jobs are marked as done.
	 *
	 * @param count The number of tasks to add.
	 */
	void Add(size_t count);

	/**
	 * @brief Marks a single task as done.
	 *
	 * This decreases the count of unfinished jobs. If all jobs are done,
	 * waiting threads are unblocked.
	 */
	void Done();

	/**
	 * @brief Blocks until all tasks are completed.
	 *
	 * This function waits until the count of unfinished jobs reaches zero.
	 */
	void Wait();

private:
	std::atomic<uint32_t> m_jobs{0};
	std::mutex m_mutex;
	std::condition_variable m_is_finished;
	uint32_t m_is_waiting{0};
};

} // namespace tp