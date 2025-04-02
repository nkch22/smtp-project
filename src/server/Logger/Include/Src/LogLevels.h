#pragma once
#include <unordered_map>

#include "SharedInclude.h"

#define DEFAULT_FORMAT "[{:i}] [{:T}] {:t} [{:l}] [{:L}] [{:m}]"

#define INNER_DEFAULT_LEVEL -1

namespace logger
{
using Format = std::string;

class LogLevel
{
private:
	int m_level;
	std::string name;
	Format m_format;
public:
	LogLevel(const LogLevel&);
	LogLevel(LogLevel&&);

	LogLevel(const std::string& name, const Format& ft = DEFAULT_FORMAT, const int level = INNER_DEFAULT_LEVEL);
	~LogLevel() = default;

	void set_int(const int);
	void set_name(const std::string&);
	void set_format(const Format&);

	int get_int() const;
	const std::string& get_name() const;
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
	using inner_map = std::unordered_map<std::string, LogLevel>;

	static inner_map m_levels_map;

public:
	static void add(LogLevel&);

	static void edit(const std::string&, const LogLevel&);

	static int get_size();

	static const LogLevel& get(const std::string&);
};

} // namespace logger_inner

