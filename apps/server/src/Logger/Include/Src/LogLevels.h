#pragma once
#include <unordered_map>

#include "SharedInclude.h"

#define NO_LOG_LEVEL logger_inner::GlobalLogLevel::get(0)
#define PROD_LOG_LEVEL logger_inner::GlobalLogLevel::get(1)
#define DEBUG_LOG_LEVEL logger_inner::GlobalLogLevel::get(2)
#define TRACE_LOG_LEVEL logger_inner::GlobalLogLevel::get(3)

#define DEFAULT_LOG_LEVEL PROD_LOG_LEVEL

#define INNER_DEFAULT_LEVEL -1

namespace logger
{
using Format = std::string;

class LogLevel
{
private:
	int m_level;
	Format m_format;
public:
	LogLevel(const LogLevel&);
	LogLevel(LogLevel&&);

	LogLevel(const Format& ft = DEFAULT_FORMAT, const int level = INNER_DEFAULT_LEVEL);
	~LogLevel() = default;

	void set_level(const int);
	void set_format(const Format&);

	int get_level() const;
	const Format& get_format() const;

	void operator=(const LogLevel&);

	const bool operator==(const LogLevel&) const;
};
}

namespace logger_inner
{
using logger::LogLevel;

class GlobalLogLevel
{
private:
	using inner_map = std::unordered_map<int, LogLevel>;

	static inner_map m_levels_map;

public:
	static void add(LogLevel&);

	static void edit(const int, const LogLevel&);

	static void erase(const int);

	static int get_size();

	static const LogLevel& get(const int);
};

} // namespace logger_inner

