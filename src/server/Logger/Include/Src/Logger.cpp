#include "../Logger.h"

#include "RealLogger.h"

using namespace logger;
using namespace logger_inner;

Logger::Logger(const char* loc) :
	m_real{RealLogger::get_instance()}, m_location{loc}, m_local_level{m_real->real_get_level()},
	m_local_format{m_real->get_global_format()} {};

bool Logger::init(const LogLevel level, const std::string& save_path, const Format& ft, const unsigned int amount,
				  const bool is_config, const bool do_flush)
{
	RealLogger* real = RealLogger::get_instance(level, save_path, ft, amount, is_config, do_flush);
	bool result = real != nullptr;

	if (result)
	{
		LOGGER(log);
		log.log_message("logger is successfully initialized");
	}

	return result;
}

void Logger::log_error(const std::string& msg)
{
	m_real->save_to_queue(msg, ERROR, m_location, m_local_level, m_local_format);
}

void Logger::log_warning(const std::string& msg)
{
	m_real->save_to_queue(msg, WARNING, m_location, m_local_level, m_local_format);
}

void Logger::log_message(const std::string& msg)
{
	m_real->save_to_queue(msg, INFORMATION, m_location, m_local_level, m_local_format);
}

void Logger::set_global_level(const LogLevel _level)
{
	RealLogger::get_instance()->real_set_level(_level);
}
const LogLevel& Logger::get_global_level() const
{
	return m_real->real_get_level();
}

void Logger::log_return_nothing()
{
	if (m_local_level.get_int() >= 2)
		m_real->save_to_queue("successfully executed", INFORMATION, m_location, m_local_level, m_local_format);
}

void Logger::log_func_start()
{
	if (m_local_level.get_int() >= 2)
		m_real->save_to_queue("started", INFORMATION, m_location, m_local_level, m_local_format);
}

void Logger::log_arguments()
{
	m_real->save_to_queue({"arguments: " + m_buff.get()}, INFORMATION, m_location, m_local_level, m_local_format);
	m_buff.clear();
}

void Logger::set_local_level(const LogLevel level)
{
	m_local_level = level;
}

const LogLevel& Logger::get_local_level() const
{
	return m_local_level;
}

void Logger::set_local_format(const Format& ft)
{
	m_local_format = ft;
}

const Format& Logger::get_local_format() const
{
	return m_local_format;
}

void Logger::stop_config()
{
	RealLogger::get_instance()->real_stop_config();
}

void Logger::set_output_dir(const std::string& path)
{
	RealLogger::get_instance()->set_output(path);
}

std::string Logger::get_output_path()
{
	return RealLogger::get_instance()->get_path();
}

void Logger::set_flush(const bool value)
{
	RealLogger::get_instance()->real_set_flush(value);
}

void Logger::set_global_format(const Format& ft)
{
	RealLogger::get_instance()->set_global_format(ft);
}

const Format& Logger::get_global_format()
{
	return RealLogger::get_instance()->get_global_format();
}