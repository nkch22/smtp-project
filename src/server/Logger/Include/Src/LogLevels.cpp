#include "LogLevels.h"

// Log level
using namespace logger;

LogLevel::LogLevel(const LogLevel& obj) : m_level{obj.m_level}, m_format{obj.m_format}, m_name{obj.m_name} {}
LogLevel::LogLevel(LogLevel&& obj) :
	m_level{std::move(obj.m_level)}, m_format{std::move(obj.m_format)}, m_name{std::move(obj.m_name)} {};

LogLevel::LogLevel(const std::string& name, const Format& ft, const int level) :
	m_level{level}, m_format{ft}, m_name{name} {};

void LogLevel::set_int(const int value)
{
	m_level = value;
}
void LogLevel::set_name(const std::string& name)
{
	m_name = name;
}
void LogLevel::set_format(const Format& value)
{
	m_format = value;
}

int LogLevel::get_int() const
{
	return m_level;
}
const std::string& LogLevel::get_name() const
{
	return m_name;
}
const Format& LogLevel::get_format() const
{
	return m_format;
}

void LogLevel::operator=(const LogLevel& obj)
{
	m_format = obj.m_format;
	m_level = obj.m_level;
	m_name = obj.m_name;
}

const bool LogLevel::operator==(const LogLevel& obj) const
{
	return m_name == obj.m_name;
}

// Global log levels
using namespace logger_inner;

GlobalLogLevel::inner_map GlobalLogLevel::m_levels_map{{"NO", LogLevel{"NO", FORMAT_NO, 0}},
													   {"PROD", LogLevel{"PROD", FORMAT_PROD, 1}},
													   {"DEBUG", LogLevel{"DEBUG", FORMAT_DEBUG, 2}},
													   {"TRACE", LogLevel{"TRACE", FORMAT_TRACE, 3}}};

void GlobalLogLevel::add(LogLevel& lv)
{
	if (m_levels_map.contains(lv.get_name()) || lv.get_name().empty()) return;

	if (lv.get_int() == -1) lv.set_int(m_levels_map.size());

	m_levels_map.emplace(lv.get_name(), lv);
}

void GlobalLogLevel::edit(const std::string& key, const LogLevel& obj)
{
	if (!m_levels_map.contains(key) || obj.get_name().empty()) return;

	m_levels_map[key] = obj;
}

void GlobalLogLevel::erase(const std::string& key) {
	m_levels_map.erase(key);
}

int GlobalLogLevel::get_size()
{
	return m_levels_map.size();
}

const LogLevel& GlobalLogLevel::get(const std::string& key)
{
	return m_levels_map[key];
}