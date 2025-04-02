#pragma once

#include "SharedInclude.h"
#include "Concurrency/UnboundedBlockingMPMCQueue.h"
#include "ThreadMap.h"
#include "MessageFormatter.h"

namespace logger_inner
{

class RealLogger
{
private:

	using Queue = concurrency::UnboundedBlockingMPMCQueue<Message>;

	static RealLogger* m_instance;

	logger::LogLevel m_level;
	std::string m_output_path;
	std::ofstream m_file;

	std::mutex m_mutex;

	bool m_end;
	bool m_do_flush;
	bool m_is_config;

	unsigned int m_amount;

	Queue m_queue;
	std::thread m_thr;

	ThreadMap m_thr_map;
	GlobalLogLevel m_level_map;

	RealLogger(const logger::LogLevel, const std::string&, const unsigned int, const bool, const bool);

	~RealLogger() = default;

	void file_init(const unsigned int);

public:
	void operator=(const RealLogger&) = delete;
	RealLogger(const RealLogger&) = delete;
	RealLogger(RealLogger&&) = delete;

	static RealLogger* get_instance(const logger::LogLevel = DEFAULT_LOG_LEVEL, const std::string& = DEFAULT_PATH,
									const unsigned int amount = DEFAULT_AMOUNT, const bool is_config = DEFAULT_CONFIG,
									const bool do_flush = DEFAULT_FLUSH);

	static void destroy();

	void save_to_queue(const std::string&, const logger::MessageTypes, const std::source_location&,
					   const logger::LogLevel level, std::thread::id id = std::this_thread::get_id());

	void real_set_level(const logger::LogLevel);
	logger::LogLevel real_get_level();

	void flush_message(const Message&);

	static void handle_fatal_error();

	void real_stop_config();

	void set_output(const std::string&);

	std::string get_path() const;

	void real_set_flush(const bool);

	void add_custom_level(LogLevel&);
};

} // namespace logger
