#include "LogLevels.h"

// Log level
using namespace logger;

LogLevel::LogLevel(const int numb, const Format& ft) : m_level{numb}, m_format{ft} {}

void LogLevel::set_int(const int value)
{
	m_level = value;
}
void LogLevel::set_format(const Format& value)
{
	m_format = value;
}

int LogLevel::get_int() const
{
	return m_level;
}
const Format& LogLevel::get_format() const
{
	return m_format;
}

// Global log levels
using namespace logger_inner;

GlobalLogLevel::inner_map GlobalLogLevel::m_levels_map{
										{"NO", LogLevel{}},
										{"PROD", LogLevel{}},
										{"DEBUG", LogLevel{}},
										{"TRACE", LogLevel{}}
};

void GlobalLogLevel::add(const std::string& key, const Format& ft)
{
	m_levels_map.emplace(key, std::pair<int, Format>(m_levels_map.size(), ft));
}

void GlobalLogLevel::edit(const std::string& key, const LogLevel& obj)
{
	m_levels_map[key] = obj;
}

int GlobalLogLevel::get_size()
{
	return m_levels_map.size();
}

const LogLevel& GlobalLogLevel::get(const std::string& key)
{
	return m_levels_map[key];
}