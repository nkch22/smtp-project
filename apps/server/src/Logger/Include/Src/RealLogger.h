#pragma once

#include "Concurrency/UnboundedBlockingMPMCQueue.h"
#include "MessageFormatter.h"
#include "MessageType.h"
#include "SharedInclude.h"
#include "ThreadMap.h"
#include <atomic>

/*! @file RealLogger.h
*	@brief Implements all logger logic
*/

namespace logger_inner
{
using logger::Format;

/*! @class RealLogger
*	@brief Singleton logger implementation
* 
*	Class Logger uses this class to perform every logging operation
*	@warning There is no need to use this class directly, it is for inner use only
* 
*	It uses inner queue to store messages and worker thread to flush messages from queue
*/

class RealLogger
{
private:
	using Queue = concurrency::UnboundedBlockingMPMCQueue<Message>;

	static RealLogger* m_instance;

	logger::LogLevel m_level;
	std::string m_output_path;
	std::ofstream m_file;

	std::mutex m_mutex;

	std::atomic<bool> m_end;
	std::atomic<bool> m_do_flush;
	std::atomic<bool> m_is_config;

	std::atomic<unsigned int> m_amount;

	Queue m_queue;
	std::thread m_thr;

	ThreadMap m_thr_map;
	GlobalLogLevel m_level_map;

	RealLogger(const logger::LogLevel&, const std::string&, const unsigned int, const bool, const bool);

	~RealLogger() = default;

	void file_init(const unsigned int);

public:
	void operator=(const RealLogger&) = delete;
	RealLogger(const RealLogger&) = delete;
	RealLogger(RealLogger&&) = delete;

	/*! @brief Usable constructor
	*	
	*	It is implementation for init Logger method
	*/
	static RealLogger* get_instance(const logger::LogLevel& = DEFAULT_LOG_LEVEL, const std::string& = DEFAULT_PATH,
									const unsigned int amount = DEFAULT_AMOUNT, const bool is_config = DEFAULT_CONFIG,
									const bool do_flush = DEFAULT_FLUSH);

	/*! @brief Destructor
	*	It joins worker thread and deletes allocated memory
	*/
	static void destroy();

	/*! @brief Main method to log something
	*	Saves given message to inner queue for further flushing
	*/
	void save_to_queue(const std::string&, const logger_inner::MessageType&, const std::string&, const logger::LogLevel&,
					   const Format&, std::thread::id id = std::this_thread::get_id());

	/*! @brief Global log level setter*/
	void real_set_level(const logger::LogLevel&);
	/*! @brief Global log level getter*/
	const logger::LogLevel& real_get_level();

	/*! @brief Flushing method
	*	Performs actual flushing logic
	*	@warning it must be called only inside of inner worker thread
	*/
	void flush_message(const Message&);

	/*!@brief Fatal error handler
	*	It will log unhandled exception
	*	@warning it will work only if logger is successfully initialized
	*/
	static void handle_fatal_error();

	/*! @brief Method to stop config mode*/
	void real_stop_config();

	/*! @brief Output directory setter
	*	@warning Can be used only in config mode
	*/
	void set_output(const std::string&);

	/*! @brief Output directory getter*/
	const std::string& get_path() const;

	/*! @brief Flush setter*/
	void real_set_flush(const bool);
};

} // namespace logger_inner
