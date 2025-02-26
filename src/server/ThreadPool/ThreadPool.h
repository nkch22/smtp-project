#pragma once

#include <thread>
#include <vector>

#include "Task.h"
#include "UnboundedBlockingMPMCQueue.h"

namespace tp
{

/**
 * @class ThreadPool
 * @brief Fixed-size pool of worker threads.
 */
class ThreadPool
{
public:
	/**
	 * @brief Constructs a thread pool with a given number of threads.
	 * @param threads Number of worker threads.
	 */
	explicit ThreadPool(size_t threads);

	/**
	 * @brief Destroys the thread pool, ensuring proper cleanup.
	 */
	~ThreadPool();

	// Non-copyable
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	// Non-movable
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	/**
	 * @brief Creates the worker threads for the thread pool.
	 */
	void Start();

	/**
	 * @brief Schedules a task for execution in one of the worker threads.
	 * @param task The task to execute.
	 */
	void Submit(Task task);

	/**
	 * @brief Retrieves the thread pool associated with the current worker thread.
	 * @return Pointer to the current thread pool, or nullptr if not in a worker thread.
	 */
	static ThreadPool* Current();

	/**
	 * @brief Stops the worker threads as soon as possible.
	 * @note Pending tasks will be finished properly.
	 */
	void Stop();

private:
	/**
	 * @brief Starts the worker threads.
	 */
	void StartWorkerThreads();

	/**
	 * @brief Joins the worker threads, ensuring they have completed execution.
	 */
	void JoinWorkerThreads();

	/**
	 * @brief Worker thread execution routine.
	 */
	void WorkerRoutine();

	/**
	 * @brief Determines the optimal number of worker threads.
	 * @param requested Number of requested worker threads.
	 * @return Optimal number of worker threads.
	 */
	static size_t GetOptimalNumberOfWorkers(size_t requested);

	/**
	 * @brief Executes the given task.
	 * @param task The task to invoke.
	 */
	void Invoke(Task& task);

private:
	std::vector<std::thread> m_workers;
	UnboundedBlockingMPMCQueue<Task> m_queue;
	static thread_local ThreadPool* s_current;
};

} // namespace tp
