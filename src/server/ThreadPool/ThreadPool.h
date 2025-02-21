#pragma once

#include <thread>
#include <vector>

#include "Task.h"
#include "UnboundedBlockingMPMCQueue.h"

namespace tp
{

// Fixed-size pool of worker threads
class ThreadPool
{
public:
	explicit ThreadPool(size_t threads);
	~ThreadPool();

	// Non-copyable
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	// Non-movable
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	// Creates the workers for the thread pool
	void Start();

	// Schedules task for execution in one of the worker threads
	void Submit(Task);

	// Locates current thread pool from worker thread
	static ThreadPool* Current();

	// Stops the worker threads as soon as possible
	// Pending tasks will be discarded
	void Stop();

private:
	void StartWorkerThreads();
	void JoinWorkerThreads();
	void WorkerRoutine();

	static size_t GetOptimalNumberOfWorkers(size_t);

	void Invoke(Task& task);

private:
	std::vector<std::thread> m_workers;
	UnboundedBlockingMPMCQueue<Task> m_queue;
	static thread_local ThreadPool* s_current;
};

} // namespace tp
