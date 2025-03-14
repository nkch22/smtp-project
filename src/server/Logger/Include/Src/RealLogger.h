#pragma once
#include "SharedInclude.h"


class RealLogger
{
private:
	struct Message
	{
		std::string msg;
		logger::MessageTypes type;
		std::source_location location;
		logger::LogLevels level;
		std::thread::id thr_id;
	};

	using queue = std::queue<Message>;
	static RealLogger* m_instance;

	logger::LogLevels m_level;
	std::string m_output_path;
	std::ofstream m_file;

	std::mutex m_mutex;
	std::condition_variable m_con_var;

	bool m_end;
	bool m_do_flush;
	bool m_is_config;

	unsigned int m_amount;

	queue m_queue;
	std::thread m_thr;

	RealLogger(const logger::LogLevels, const std::string&, const unsigned int, const bool, const bool);

	~RealLogger() = default;

	void file_init(const unsigned int);

public:
	static RealLogger* get_instance(const logger::LogLevels = DEFAULT_LEVEL, const std::string& = DEFAULT_PATH,
									const unsigned int amount = DEFAULT_AMOUNT, const bool is_config = DEFAULT_CONFIG,
									const bool do_flush = DEFAULT_FLUSH);

	static void destroy();

	void save_to_queue(const std::string&, const logger::MessageTypes, const std::source_location&,
					   const logger::LogLevels level, std::thread::id id = std::this_thread::get_id());

	void real_set_level(const logger::LogLevels);
	logger::LogLevels real_get_level();

	void flush_message(const Message&);

	static void handle_fatal_error();

	void real_stop_config();

	void set_output(const std::string&);

	void real_set_flush(const bool);
};
