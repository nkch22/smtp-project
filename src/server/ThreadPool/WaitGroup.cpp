#include "WaitGroup.h"

void WaitGroup::Add(size_t count)
{
	m_jobs.fetch_add(count);
}

void WaitGroup::Done()
{
	std::lock_guard guard(m_mutex);
	m_jobs.fetch_sub(1);
	if (m_jobs.load() == 0 && m_is_waiting > 0)
	{
		m_is_finished.notify_all();
	}
}

void WaitGroup::Wait()
{
	std::unique_lock lock(m_mutex);
	m_is_waiting += 1;
	m_is_finished.wait(lock, [this] { return m_jobs == 0; });
	m_is_waiting -= 1;
}
