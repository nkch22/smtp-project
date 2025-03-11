#include "../Logger.h"
#include "RealLogger.h"

// RealLogger

using namespace logger;

RealLogger* RealLogger::m_instance = nullptr;

RealLogger::RealLogger(const LogLevels _level, const std::string& _save, const unsigned int amount,
					   const bool is_config, const bool do_flush) 
	:
	m_level{_level}, m_output_path{_save}, m_end{false}, m_do_flush{do_flush}, m_is_config{is_config}, m_amount{amount}, m_thr{}
{
	if (amount <= 0) throw std::invalid_argument{"logs amount cannot be less than 1"};
	if (!is_config) file_init(amount);

	m_thr = std::thread{[this]
						{
							std::unique_lock<std::mutex> lock{m_mutex};
							while (true)
							{
								if (!m_end)
								{
									m_con_var.wait(lock, [this]() { return (!m_queue.empty() && !m_is_config); });
								}
								else
								{
									while (!m_queue.empty())
									{
										flush_message(m_queue.front());
										m_queue.pop();
									}
									break;
								}

								queue localQueue;
								localQueue.swap(m_queue);
								lock.unlock();

								while (!localQueue.empty())
								{
									flush_message(localQueue.front());
									localQueue.pop();
								}

								lock.lock();
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

	if (error)
		save_to_queue("invalid output path, default will be used", WARNING, std::source_location::current(), m_level,
				  std::this_thread::get_id());
}

RealLogger* RealLogger::get_instance(const LogLevels level, const std::string& path,
													 const unsigned int amount, const bool is_config,
													 const bool do_flush)
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
		std::unique_lock<std::mutex> lock{m_instance->m_mutex};
		m_instance->m_end = 1;
	}
	m_instance->m_con_var.notify_all();

	m_instance->m_thr.join();

	delete m_instance;
	m_instance = nullptr;
}

void RealLogger::save_to_queue(const std::string& str, const MessageTypes type,
								   const std::source_location& location, const LogLevels level, std::thread::id id)
{
	if (!m_do_flush) return;

	{
		std::unique_lock<std::mutex> lock{m_mutex};
		m_queue.emplace(Message{str, type, location, level, id});
	}
	m_con_var.notify_all();
}

void RealLogger::real_set_level(const LogLevels _level)
{
	std::unique_lock<std::mutex> lock{m_mutex};
	m_level = _level;
}
LogLevels RealLogger::real_get_level()
{
	std::unique_lock<std::mutex> lock{m_mutex};
	return m_level;
}

void RealLogger::flush_message(const Message& message)
{
	if (message.level == LOG_LEVEL_NO) return;

	std::string time = std::format("[{:%H_%M_%S}]", std::chrono::system_clock::now());

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

	std::cout << DEFAULT_COLOR "[" << message.thr_id << "]" << time;

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

	m_file << "[" << message.thr_id << "]" << time << message_type << level_str << func_name << " " << message.msg
		   << "\n";
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
		buff->save_to_queue(str, ERROR, std::source_location::current(),
												 buff->real_get_level(), std::thread::id{});
	}

	Logger::destroy();

	std::abort();
}

void RealLogger::real_stop_config()
{
	if (!m_is_config) return;

	file_init(m_amount);

	{
		std::lock_guard<std::mutex> lock{m_mutex};
		m_is_config = 0;
	}

	m_con_var.notify_all();
}

void RealLogger::set_output(const std::string& path)
{
	std::unique_lock<std::mutex> lock{m_mutex};
	m_output_path = path;
}

void RealLogger::real_set_flush(const bool value)
{
	m_do_flush = value;
}

// Logger

Logger::Logger(const std::source_location location) :
	m_real{RealLogger::get_instance()}, m_location{location}, m_local_level{m_real->real_get_level()} {};

bool Logger::init(const LogLevels level, const std::string& save_path, const unsigned int amount,
				  const bool is_config, const bool do_flush)
{
	RealLogger* real = RealLogger::get_instance(level, save_path, amount, is_config, do_flush);
	bool result = real != nullptr;

	if (result)
	{
		Logger log;
		log.log_message("logger is successfully initialized");
	}

	return result;
}

void Logger::destroy()
{
	auto buff = RealLogger::get_instance();
	buff->save_to_queue("logger is destroyed", INFORMATION, std::source_location::current(),
								 buff->real_get_level(), std::thread::id{});

	RealLogger::destroy();
}

void Logger::log_error(const std::string& msg)
{
	m_real->save_to_queue(msg, ERROR, m_location, m_local_level);
}

void Logger::log_warning(const std::string& msg)
{
	m_real->save_to_queue(msg, WARNING, m_location, m_local_level);
}

void Logger::log_message(const std::string& msg)
{
	m_real->save_to_queue(msg, INFORMATION, m_location, m_local_level);
}

void Logger::set_global_level(const LogLevels _level)
{
	m_real->real_set_level(_level);
}
LogLevels Logger::get_global_level() const
{
	return m_real->real_get_level();
}

void Logger::log_return_nothing()
{
	if (m_local_level >= LOG_LEVEL_DEBUG)
		m_real->save_to_queue("successfully executed", INFORMATION, m_location, m_local_level);
}

void Logger::log_func_start()
{
	if (m_local_level >= LOG_LEVEL_DEBUG) m_real->save_to_queue("started", INFORMATION, m_location, m_local_level);
}

void Logger::log_arguments()
{
	m_real->save_to_queue({"arguments: " + m_buff.get()}, INFORMATION, m_location, m_local_level);
	m_buff.clear();
}

void Logger::set_local_level(const LogLevels level)
{
	m_local_level = level;
}

LogLevels Logger::get_local_level() const
{
	return m_local_level;
}

void Logger::stop_config()
{
	RealLogger::get_instance()->real_stop_config();
}

void Logger::set_output_dir(const std::string& path)
{
	RealLogger::get_instance()->set_output(path);
}

void Logger::set_flush(const bool value)
{
	RealLogger::get_instance()->real_set_flush(value);
}

// Buffer

Buffer::Buffer() : Buffer{""} {}
Buffer::Buffer(const std::string& str) : m_real_buff{new std::string{str}} {}

Buffer::~Buffer()
{
	delete m_real_buff;
}

std::string Buffer::get() const
{
	return *m_real_buff;
}

void Buffer::clear()
{
	m_real_buff->clear();
}

Buffer& Buffer::operator<<(const std::string& str)
{
	*m_real_buff += str;
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const char* str)
{
	*m_real_buff += str;
	*m_real_buff += " ";
	return *this;
}

Buffer& Buffer::operator<<(const int value)
{
	*m_real_buff += std::to_string(value);
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const unsigned int value)
{
	*m_real_buff += std::to_string(value);
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const double value)
{
	*m_real_buff += std::to_string(value);
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const bool value)
{
	*m_real_buff += std::to_string((int)value);
	*m_real_buff += " ";
	return *this;
}