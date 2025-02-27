#include "Logger.h"

// RealLogger

using namespace logger;

Logger::RealLogger* Logger::RealLogger::m_instance = nullptr;

LogLevels* Logger::RealLogger::m_level = nullptr;
std::string* Logger::RealLogger::m_output_path = nullptr;
std::ofstream* Logger::RealLogger::m_file = nullptr;

std::mutex* Logger::RealLogger::m_mutex = nullptr;
std::condition_variable* Logger::RealLogger::m_con_var = nullptr;
bool* Logger::RealLogger::m_end = nullptr;

std::thread* Logger::RealLogger::m_thr = nullptr;
Logger::RealLogger::queue* Logger::RealLogger::m_queue = nullptr;

Logger::RealLogger::RealLogger(const LogLevels& _level, const std::string& _save, const unsigned int& amount)
{
	if (amount <= 0) throw std::invalid_argument{"logs amount cannot be less than 1"};
	if (_level < LOG_LEVEL_NO || _level > LOG_LEVEL_TRACE) throw std::invalid_argument{"invalid log level"};

	m_level = new LogLevels{_level};
	m_output_path = new std::string{_save};

	m_mutex = new std::mutex;
	m_con_var = new std::condition_variable;
	m_end = new bool{false};

	m_queue = new queue{};

	std::string log_dir{"Logs"};

	if (std::filesystem::is_directory(*m_output_path))
	{
		log_dir = *m_output_path + "/Logs";
	}
	else if (std::filesystem::is_regular_file(*m_output_path))
		throw std::invalid_argument{"save path cannot be file"};

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

	std::string buff_name =
		log_dir + "/log_" + std::format("{:%d-%m-%y-%H_%M_%S}", std::chrono::system_clock::now()) + ".txt";
	m_file = new std::ofstream{buff_name};

	m_thr = new std::thread{[]
							{
								while (true)
								{
									Message message;
									{
										std::unique_lock<std::mutex> lock{*m_mutex};

										m_con_var->wait(lock, [] { return *m_end || !m_queue->empty(); });

										if (*m_end && m_queue->empty()) return;

										message = m_queue->front();
										m_queue->pop();
									}

									if (message.level != LOG_LEVEL_NO)
									{
										std::string time =
											std::format("[{:%H_%M_%S}]", std::chrono::system_clock::now());

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

										{
											std::lock_guard<std::mutex> lock{*m_mutex};
											std::cout << DEFAULT_COLOR "[" << std::this_thread::get_id() << "]" << time;

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

											std::cout << message_type << DEFAULT_COLOR << level_str << func_name << " "
													  << message.msg << "\n";

											*m_file << "[" << std::this_thread::get_id() << "]" << time << message_type
													<< level_str << func_name << " " << message.msg << "\n";
										}
									}
								}
							}};
};

Logger::RealLogger* Logger::RealLogger::get_instance()
{
	return get_instance(DEFAULT_LEVEL, {}, DEFAULT_AMOUNT);
}

Logger::RealLogger* Logger::RealLogger::get_instance(const LogLevels& level, const std::string& path,
													 const unsigned int& amount)
{
	if (m_instance == nullptr)
	{
		m_instance = new Logger::RealLogger{level, path, amount};
	}
	return m_instance;
}

void Logger::RealLogger::destroy()
{
	if (m_instance != nullptr)
	{
		{
			std::unique_lock<std::mutex> lock{*m_mutex};
			*m_end = 1;
		}
		m_con_var->notify_all();

		m_thr->join();

		delete m_level;
		delete m_output_path;
		delete m_file;

		delete m_mutex;
		delete m_con_var;
		delete m_end;

		delete m_thr;
		delete m_queue;

		delete m_instance;
		m_instance = nullptr;
	}
}

void Logger::RealLogger::real_save(const std::string& str, const Logger::MessageTypes& type,
								   const std::source_location& location, const LogLevels& level)
{
	std::unique_lock<std::mutex> lock{*m_mutex};
	m_queue->emplace(Message{str, type, location, level});
}

void Logger::RealLogger::real_set_level(const LogLevels& _level)
{
	std::lock_guard<std::mutex> lock{*m_mutex};
	*m_level = _level;
}
LogLevels Logger::RealLogger::real_get_level()
{
	std::lock_guard<std::mutex> lock{*m_mutex};
	return *m_level;
}

// Logger

Logger::Logger(const std::source_location location) :
	m_real{Logger::RealLogger::get_instance()}, m_location{location}, m_local_level{m_real->real_get_level()} {};

bool Logger::init(const LogLevels& level, const std::string& save_path, const unsigned int& amount)
{
	Logger::RealLogger* real = Logger::RealLogger::get_instance(level, save_path, amount);
	bool result = real != nullptr;

	if (result)
	{
		Logger log;
		log.save_message("logger is successfully initialized");
	}

	return result;
}

bool Logger::destroy()
{
	Logger::RealLogger* buff = Logger::RealLogger::get_instance();

	Logger log;
	log.save_message("logger is destroyed");

	Logger::RealLogger::destroy();
	return buff == nullptr;
}

void Logger::save_error(const std::string& msg)
{
	m_real->real_save(msg, ERROR, m_location, m_local_level);
}

void Logger::save_warning(const std::string& msg)
{
	m_real->real_save(msg, WARNING, m_location, m_local_level);
}

void Logger::save_message(const std::string& msg)
{
	m_real->real_save(msg, INFORMATION, m_location, m_local_level);
}

void Logger::set_global_level(const LogLevels& _level)
{
	if (_level >= LOG_LEVEL_NO && _level <= LOG_LEVEL_TRACE)
		m_real->real_set_level(_level);
	else
		throw std::invalid_argument{"invalid level argumet"};
}
LogLevels Logger::get_global_level() const
{
	return m_real->real_get_level();
}

void Logger::save_return_nothing()
{
	if (m_local_level >= LOG_LEVEL_DEBUG)
		m_real->real_save("successfully executed", INFORMATION, m_location, m_local_level);
}

void Logger::save_func_start()
{
	if (m_local_level >= LOG_LEVEL_DEBUG) m_real->real_save("started", INFORMATION, m_location, m_local_level);
}

void Logger::save_arguments()
{
	m_real->real_save({"arguments: " + m_buff.get()}, INFORMATION, m_location, m_local_level);
	m_buff.clear();
}

void Logger::set_local_level(const LogLevels& level)
{
	m_local_level = level;
}

LogLevels Logger::get_local_level() const
{
	return m_local_level;
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

Buffer& Buffer::operator<<(const int& value)
{
	*m_real_buff += std::to_string(value);
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const unsigned int& value)
{
	*m_real_buff += std::to_string(value);
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const double& value)
{
	*m_real_buff += std::to_string(value);
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const bool& value)
{
	*m_real_buff += std::to_string((int)value);
	*m_real_buff += " ";
	return *this;
}

// MainLogger

MainLogger::MainLogger(const LogLevels& level, const std::string& path, const unsigned int& amount,
					   const std::source_location location)
{
	Logger::init(level, path, amount);
	m_log = new Logger{location};
}

MainLogger::~MainLogger()
{
	delete m_log;
	Logger::destroy();
}

Logger& MainLogger::get() const
{
	return *m_log;
}