#include "ThreadPool.h"

namespace concurrency
{
ThreadPool::ThreadPool(size_t threads) : m_workers(GetOptimalNumberOfWorkers(threads)) {}

ThreadPool::~ThreadPool() = default;

void ThreadPool::Start()
{
	StartWorkerThreads();
}

void ThreadPool::Submit(Task task)
{
	m_queue.Push(std::move(task));
}

ThreadPool* ThreadPool::Current()
{
	return s_current;
}

void ThreadPool::Stop()
{
	m_queue.Close();
	JoinWorkerThreads();
}

void ThreadPool::StartWorkerThreads()
{
	for (auto& worker : m_workers)
	{
		worker = std::thread(
			[this]()
			{
				s_current = this;
				WorkerRoutine();
			});
	}
}

void ThreadPool::WorkerRoutine()
{
	while (true)
	{
		std::optional<Task> task = m_queue.Pop();
		if (!task) break;
		Invoke(*task);
	}
}

size_t ThreadPool::GetOptimalNumberOfWorkers(size_t count)
{
	if (count > 0) return count;
	unsigned hardware_threads = std::thread::hardware_concurrency();
	if (hardware_threads == 0) hardware_threads = 2;
	return hardware_threads;
}

void ThreadPool::JoinWorkerThreads()
{
	for (auto& worker : m_workers)
	{
		worker.join();
	}
}

void ThreadPool::Invoke(Task& task)
{
	task();
}

thread_local ThreadPool* ThreadPool::s_current{nullptr};
} // namespace concurrency
