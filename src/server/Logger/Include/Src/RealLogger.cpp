#include "RealLogger.h"

using namespace logger_inner;

RealLogger* RealLogger::m_instance = nullptr;

RealLogger::RealLogger(const LogLevels _level, const std::string& _save, const unsigned int amount,
					   const bool is_config, const bool do_flush) :
	m_level{_level}, m_output_path{_save}, m_end{false}, m_do_flush{do_flush}, m_is_config{is_config}, m_amount{amount}
{
	if (m_amount < 1)
	{
		save_to_queue("logs amount cannot be less than 1, Default value will be used instead", WARNING,
					  std::source_location::current(), m_level, std::this_thread::get_id());
		m_amount = DEFAULT_AMOUNT;
	}

	if (!is_config) file_init(m_amount);

	m_thr = std::thread{[this]
						{
							while (!m_end)
							{
								std::queue<Message> localQueue = m_queue.Extract();
								while (!localQueue.empty())
								{
									flush_message(localQueue.front());
									localQueue.pop();
								}
							}
							// process messages that left in the queue
							auto message = m_queue.Pop();
							while (message)
							{
								flush_message(*message);
								message = m_queue.Pop();
							}
						}};
};

void RealLogger::file_init(const unsigned int amount)
{
	std::string log_dir{"Logs"};

	bool error = 0;
	if (std::filesystem::is_directory(m_output_path))
	{
		log_dir = m_output_path + "/Logs";
	}
	else if (m_output_path != "")
		error = 1;

	if (std::filesystem::is_directory(log_dir))
	{
		unsigned int count = std::distance(std::filesystem::directory_iterator{log_dir}, {});
		if (count >= amount)
		{
			for (const auto& file : std::filesystem::directory_iterator(log_dir))
			{
				std::filesystem::remove(file);

				count = std::distance(std::filesystem::directory_iterator{log_dir}, {});
				if (count < amount) break;
			}
		}
	}
	else
	{
		std::filesystem::create_directory(log_dir);
	}

	std::string buff_name = log_dir + "/log_";
	buff_name += std::format("{:%d-%m-%y-%H_%M_%S}", std::chrono::system_clock::now()) + ".txt";

	m_file = std::ofstream{buff_name};

	m_output_path = buff_name;

	if (error)
		save_to_queue("invalid output path, default will be used", WARNING, std::source_location::current(), m_level,
					  std::this_thread::get_id());
}

RealLogger* RealLogger::get_instance(const LogLevels level, const std::string& path, const unsigned int amount,
									 const bool is_config, const bool do_flush)
{
	if (m_instance == nullptr)
	{
		m_instance = new RealLogger{level, path, amount, is_config, do_flush};

		atexit([] { Logger::destroy(); });
		std::set_terminate(handle_fatal_error);
	}
	return m_instance;
}

void RealLogger::destroy()
{
	if (m_instance == nullptr) return;

	{
		std::lock_guard guard{m_instance->m_mutex};
		m_instance->m_end = 1;
	}

	m_instance->m_queue.Close();
	m_instance->m_thr.join();

	delete m_instance;
	m_instance = nullptr;
}

void RealLogger::save_to_queue(const std::string& str, const MessageTypes type, const std::source_location& location,
							   const LogLevels level, std::thread::id id)
{
	if (!m_do_flush || level == LOG_LEVEL_NO) return;

	map.add(id);
	m_queue.Push(Message{str, type, location, level, id});
}

void RealLogger::real_set_level(const LogLevels _level)
{
	std::lock_guard guard{m_mutex};
	m_level = _level;
}
LogLevels RealLogger::real_get_level()
{
	std::lock_guard guard{m_mutex};
	return m_level;
}

void RealLogger::flush_message(const Message& message)
{
	std::string time = std::format("[{:%H.%M.%S-%d.%m.%y}]", std::chrono::system_clock::now());

	std::string message_type;
	switch (message.type)
	{
	case ERROR:
		message_type = " E ";
		break;
	case WARNING:
		message_type = " W ";
		break;
	case INFORMATION:
		message_type = " I ";
		break;
	}

	std::string level_str{"[" + std::to_string(message.level) + "]"};

	std::string func_name{"["};
	func_name += message.location.function_name();
	func_name += "]";

	std::cout << DEFAULT_COLOR "[" << map.get(message.thr_id) << "]" << time;

	switch (message.type)
	{
	case ERROR:
		std::cout << ERROR_COLOR;
		break;
	case WARNING:
		std::cout << WARNING_COLOR;
		break;
	case INFORMATION:
		std::cout << INFORMATION_COLOR;
		break;
	}

	std::cout << message_type << DEFAULT_COLOR << level_str << func_name << " " << message.msg << "\n";

	if (m_is_config) return;

	m_file << "[" << map.get(message.thr_id) << "]" << time << message_type << level_str << func_name << " "
		   << message.msg << "\n";
}

void RealLogger::handle_fatal_error()
{
	auto buff = RealLogger::get_instance();
	if (buff->m_is_config) std::abort();

	try
	{
		std::rethrow_exception(std::current_exception());
	}
	catch (const std::exception& ex)
	{
		std::string str{"Fatal error: "};
		str += ex.what();
		buff->save_to_queue(str, ERROR, std::source_location::current(), buff->real_get_level(), std::thread::id{});
	}

	Logger::destroy();

	std::abort();
}

void RealLogger::real_stop_config()
{
	if (!m_is_config) return;

	file_init(m_amount);

	{
		std::lock_guard lock{m_mutex};
		m_is_config = 0;
	}
}

void RealLogger::set_output(const std::string& path)
{
	std::lock_guard guard{m_mutex};
	m_output_path = path;
}

std::string RealLogger::get_path() const
{
	return m_output_path;
}

void RealLogger::real_set_flush(const bool value)
{
	m_do_flush = value;
}
