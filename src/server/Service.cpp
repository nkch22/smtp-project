#include "Service.h"

#include <iostream>

Service::Service(size_t threads_count) :
	m_work(m_context), m_workers(threads_count), m_is_started(false), m_threads_count(threads_count)
{
}

bool Service::Start()
{
	assert(!m_is_started.load() && "Asio context is already started");

	auto start_handler = [this]
	{
		if (m_is_started) return;

		m_is_started = true;

		OnStarted();
	};
	m_context.post(start_handler);

	SpawnWorkers();
	return true;
}

bool Service::Stop()
{
	assert(!m_is_started && "Asio service is not started!");

	auto stop_handler = [this]
	{
		if (!m_is_started) return;

		m_context.stop();

		m_is_started = false;

		OnStopped();
	};
	m_context.post(stop_handler);

	JoinWorkerThreads();
	return true;
}

asio::io_context& Service::GetAsioService() noexcept
{
	return m_context;
}

void Service::OnStarted() {}

void Service::OnStopped() {}

void Service::OnError(int error, const std::string& category, const std::string& message) {}

void Service::WorkerRoutine()
{
	while (!m_context.stopped())
	{
		try
		{
			m_context.run();
		}
		catch (const asio::system_error& e)
		{
			std::error_code error_code = e.code();
			// Skip Asio disconnect errors
			if (error_code == asio::error::not_connected) continue;

			SendError(error_code);
		}
		catch (const std::exception& e)
		{
			// TODO: change me to use logger
			std::cerr << "Exception in thread: " << e.what() << std::endl;
		}
		catch (...)
		{
			// TODO: change me to use logger
			std::cerr << "Unknown exception in thread" << std::endl;
		}
	}
}

void Service::SendError(std::error_code ec)
{
	OnError(ec.value(), ec.category().name(), ec.message());
}

void Service::SpawnWorkers()
{
	for (auto& worker : m_workers)
	{
		worker = std::thread(&Service::WorkerRoutine, this);
	}
}

void Service::JoinWorkerThreads()
{
	for (auto& thread : m_workers) thread.join();
}