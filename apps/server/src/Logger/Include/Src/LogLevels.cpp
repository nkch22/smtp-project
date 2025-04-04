#include "LogLevels.h"

// Log level
using namespace logger;

LogLevel::LogLevel(const LogLevel& obj) : m_level{obj.m_level}, m_format{obj.m_format} {}
LogLevel::LogLevel(LogLevel&& obj) :
	m_level{std::move(obj.m_level)}, m_format{std::move(obj.m_format)} {};

LogLevel::LogLevel( const Format& ft, const int level) :
	m_level{level}, m_format{ft} {};

void LogLevel::set_level(const int value)
{
	m_level = value;
}
void LogLevel::set_format(const Format& value)
{
	m_format = value;
}

int LogLevel::get_level() const
{
	return m_level;
}
const Format& LogLevel::get_format() const
{
	return m_format;
}

void LogLevel::operator=(const LogLevel& obj)
{
	m_format = obj.m_format;
	m_level = obj.m_level;
}

const bool LogLevel::operator==(const LogLevel& obj) const
{
	return m_level == obj.m_level;
}

// Global log levels
using namespace logger_inner;

GlobalLogLevel::inner_map GlobalLogLevel::m_levels_map{{0, LogLevel{FORMAT_NO, 0}},
													   {1, LogLevel{FORMAT_PROD, 1}},
													   {2, LogLevel{FORMAT_DEBUG, 2}},
													   {3, LogLevel{FORMAT_TRACE, 3}}};

void GlobalLogLevel::add(LogLevel& lv)
{
	if (m_levels_map.contains(lv.get_level())) return;

	if (lv.get_level() == -1) lv.set_level(m_levels_map.size());

	m_levels_map.emplace(lv.get_level(), lv);
}

void GlobalLogLevel::edit(const int key, const LogLevel& obj)
{
	if (!m_levels_map.contains(key)) return;

	m_levels_map[key] = obj;
}

void GlobalLogLevel::erase(const int key) {
	if (key <= 3) return;
	m_levels_map.erase(key);
}

int GlobalLogLevel::get_size()
{
	return m_levels_map.size();
}

const LogLevel& GlobalLogLevel::get(const int key)
{
	return m_levels_map[key];
}