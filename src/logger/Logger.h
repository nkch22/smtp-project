#pragma once

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <source_location>
#include <string>
#include <thread>

#define LOG_START_ARGS(...)                                                                                            \
	Logger log;                                                                                                        \
	switch (log.get_level())                                                                                           \
	{                                                                                                                  \
	case LOG_LEVEL_TRACE:                                                                                              \
		log.save_arguments(__VA_ARGS__);                                                                               \
		break;                                                                                                         \
	case LOG_LEVEL_DEBUG:                                                                                              \
		log.save_func_start();                                                                                         \
		break;                                                                                                         \
	}

#define LOG_START()                                                                                                    \
	Logger log;                                                                                                        \
	if (log.get_level() >= LOG_LEVEL_DEBUG) log.save_func_start()

#define LOG_RETURN(value)                                                                                              \
	switch (log.get_level())                                                                                           \
	{                                                                                                                  \
	case LOG_LEVEL_TRACE:                                                                                              \
		log.save_return(value);                                                                                        \
		break;                                                                                                         \
	case LOG_LEVEL_DEBUG:                                                                                              \
		log.save_return_nothing();                                                                                     \
		break;                                                                                                         \
	}                                                                                                                  \
	return value

#define LOG_RETURN_NOTHING()                                                                                           \
	if (log.get_level() >= LOG_LEVEL_DEBUG) log.save_return_nothing()

#define LOG_SAVE_ERROR(value) log.save_error(value)
#define LOG_SAVE_WARNING(value) log.save_warning(value)
#define LOG_SAVE_MESSAGE(value) log.save_message(value)

enum LogLevels
{
	LOG_LEVEL_NO,
	LOG_LEVEL_PROD,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE
};

class Buffer
{
private:
	std::string* m_real_buff;

public:
	Buffer();
	Buffer(const std::string&);

	~Buffer();

	std::string get() const;

	void clear();

	Buffer& operator<<(const std::string&);
	Buffer& operator<<(const char*);

	Buffer& operator<<(const int&);
	Buffer& operator<<(const unsigned int&);
	Buffer& operator<<(const double&);
	Buffer& operator<<(const bool&);

	template<typename T>
	Buffer& operator<<(const T&)
	{
		throw std::exception{};
	}
};

class Logger
{
private:
	enum MessageTypes
	{
		ERROR,
		WARNING,
		INFORMATION
	};

	class RealLogger
	{
	private:
		static RealLogger* m_instance;

		static unsigned short* m_level;
		static std::string* m_output_path;
		static std::ofstream* m_file;
		static std::mutex* m_mutex;

		RealLogger(const unsigned short&, const std::string&, const unsigned int&);

		~RealLogger() = default;

	public:
		static RealLogger* get_instance();
		static RealLogger* get_instance(const unsigned short&, const std::string&, const unsigned int&);

		static void destroy();

		void real_save(const std::string&, const Logger::MessageTypes&, const std::source_location&);

		void real_set_level(const unsigned short&);
		unsigned short real_get_level() const;
	};

	RealLogger* m_real;

	Buffer m_buff;

	const std::source_location m_location;

	template<typename T>
	void save_argument(const T& value)
	{
		m_buff << value;
	}

	void save_arguments();

public:
	Logger(const std::source_location location = std::source_location::current());
	~Logger();

	static bool init();
	static bool init(const unsigned short& level);
	static bool init(const std::string& save_path);
	static bool init(const unsigned int& amount);
	static bool init(const unsigned short& level, const std::string& save_path, const unsigned int& amount);

	static bool destroy();

	void save_error(const std::string&);
	void save_warning(const std::string&);
	void save_message(const std::string&);

	void set_level(const unsigned short&);
	unsigned short get_level() const;

	template<typename T>
	void save_return(const T& value)
	{
		m_buff << value;
		m_real->real_save({"returned: " + m_buff.get()}, INFORMATION, m_location);
		m_buff.clear();
	}

	void save_return_nothing();

	template<typename T, typename... Args>
	void save_arguments(const T& first, Args&... args)
	{
		save_argument(first);
		save_arguments(std::forward<Args>(args)...);
	}

	void save_func_start();
};

class MainLogger
{
private:
	Logger* m_log;

public:
	MainLogger();
	MainLogger(const unsigned short&);
	MainLogger(const unsigned short&, const std::string&);
	MainLogger(const unsigned short&, const std::string&, const unsigned int&);

	~MainLogger();

	Logger& get() const;
};