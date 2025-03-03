#pragma once

#include <thread>
#include <vector>

#include "asio.hpp"

/**
 * @class Service
 * @brief Provides an asynchronous execution environment using ASIO's io_context and worker threads.
 */
class Service
{
public:
	/**
	 * @brief Constructs a Service with a specified number of worker threads.
	 * @param threads_count Number of worker threads to spawn.
	 */
	explicit Service(size_t threads_count);

	// Non-copyable
	Service(const Service&) = delete;
	Service& operator=(const Service&) = delete;

	// Non-movable
	Service(Service&&) = delete;
	Service& operator=(Service&&) = delete;

	virtual ~Service() = default;

	/**
	 * @brief Starts the service and worker threads.
	 * @return True on success, false otherwise.
	 */
	virtual bool Start();

	/**
	 * @brief Stops the service, shutting down worker threads.
	 * @return True if successfully stopped.
	 */
	virtual bool Stop();

	/**
	 * @brief Provides access to the underlying ASIO io_service.
	 * @return Reference to the ASIO io_service instance.
	 */
	virtual asio::io_service& GetAsioService() noexcept;

	/**
	 * @brief Dispatches the given handler.
	 *
	 * The given handler may be executed immediately if this function is called from an IO service thread.
	 * Otherwise, it will be enqueued in the IO service pending operations queue.
	 *
	 * @tparam CompletionHandler A callable type representing the handler function.
	 * @param handler The handler function to dispatch.
	 * @return The asynchronous result of the handler.
	 */
	template<typename CompletionHandler>
	typename asio::async_result<asio::decay_t<CompletionHandler>, void()>::return_type Dispatch(
		CompletionHandler&& handler)
	{
		return m_context.dispatch(handler);
	}

	/**
	 * @brief Posts the given handler to be executed asynchronously.
	 *
	 * The given handler will always be enqueued in the IO service pending operations queue.
	 * Unlike Dispatch(), it will never execute the handler immediately.
	 *
	 * @tparam CompletionHandler A callable type representing the handler function.
	 * @param handler The handler function to post.
	 * @return The asynchronous result of the handler.
	 */
	template<typename CompletionHandler>
	typename asio::async_result<asio::decay_t<CompletionHandler>, void()>::return_type Post(CompletionHandler&& handler)
	{
		return m_context.post(handler);
	}

	/**
	 * @brief Called when the service starts.
	 *
	 * This method is meant to be overridden by derived classes to implement custom behavior
	 * when the service starts.
	 */
	virtual void OnStarted();

	/**
	 * @brief Called when the service stops.
	 *
	 * This method is meant to be overridden by derived classes to implement custom behavior
	 * when the service stops.
	 */
	virtual void OnStopped();

	/**
	 * @brief Handles error notifications.
	 *
	 * Can be overridden to handle errors reported by the service.
	 *
	 * @param error The error code.
	 * @param category A string representing the error category.
	 * @param message A human-readable error message.
	 */
	virtual void OnError(int error, const std::string& category, const std::string& message);

private:
	/**
	 * @brief Worker thread routine.
	 *
	 * Each worker thread runs this function to process queued tasks in the io_context.
	 * The function loops until the service is stopped.
	 */
	void WorkerRoutine();

	/**
	 * @brief Sends an error notification.
	 *
	 * Converts an std::error_code into an error report and invokes OnError().
	 *
	 * @param ec The error code to report.
	 */
	void SendError(std::error_code ec);

	/**
	 * @brief Spawns worker threads.
	 *
	 * Creates and starts worker threads to process tasks in the io_context.
	 */
	void SpawnWorkers();
	void SpawnWorker();

	/**
	 * @brief Joins worker threads.
	 *
	 * Waits for all worker threads to finish execution.
	 */
	void JoinWorkerThreads();

private:
	asio::io_context m_context;
	/** Keeps the io_context active to prevent premature exit. */
	asio::io_context::work m_work;
	std::vector<std::thread> m_workers;
	std::atomic<bool> m_is_started{false};
	constexpr size_t m_threads_count;
};